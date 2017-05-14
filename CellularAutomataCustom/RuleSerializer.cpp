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
	string saveFilePathPost = ".txt";

	//this character separates rules within rulesets
	char delimiterChar = '~';
	
	std::vector<RuleData> rulesets[rulesetsMaxLen];
	string rulesetNames[rulesetsMaxLen];


	void Initialize()
	{
		//try to load files named "rulesetX".. if not found, try incrementing X counter a few times.
		//always rename files to be ruleset<RULESETCOUNT>
		//create RuleDatas from these files, sotre them 
		int rulesetNum = 1;
		fstream afile;
		bool bExit = false;

		//read all files in /save dir that match the file strings.
		while (!bExit && rulesetNum < rulesetsMaxLen)
		{
			afile.open(RulesetIndexToFilePath(rulesetNum), fstream::in);
			if (afile.is_open())
			{
				afile.close();

				//load this file into a vector of ruledatas, save to correct index in array of rulestets
				rulesets[rulesetNum-1] = LoadFrom(RulesetIndexToFilePath(rulesetNum));

				numRulesets++;
				rulesetNum++;
			}
			else
			{
				bExit = true;
			}
		}
	}
	string RulesetIndexToFilePath(int i)
	{
		return (saveFilePathPre + std::to_string(i) + saveFilePathPost).c_str();
	}

	void SaveHeaderFile()
	{
		//create output stream into file
		std::fstream myfile((saveFilePathPre + "header.txt").c_str());

		//open the file
		myfile.open((saveFilePathPre + "header.txt").c_str(), fstream::out | fstream::in);

		//write data
		myfile << numRulesets;

		//close file
		myfile.close();
	}

	//if this file exists, it will write to it. if not, it will make it.
	//only pass the file name size_to the _path param.
	void SaveToFile(string _path, RuleData _rd)
	{
		//youre going to need to add the if not, it will make it. part. ~~ maybe, it should do it automatically if I'm understanding this dumb shit correctly.



		//full filepath
		_path = saveFilePathPre + _path;

		//delete the file
		if (remove((_path + ".txt").c_str()) != 0)
		{
			//count our files
            numRulesets++;
			if (numRulesets > rulesetsMaxLen)
			{
                numRulesets = rulesetsMaxLen;
			}
		}

		//create output stream into file
		std::fstream myfile((_path + ".txt").c_str());
		myfile.open((_path + ".txt").c_str(), fstream::out | fstream::in);

		//open the file
		myfile.open((_path + ".txt").c_str());

		//write data
		myfile << _rd.ThisColorIndex << "\n";
		myfile << _rd.MinNumNeighbors << "\n";
		myfile << _rd.MaxNumNeighbors << "\n";
		myfile << _rd.IfColorIndex << "\n";
		myfile << _rd.ThenColorIndex;
		
		//close file
		myfile.close();
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
			rulesetNames[numRulesets-1] = line;
			line = "";

			string t = rulesetNames[numRulesets - 1];

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

	/*void DeleteFile(size_t _index, bool _fromArrayOnly = false)
	{
		if (_index >= numSaveFiles || _index < 0)
		{
			printf("\tdanger!");
			return;
		}

		//decrement array count
		numSaveFiles--;

		//hold onto the old filename
		string _path = saveFilepaths[_index];

		//shift all array elements back one from _index + 1
		for (size_t i = _index + 1; i < numSaveFiles; i++)
		{
			//set previous to current == set current to next, offset one
			saveFilepaths[i - 1] = saveFilepaths[i];
		}

		//if we don't need to remove the file itself, do not.
		if (_fromArrayOnly)
		{
			return;
		}

		//remove the file
		remove((_path + ".txt").c_str());
	}

	void DeleteFile(string _path, bool _fromArrayOnly=false)
	{
		for (size_t i = 0; i < numSaveFiles; i++)
		{
			if (_path == saveFilepaths[i])
			{
				DeleteFile(i, _fromArrayOnly);
			}
		}
	}*/
}