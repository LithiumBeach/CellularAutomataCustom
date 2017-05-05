/*Rule.h generated on Mon 03/27/2017 at 21:05:29.23*/
#pragma once

#include <string>

#include "RuleInterface.h"

struct RuleData
{
	RuleData(){};
	RuleData(int a_ThisColor, int a_MinNumNeighbors, int a_MaxNumNeighbors, bool a_RingLevel[2], int a_IfColor, int a_ThenColor)
	{
		ThisColorIndex = a_ThisColor;

		MinNumNeighbors = a_MinNumNeighbors;
		MaxNumNeighbors = a_MaxNumNeighbors;

		RingLevel[0] = a_RingLevel[0];
		RingLevel[1] = a_RingLevel[1];

		IfColorIndex = a_IfColor;
		ThenColorIndex = a_ThenColor;
	}

	//Concerning cells of this color:
	//std::string ThenColorLabel = "then this Cell will change to color";
	sf::Color ThisColor;
	int ThisColorIndex;

	//If there are (#)
	//std::string MinNumNeighborsLabel = "If there are";
	int MinNumNeighbors;
	int MaxNumNeighbors;

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
	Rule(int a_ThisColor, int a_MinNumNeighbors, int a_MaxNumNeighbors, bool a_RingLevel[2], int a_IfColor, int a_ThenColor, int _containerIndex);
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
	void HandleAdvanceThisColorButton();
	void HandleReverseAdvanceThisColorButton();
	std::function<void()> ptr_HandleAdvanceThisColorButton;
	std::function<void()> ptr_HandleReverseAdvanceThisColorButton;

public:
	//Num Neighbor events
	void HandleIncreaseMinNumNeighborsButton();
	std::function<void()> ptr_HandleIncreaseMinNumNeighborsButton;
	void HandleDecreaseMinNumNeighborsButton();
	std::function<void()> ptr_HandleDecreaseMinNumNeighborsButton;

	void HandleIncreaseMaxNumNeighborsButton();
	std::function<void()> ptr_HandleIncreaseMaxNumNeighborsButton;
	void HandleDecreaseMaxNumNeighborsButton();
	std::function<void()> ptr_HandleDecreaseMaxNumNeighborsButton;

	//If Color events
	void HandleAdvanceIfColorButton();
	void HandleReverseAdvanceIfColorButton();
	std::function<void()> ptr_HandleAdvanceIfColorButton;
	std::function<void()> ptr_HandleReverseAdvanceIfColorButton;

	//Then Color events
	void HandleAdvanceThenColorButton();
	void HandleReverseAdvanceThenColorButton();
	std::function<void()> ptr_HandleAdvanceThenColorButton;
	std::function<void()> ptr_HandleReverseAdvanceThenColorButton;

	//handle delete button
	void HandleDelete();
	std::function<void()> ptr_HandleDeleteButton;

public:
	static int s_RuleCount;

public:
	int containerIndex;

	void Refresh(int _newIndex);
};
