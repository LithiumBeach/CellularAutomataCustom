/*RuleInterface.h generated on Tue 04/04/2017 at 20:29:19.64*/
#pragma once

#include <SFML\Graphics\RenderWindow.hpp>

#include "Button.h"

extern sf::RenderWindow* g_WINDOW;

//handles all drawing of a Rule's data
//interface changes.

class RuleInterface
{
public:
	typedef sf::Vector2f Vector2f;
public:
	RuleInterface();
	void InitializeButtonsNoEvents();

	//update this manually ahahAHAHAHAAHA
	const float Y_SIZE = 200;
   ~RuleInterface();

public:
   void Update(float a_DeltaTime);
   void Draw();

public:
	//for multiple rulesets, and when we need to scroll the entire rule in the gui
	sf::Vector2f Origin;
	void SetPosition(sf::Vector2f a_Origin);

////////////////////////////////////////////////////////////////
	// Rule Data Representation
public:
	void SetRuleLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text RuleLabelText;

public:
	//MIN NEIGHBORS
	//label
	void SetMinNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text MinNumNeighborsLabelText;
	//data link
	void SetMinNumNeighborsText(int a_MinNumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	void UpdateMinNumNeighborsText(int a_MinNumNeighbors);
	//ui
	sf::Text MinNumNeighborsText;
	Button IncreaseMinNumNeighborsButton;
	Button DecreaseMinNumNeighborsButton;
	//MAX NEIGHBORS
	//label
	void SetMaxNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text MaxNumNeighborsLabelText;
	//data link
	void SetMaxNumNeighborsText(int a_MaxNumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	void UpdateMaxNumNeighborsText(int a_MaxNumNeighbors);
	//ui
	sf::Text MaxNumNeighborsText;
	Button IncreaseMaxNumNeighborsButton;
	Button DecreaseMaxNumNeighborsButton;

public:
	//label
	void SetThisColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text ThisColorLabelText;
	//data link
	void SetChangeThisColorButtonColor(sf::Color a_Color);
	//ui
	Button ChangeThisColorButton;


public:
	//label
	void SetIfColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text IfColorLabelText;
	//data link
	void SetChangeIfColorButtonColor(sf::Color a_Color);
	//ui
	Button ChangeIfColorButton;
public:
	//label
	void SetThenColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text ThenColorLabelText;
	//data link
	void SetChangeThenColorButtonColor(sf::Color a_Color);
	//ui
	Button ChangeThenColorButton;

	Button DeleteSelfButton;

};
