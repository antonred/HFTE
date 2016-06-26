

#include "Account.h"
#include "Logger.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <sodium.h>
#include <cpuid.h>

void dump(const char *prefix, unsigned char *data, int len)
{
	printf("%s dump: ", prefix);
	for (int i = 0; i < len; i++) printf("%x", data[i]);
	printf("  ### size %d\n", len);
}

std::atomic<bool> Account::libInit(false);
std::mutex Account::libInitMutex;

Account::Account()
	: inititalized(false),
	  login(nullptr), login_len(0),
	  pass(nullptr), pass_len(0),
	  salt(nullptr), salt_len(0),
	  login_temp(nullptr), pass_temp(nullptr)
{
	LOGT("Account::Account()");
	if (libInit == false) {
		libInitMutex.lock();
		if (libInit == false) {
			int sodium_init_result = sodium_init();
			assert(sodium_init_result != -1);
		}
		libInitMutex.unlock();
	}
	LOGT("Account::Account() OK");
}
Account::~Account()
{
	LOGT("Account::~Account()");

	Clean();

	if (login != nullptr) delete [] login;
	if (pass != nullptr) delete [] pass;
	if (salt != nullptr) delete [] salt;

	LOGT("Account::~Account() OK");
}


void Account::SetAccountFromClear(const char *symlink, char *login, char *pass)
{
	LOGT("Account::SetAccountFromClear({})", symlink);

	this->symlink = symlink;

	if (salt != nullptr) delete [] salt;
	salt = new unsigned char[crypto_secretbox_NONCEBYTES];
	salt_len = crypto_secretbox_NONCEBYTES;
	randombytes_buf(salt, crypto_secretbox_NONCEBYTES);

	Encrypt(login, &this->login, &login_len);
	Encrypt(pass, &this->pass, &pass_len);

	//dump("###out login",this->login,this->login_len);
	//dump("###out pass",this->pass,this->pass_len);
	//dump("###out salt",this->salt,this->salt_len);

	inititalized = true;

	LOGT("Account::SetAccountFromClear({}) OK", symlink);
}
void Account::SetAccountFromSecret(
	const char *symlink,
	unsigned char *login, int login_len,
	unsigned char *pass, int pass_len,
	unsigned char *salt, int salt_len)
{
	LOGT("Account::SetAccountFromSecret({})", symlink);

	this->symlink = symlink;

	if (this->login != nullptr) delete [] this->login;
	this->login = new unsigned char[login_len];
	memcpy(this->login, login, login_len);
	this->login_len = login_len;

	if (this->pass != nullptr) delete [] this->pass;
	this->pass = new unsigned char[pass_len];
	memcpy(this->pass, pass, pass_len);
	this->pass_len = pass_len;

	if (this->salt != nullptr) delete [] this->salt;
	this->salt = new unsigned char[salt_len];
	memcpy(this->salt, salt, salt_len);
	this->salt_len = salt_len;

	inititalized = true;

	//dump("###in  login",this->login,this->login_len);
	//dump("###in  pass",this->pass,this->pass_len);
	//dump("###in  salt",this->salt,this->salt_len);

	LOGT("Account::SetAccountFromSecret({}) OK", symlink);
}


const char *Account::GetSymlink()
{
	LOGT("Account::GetSymlink()");
	assert(inititalized);
	return symlink.c_str();
}
const char *Account::GetLoginClear()
{
	LOGT("Account::GetLoginClear()");
	assert(inititalized);
	Decrypt(login, login_len, &login_temp);
	return login_temp;
}
const char *Account::GetPassClear()
{
	LOGT("Account::GetPassClear()");
	assert(inititalized);
	Decrypt(pass, pass_len, &pass_temp);
	return pass_temp;
}
void Account::Clean()
{
	if (login_temp != nullptr) {
		int i = 0;
		while (login_temp[i] != 0) login_temp[i++] = 'X';
		delete [] login_temp;
	}
	login_temp = nullptr;

	if (pass_temp != nullptr) {
		int i = 0;
		while (pass_temp[i] != 0) pass_temp[i++] = 'X';
		delete [] pass_temp;
	}
	pass_temp = nullptr;
	LOGT("Account::Clean() cleaned");
}


unsigned char *Account::GetLoginSecret(int *secret_len)
{
	LOGT("Account::GetLoginSecret() login_len {}", login_len);
	assert(inititalized);
	*secret_len = login_len;
	return login;
}
unsigned char *Account::GetPassSecret(int *secret_len)
{
	LOGT("Account::GetPassSecret() pass_len {}", pass_len);
	assert(inititalized);
	*secret_len = pass_len;
	return pass;
}
unsigned char *Account::GetSaltSecret(int *secret_len)
{
	LOGT("Account::GetSaltSecret() salt_len {}", salt_len);
	assert(inititalized);
	*secret_len = salt_len;
	return salt;
}


unsigned char *Account::GetKey()
{
	unsigned eax, ebx, ecx, edx;
	int result = __get_cpuid(0, &eax, &ebx, &ecx, &edx);
	assert(result == 1);

	unsigned char *key = new unsigned char[crypto_secretbox_KEYBYTES];
	memset(key,0,crypto_secretbox_KEYBYTES);
	int key_len = sprintf((char*)key, "%08x%08x%08x%08x", edx, ecx, eax, ebx);
	assert(key_len == 32);

	return key;
}
void Account::Encrypt(char *clear, unsigned char **secret, int *secret_len)
{
	unsigned char *key = GetKey();

	int clear_len = strlen(clear) + 1;

	if (*secret != nullptr) delete [] *secret;
	*secret_len = crypto_secretbox_MACBYTES + clear_len;
	*secret = new unsigned char[*secret_len];

	//printf("ecrypting: %s clear_len %d secret_len %d\n", clear, clear_len, *secret_len);
	int result = crypto_secretbox_easy(*secret, (unsigned char*)clear, clear_len, salt, key);
	delete [] key;
	assert(result == 0);
}
void Account::Decrypt(unsigned char *secret, int secret_len, char **clear)
{
	unsigned char *key = GetKey();

	if (*clear != nullptr) delete [] *clear;
	*clear = new char[secret_len - crypto_secretbox_MACBYTES];

	int result = crypto_secretbox_open_easy((unsigned char*)*clear, secret, secret_len, salt, key);
	delete [] key;
	assert(result == 0);

	//printf("decrypted: %s clear_len %d secret_len %d\n", *clear, strlen(*clear), secret_len);
}

