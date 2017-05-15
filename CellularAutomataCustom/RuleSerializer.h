/*RuleSerializer.h generated on Tue 05/09/2017 at 21:02:46.13*/
#pragma once

#include <string>

#include "Rule.h"

namespace ruleSerializer
{
	const size_t rulesetsMaxLen = 100;
	extern size_t numRulesets;
	//extern std::string saveFilepaths[];

	extern std::string saveFilePathPre;
	extern char delimiterChar;

	//an array of ruledata vectors
	extern std::vector<RuleData> rulesets[];
	extern std::string rulesetNames[];

	extern int currentLoadedRuleIndex;

	extern void Initialize();
	extern std::string RulesetIndexToFilePath(int i);
	extern int RulesetFilePathToIndex(std::string _fp);

	extern void SaveToFile(int _index, std::vector<RuleData> _rd);
	extern void SaveToFile(std::string _path, std::vector<RuleData> _rd);

	extern std::vector<RuleData> LoadFrom(int _index);
	extern std::vector<RuleData> LoadFrom(std::string _path);
	extern void DeleteFile(size_t _index);
};
 
