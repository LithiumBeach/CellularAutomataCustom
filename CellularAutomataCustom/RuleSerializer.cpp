#include "RuleSerializer.h"
#include <fstream>
#include <stdio.h>

//all data in this serializer must also be serialized..
namespace ruleSerializer
{
	typedef std::fstream fstream;
    typedef std::string string;

	size_t numRulesets = 0;
	//string saveFilepaths[rulesetsMaxLen];

	string saveFilePathPre = "../save/ruleset";
	//after "ruleset", before ".txt", put the number (starting with 1)

	//this character separates rules within rulesets
	char delimiterChar = '~';
	
	std::vector<RuleData> rulesets[rulesetsMaxLen];
	string rulesetNames[rulesetsMaxLen];

	int currentLoadedRuleIndex = 0;


	void Initialize()
	{
		//try to load files named "rulesetX".. if not found, try incrementing X counter a few times.
		//always rename files to be ruleset<RULESETCOUNT>
		//create RuleDatas from these files, sotre them 
		numRulesets = 0;
		fstream afile;
		bool bExit = false;

		//read all files in /save dir that match the file strings.
		while (!bExit && (numRulesets) < rulesetsMaxLen)
		{
			afile.open(RulesetIndexToFilePath(numRulesets), fstream::in);
			if (afile.is_open())
			{
				afile.close();


				//load this file into a vector of ruledatas, save to correct index in array of rulestets
				rulesets[numRulesets] = LoadFrom(RulesetIndexToFilePath(numRulesets));

				numRulesets++;
			}
			else
			{
				bExit = true;
				afile.close();
			}
		}
	}
	string RulesetIndexToFilePath(int i)
	{
		string a = saveFilePathPre;
		a += std::to_string(i+1);
		a += ".txt";
		return (a).c_str();
	}

	int RulesetFilePathToIndex(string _fp)
	{
		int startPosToErase = _fp.find(".txt");
		_fp.erase(startPosToErase, 4);
		_fp.erase(0, saveFilePathPre.size());
		return atoi(_fp.c_str()) - 1;
	}

	void SaveToFile(int _index, std::vector<RuleData> _rd)
	{
		SaveToFile(RulesetIndexToFilePath(_index), _rd);
	}

	//if this file exists, it will write to it. if not, it will make it.
	//only pass the file name size_to the _path param.
	void SaveToFile(string _path, std::vector<RuleData> _rd)
	{
		//delete the file
		bool didDelete = true;
		if (remove((_path).c_str()) != 0)
		{//if we didn't delete the file
			numRulesets++;//we are creating a new rule
			didDelete = false;
		}

		//create output stream into file
		std::fstream myfile;
		myfile.open((_path).c_str(), fstream::out | fstream::in | fstream::app);
		
		string name;
		if (didDelete)
		{
			name = rulesetNames[RulesetFilePathToIndex(_path)];//"ruleset" + (std::to_string(RulesetFilePathToIndex(_path)));
		}
		else
		{
			int index = RulesetFilePathToIndex(_path);
			name = "ruleset" + (std::to_string(index+1));
			rulesetNames[index] = name;
		}
		myfile << name << "\n";
		//write data
		for (size_t i = 0; i < _rd.size(); i++)
		{
			myfile << _rd.at(i).ThisColorIndex << "\n";
			myfile << _rd.at(i).MinNumNeighbors << "\n";
			myfile << _rd.at(i).MaxNumNeighbors << "\n";
			myfile << _rd.at(i).IfColorIndex << "\n";
			myfile << _rd.at(i).ThenColorIndex << "\n";
			myfile << delimiterChar;
			if (i < _rd.size()-1)
			{
				myfile << "\n";
			}
		}
		
		//close file
		myfile.close();
	}

	std::vector<RuleData> LoadFrom(int _index)
	{
		return LoadFrom(RulesetIndexToFilePath(_index));
	}
	std::vector<RuleData> LoadFrom(string _path)
	{
		string line;
		fstream myfile;
		myfile.open(_path, fstream::in | fstream::out);

		if (myfile.is_open())
		{
			std::vector<RuleData> rs;

			//get display name of ruleset
			getline(myfile, line);
			rulesetNames[RulesetFilePathToIndex(_path)] = line;
			line = "";

			while (getline(myfile, line))
			{
				RuleData r = RuleData();
				r.ThisColorIndex = std::stoi(line);
				line = "";
				getline(myfile, line);
				r.MinNumNeighbors = std::stoi(line);
				line = "";
				getline(myfile, line);
				r.MaxNumNeighbors = std::stoi(line);
				line = "";
				getline(myfile, line);
				r.IfColorIndex = std::stoi(line);
				line = "";
				getline(myfile, line);
				r.ThenColorIndex = std::stoi(line);
				line = "";
				rs.push_back(r);

				//consume delimiter char
				getline(myfile, line);
			}

			myfile.close();

			return rs;
		}

		return std::vector<RuleData>();
	}

	void DeleteFile(size_t _index)
	{
		if (_index >= numRulesets || _index < 0)
		{
			printf("\tdanger!");
			return;
		}

		//hold onto the old filename
		//string _path = RulesetIndexToFilePath(_index);

		//shift all array elements back one from _index + 1
		//for (size_t i = _index+2; i < numRulesets; i++)
		//{
		//	//set previous to current == set current to next, offset one
		//	rulesetNames[i - 1] = rulesetNames[i];
		//	rulesets[i - 1] = rulesets[i];
		//}

		//decrement array count
		numRulesets--;

		for (size_t i = _index+1; i < numRulesets+1; i++)
		{
			//read this ruleset data
			std::vector<RuleData> tmpRD = LoadFrom(RulesetIndexToFilePath(i));

			//write to the previous ruleset
			//rulesetNames[i - 1] = rulesetNames[i];
			SaveToFile(RulesetIndexToFilePath(i-1), tmpRD);
		}

		for (size_t i = 1; i < numRulesets; i++)
		{
			rulesets[i] = LoadFrom(RulesetIndexToFilePath(i));
			rulesetNames[i] = "ruleset" + std::to_string(i+1);
		}

		//remove the LAST file (Which by now should be empty)
		remove((RulesetIndexToFilePath(numRulesets)).c_str());
	}
}