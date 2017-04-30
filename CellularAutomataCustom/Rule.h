/*Rule.h generated on Mon 03/27/2017 at 21:05:29.23*/
#pragma once

#include <string>

#include "RuleInterface.h"

struct RuleData
{
	RuleData(){};
	RuleData(int a_NumNeighbors, bool a_RingLevel[2], sf::Color a_IfColor, sf::Color a_ThenColor)
	{
		NumNeighbors = a_NumNeighbors;

		RingLevel[0] = a_RingLevel[0];
		RingLevel[1] = a_RingLevel[1];

		IfColor = a_IfColor;
		ThenColor = a_ThenColor;
	}

	//If there are (#)
	//std::string NumNeighborsLabel = "If there are";
	int NumNeighbors;

	//in ring(s) level around this Cell
	//std::string RingLevelLabel = "in ring(s) level around this Cell";
	bool RingLevel[2];

	//AND they are of color (alive/dead, emotion?)
	//std::string IfColorLabel = "AND they are of color (alive/dead, emotion?)";
	sf::Color IfColor;
	int IfColorIndex;
	//triggering bool for if we want to ignore color in our rule
	//std::string UseIfColorLabel = "Initial Color Conditions?";
	//bool UseIfColor;
	
	//then this Cell will change to color:
	//std::string ThenColorLabel = "then this Cell will change to color";
	sf::Color ThenColor;
	int ThenColorIndex;
};

class Rule
{
public:
	Rule(int a_NumNeighbors, bool a_RingLevel[2], sf::Color a_IfColor, sf::Color a_ThenColor);
	~Rule();

public:
	RuleInterface* m_Interface;
	RuleData* m_RuleData;

public:
	void Update(float dt);
	void Draw();

private:
	void InitializeInterface();
public:
	void HandleIncreaseNumNeighborsButton();
	std::function<void()> ptr_HandleIncreaseNumNeighborsButton;
	void HandleDecreaseNumNeighborsButton();
	std::function<void()> ptr_HandleDecreaseNumNeighborsButton;

	void HandleAdvanceIfColorButton();
	void HandleReverseAdvanceIfColorButton();
	std::function<void()> ptr_HandleAdvanceIfColorButton;
	std::function<void()> ptr_HandleReverseAdvanceIfColorButton;
	//TODO: list of buttons with colors as a static selecta -- ColorPickerButton : Button

	void HandleAdvanceThenColorButton();
	void HandleReverseAdvanceThenColorButton();
	std::function<void()> ptr_HandleAdvanceThenColorButton;
	std::function<void()> ptr_HandleReverseAdvanceThenColorButton;
	//TODO: list of buttons with colors as a static selecta -- ColorPickerButton : Button

public:
	static int s_RuleCount;
};
