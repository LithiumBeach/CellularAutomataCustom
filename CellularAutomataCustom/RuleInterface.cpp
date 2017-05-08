#include "RuleInterface.h"
#include "StaticNamespaces.h"

extern sf::RenderWindow* g_WINDOW;
 
RuleInterface::RuleInterface()
{
	IncreaseMinNumNeighborsButton = Button();
	DecreaseMinNumNeighborsButton = Button();
	IncreaseMaxNumNeighborsButton = Button();
	DecreaseMaxNumNeighborsButton = Button();
	DeleteSelfButton = Button();
	InitializeButtonsNoEvents();
}

void RuleInterface::InitializeButtonsNoEvents()
{
	int inc_dec_button_size = 16;
	IncreaseMinNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "+", 22);
	DecreaseMinNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "-", 22);
	IncreaseMaxNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "+", 22);
	DecreaseMaxNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "-", 22);
	DeleteSelfButton = Button(Vector2f(12, 48), Vector2f(0, 0), caColors::red, caColors::border_red, *caFonts::s_DefaultFont, 2.0f, "X");

	//transparent = temp.
	ChangeThisColorButton = Button(Vector2f(20, 20), Vector2f(0, 0), caColors::transparent, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "", 9);

	ChangeIfColorButton = Button(Vector2f(64, 64), Vector2f(0, 0), caColors::caColors[0], caColors::border_gray, *caFonts::s_DefaultFont, 2.0f);
	ChangeThenColorButton = Button(Vector2f(64, 64), Vector2f(0, 0), caColors::caColors[0], caColors::border_gray, *caFonts::s_DefaultFont, 2.0f);
}

void RuleInterface::SetRuleLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	RuleLabelText.setFont(a_Font);
	RuleLabelText.setCharacterSize(a_CharacterSize);
	RuleLabelText.setColor(a_TextColor);
	RuleLabelText.setStyle(sf::Text::Regular);
	RuleLabelText.setString(a_Words);

	//left aligned -- origin
	RuleLabelText.setOrigin(sf::Vector2f(0, RuleLabelText.getLocalBounds().height));
}

void RuleInterface::SetThisColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	ThisColorLabelText.setFont(a_Font);
	ThisColorLabelText.setCharacterSize(a_CharacterSize);
	ThisColorLabelText.setColor(a_TextColor);
	ThisColorLabelText.setStyle(sf::Text::Style::Regular);
	ThisColorLabelText.setString(a_Words);

	//left aligned -- origin
	ThisColorLabelText.setOrigin(sf::Vector2f(0, ThisColorLabelText.getLocalBounds().height));
}

void RuleInterface::SetIfColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	IfColorLabelText.setFont(a_Font);
	IfColorLabelText.setCharacterSize(a_CharacterSize);
	IfColorLabelText.setColor(a_TextColor);
	IfColorLabelText.setStyle(sf::Text::Style::Regular);
	IfColorLabelText.setString(a_Words);

	//left aligned -- origin
	IfColorLabelText.setOrigin(sf::Vector2f(0, IfColorLabelText.getLocalBounds().height));
}

void RuleInterface::SetThenColorLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	ThenColorLabelText.setFont(a_Font);
	ThenColorLabelText.setCharacterSize(a_CharacterSize);
	ThenColorLabelText.setColor(a_TextColor);
	ThenColorLabelText.setStyle(sf::Text::Style::Regular);
	ThenColorLabelText.setString(a_Words);

	//left aligned -- origin
	ThenColorLabelText.setOrigin(sf::Vector2f(0, ThenColorLabelText.getLocalBounds().height));
}

void RuleInterface::SetChangeThisColorButtonColor(sf::Color a_Color)
{
	ChangeThisColorButton.SetFill(a_Color);
}

void RuleInterface::SetChangeIfColorButtonColor(sf::Color a_Color)
{
	ChangeIfColorButton.SetFill(a_Color);
}

void RuleInterface::SetChangeThenColorButtonColor(sf::Color a_Color)
{
	ChangeThenColorButton.SetFill(a_Color);
}


RuleInterface::~RuleInterface()
{
}

void RuleInterface::SetPosition(sf::Vector2f a_Pos)
{
	Origin = a_Pos;
	//-------------

	RuleLabelText.setOrigin(sf::Vector2f(0, RuleLabelText.getLocalBounds().height));
	RuleLabelText.setPosition(Origin);

	MinNumNeighborsLabelText.setOrigin(sf::Vector2f(0, MinNumNeighborsLabelText.getLocalBounds().height  * 0.5f));
	MinNumNeighborsLabelText.setPosition(a_Pos + sf::Vector2f(25, 44));

	MaxNumNeighborsLabelText.setOrigin(sf::Vector2f(0, MaxNumNeighborsLabelText.getLocalBounds().height  * 0.5f));
	MaxNumNeighborsLabelText.setPosition(a_Pos + sf::Vector2f(133, 44));

	DeleteSelfButton.SetPosition(a_Pos + sf::Vector2f(8, 50));

	///////////////
	//Num Neighbors
	//MIN
	IncreaseMinNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(120, 25));

	MinNumNeighborsText.setOrigin(sf::Vector2f(MinNumNeighborsText.getLocalBounds().width  * 0.5f, MinNumNeighborsText.getLocalBounds().height  * 0.5f));
	MinNumNeighborsText.setPosition(a_Pos + sf::Vector2f(120, 44));

	DecreaseMinNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(120, 75));

	//MAX
	IncreaseMaxNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(180, 25));

	MaxNumNeighborsText.setOrigin(sf::Vector2f(MaxNumNeighborsText.getLocalBounds().width  * 0.5f, MaxNumNeighborsText.getLocalBounds().height  * 0.5f));
	MaxNumNeighborsText.setPosition(a_Pos + sf::Vector2f(180, 44));

	DecreaseMaxNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(180, 75));
	//
	//////////////

	ThisColorLabelText.setOrigin(sf::Vector2f(0, ThisColorLabelText.getLocalBounds().height  * 0.5f));

	ThisColorLabelText.setPosition(a_Pos + sf::Vector2f(70, -19));

	ChangeThisColorButton.SetPosition(a_Pos + sf::Vector2f(265, -9));

	IfColorLabelText.setOrigin(sf::Vector2f(IfColorLabelText.getLocalBounds().width  * 0.5f, IfColorLabelText.getLocalBounds().height  * 0.5f));
	IfColorLabelText.setPosition(a_Pos + sf::Vector2f(255, 22));

	ChangeIfColorButton.SetPosition(a_Pos + sf::Vector2f(350, 50));

	ThenColorLabelText.setOrigin(sf::Vector2f(ThenColorLabelText.getLocalBounds().width  * 0.5f, ThenColorLabelText.getLocalBounds().height  * 0.5f));
	ThenColorLabelText.setPosition(a_Pos + sf::Vector2f(440, 36));

	ChangeThenColorButton.SetPosition(a_Pos + sf::Vector2f(520, 50));
}

