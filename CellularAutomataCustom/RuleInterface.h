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
	//label
	void SetNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	sf::Text NumNeighborsLabelText;
	//data link
	void SetNumNeighborsText(int a_NumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor);
	void UpdateNumNeighborsText(int a_NumNeighbors);
	//ui
	sf::Text NumNeighborsText;
	Button IncreaseNumNeighborsButton;
	Button DecreaseNumNeighborsButton;


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

};
