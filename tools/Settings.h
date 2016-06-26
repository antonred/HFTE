
#ifndef HFTE_SETTINGS_H_
#define HFTE_SETTINGS_H_

/*!
 * Based on rapidjson https://github.com/miloyip/rapidjson
 *
 * Parses input json file and write out
 *
 */

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <string>
#include <map>

namespace HFTE
{
	/*!
	 * Loading JSON file with settings and
	 * storing values in map[SECTION]map[PARAMETER][VALUE]
	 */

	class Settings
	{
		//! Full path, e.g. "/home/user/settings.json" or current "settings.json"
		std::string file_path;

		//! All settings container map[SECTION]map[PARAMETER][VALUE]
		std::map<std::string,std::map<std::string,std::string>> settings;

	public:

		Settings();
		Settings(std::string file_path = "settings.json");
		bool Load();
		bool Load(std::string file_path);
		bool Save();
		bool Save(std::string file_path);

		std::string GetString(std::string section, std::string parameter);
		int GetInt(std::string section, std::string parameter);
		double GetDouble(std::string section, std::string parameter);
	};
}

#endif /* HFTE_SETTINGS_H_ */