void RuleInterface::SetMinNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	MinNumNeighborsLabelText.setFont(a_Font);
	MinNumNeighborsLabelText.setCharacterSize(a_CharacterSize);
	MinNumNeighborsLabelText.setColor(a_TextColor);
	MinNumNeighborsLabelText.setStyle(sf::Text::Regular);
	MinNumNeighborsLabelText.setString(a_Words);
	//left aligned...
	MinNumNeighborsLabelText.setOrigin(sf::Vector2f(0, MinNumNeighborsLabelText.getLocalBounds().height  * 0.5f));
}

void RuleInterface::UpdateMinNumNeighborsText(int a_MinNumNeighbors)
{
	MinNumNeighborsText.setString(std::to_string(a_MinNumNeighbors));
	//MinNumNeighborsText.setString("sanity check");
}

void RuleInterface::SetMinNumNeighborsText(int a_MinNumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	MinNumNeighborsText.setFont(a_Font);
	MinNumNeighborsText.setCharacterSize(a_CharacterSize);
	MinNumNeighborsText.setColor(a_TextColor);
	MinNumNeighborsText.setStyle(sf::Text::Regular);
	UpdateMinNumNeighborsText(a_MinNumNeighbors);
	//left aligned...
	MinNumNeighborsText.setOrigin(sf::Vector2f(MinNumNeighborsText.getLocalBounds().width  * 0.5f, MinNumNeighborsText.getLocalBounds().height  * 0.5f));
}

void RuleInterface::SetMaxNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	MaxNumNeighborsLabelText.setFont(a_Font);
	MaxNumNeighborsLabelText.setCharacterSize(a_CharacterSize);
	MaxNumNeighborsLabelText.setColor(a_TextColor);
	MaxNumNeighborsLabelText.setStyle(sf::Text::Regular);
	MaxNumNeighborsLabelText.setString(a_Words);
	//left aligned...
	MaxNumNeighborsLabelText.setOrigin(sf::Vector2f(0, MaxNumNeighborsLabelText.getLocalBounds().height  * 0.5f));
}

void RuleInterface::UpdateMaxNumNeighborsText(int a_MaxNumNeighbors)
{
	MaxNumNeighborsText.setString(std::to_string(a_MaxNumNeighbors));
	//MaxNumNeighborsText.setString("sanity check");
}

void RuleInterface::SetMaxNumNeighborsText(int a_MaxNumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	MaxNumNeighborsText.setFont(a_Font);
	MaxNumNeighborsText.setCharacterSize(a_CharacterSize);
	MaxNumNeighborsText.setColor(a_TextColor);
	MaxNumNeighborsText.setStyle(sf::Text::Regular);
	UpdateMaxNumNeighborsText(a_MaxNumNeighbors);
	//left aligned...
	MaxNumNeighborsText.setOrigin(sf::Vector2f(MaxNumNeighborsText.getLocalBounds().width  * 0.5f, MaxNumNeighborsText.getLocalBounds().height  * 0.5f));
}




void RuleInterface::Update(float a_DeltaTime)
{
	IncreaseMinNumNeighborsButton.Update(a_DeltaTime);
	DecreaseMinNumNeighborsButton.Update(a_DeltaTime);
	IncreaseMaxNumNeighborsButton.Update(a_DeltaTime);
	DecreaseMaxNumNeighborsButton.Update(a_DeltaTime);
	ChangeThisColorButton.Update(a_DeltaTime);
	ChangeIfColorButton.Update(a_DeltaTime);
	ChangeThenColorButton.Update(a_DeltaTime);
	DeleteSelfButton.Update(a_DeltaTime);
}

void RuleInterface::Draw()
{
	g_WINDOW->draw(ThisColorLabelText);
	ChangeThisColorButton.Draw();

	g_WINDOW->draw(MinNumNeighborsLabelText);
	g_WINDOW->draw(MinNumNeighborsText);
	IncreaseMinNumNeighborsButton.Draw();
	DecreaseMinNumNeighborsButton.Draw();
	g_WINDOW->draw(MaxNumNeighborsLabelText);
	g_WINDOW->draw(MaxNumNeighborsText);
	IncreaseMaxNumNeighborsButton.Draw();
	DecreaseMaxNumNeighborsButton.Draw();
	g_WINDOW->draw(RuleLabelText);

	g_WINDOW->draw(IfColorLabelText);
	ChangeIfColorButton.Draw();

	g_WINDOW->draw(ThenColorLabelText);
	ChangeThenColorButton.Draw();

	DeleteSelfButton.Draw();
}