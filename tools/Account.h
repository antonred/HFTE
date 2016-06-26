
#ifndef Account_H_
#define Account_H_

#include <string>
#include <atomic>
#include <mutex>

/*
 * Account
 *
 * Secure storage for login & pass pair with memory dump protection.
 *
 * Clear credentials stored temporary in login_temp & pass_temp.
 *
 * Don't copy credentials outside login_temp & pass_temp.
 *
 * Clear() anytime you are get cleared credentials.
 *
 */

class Account
{
	bool inititalized;

	std::string symlink;

	unsigned char *login; int login_len;
	unsigned char *pass; int pass_len;
	unsigned char *salt; int salt_len;

	char *login_temp;
	char *pass_temp;

	static std::atomic<bool> libInit;
	static std::mutex libInitMutex;

public:

	Account();
	~Account();

	bool IsInitialized();

	void SetAccountFromClear(const char *symlink, char *login, char *pass);
	void SetAccountFromSecret(
		const char *symlink,
		unsigned char *login, int login_len,
		unsigned char *pass, int pass_len,
		unsigned char *salt, int salt_len);

	const char *GetSymlink();
	const char *GetLoginClear();
	const char *GetPassClear();
	void Clean();

	unsigned char *GetLoginSecret(int *secret_len);
	unsigned char *GetPassSecret(int *secret_len);
	unsigned char *GetSaltSecret(int *secret_len);

private:

	unsigned char *GetKey();
	void Encrypt(char *clear, unsigned char **secret, int *secret_len);
	void Decrypt(unsigned char *secret, int secret_len, char **clear);
};

#endif /* Account_H_ */
