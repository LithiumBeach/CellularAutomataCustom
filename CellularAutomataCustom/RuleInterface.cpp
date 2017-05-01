#include "RuleInterface.h"
#include "StaticNamespaces.h"

extern sf::RenderWindow* g_WINDOW;
 
RuleInterface::RuleInterface()
{
	IncreaseNumNeighborsButton = Button();
	DecreaseNumNeighborsButton = Button();
	InitializeButtonsNoEvents();
}

void RuleInterface::InitializeButtonsNoEvents()
{
	int inc_dec_button_size = 16;
	IncreaseNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "+", 22);
	DecreaseNumNeighborsButton = Button(Vector2f(inc_dec_button_size, inc_dec_button_size), Vector2f(0, 0), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "-", 22);

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
	NumNeighborsLabelText.setOrigin(sf::Vector2f(0, NumNeighborsLabelText.getLocalBounds().height / 2.0f));
	NumNeighborsLabelText.setPosition(a_Pos + sf::Vector2f(25, 44));

	IncreaseNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(55, 25));

	NumNeighborsText.setOrigin(sf::Vector2f(NumNeighborsText.getLocalBounds().width / 2.0f, NumNeighborsText.getLocalBounds().height / 2.0f));
	NumNeighborsText.setPosition(a_Pos + sf::Vector2f(55, 44));

	DecreaseNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(55, 75));

	ThisColorLabelText.setOrigin(sf::Vector2f(ThisColorLabelText.getLocalBounds().width / 3.0f, ThisColorLabelText.getLocalBounds().height / 2.0f));
	ThisColorLabelText.setPosition(a_Pos + sf::Vector2f(120, -19));

	ChangeThisColorButton.SetPosition(a_Pos + sf::Vector2f(254, -9));

	IfColorLabelText.setOrigin(sf::Vector2f(IfColorLabelText.getLocalBounds().width / 2.0f, IfColorLabelText.getLocalBounds().height / 2.0f));
	IfColorLabelText.setPosition(a_Pos + sf::Vector2f(130, 22));

	ChangeIfColorButton.SetPosition(a_Pos + sf::Vector2f(230, 50));

	ThenColorLabelText.setOrigin(sf::Vector2f(ThenColorLabelText.getLocalBounds().width / 2.0f, ThenColorLabelText.getLocalBounds().height / 2.0f));
	ThenColorLabelText.setPosition(a_Pos + sf::Vector2f(330, 36));

	ChangeThenColorButton.SetPosition(a_Pos + sf::Vector2f(430, 50));
}

void RuleInterface::SetNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	NumNeighborsLabelText.setFont(a_Font);
	NumNeighborsLabelText.setCharacterSize(a_CharacterSize);
	NumNeighborsLabelText.setColor(a_TextColor);
	NumNeighborsLabelText.setStyle(sf::Text::Regular);
	NumNeighborsLabelText.setString(a_Words);
	//left aligned...
	NumNeighborsLabelText.setOrigin(sf::Vector2f(0, NumNeighborsLabelText.getLocalBounds().height / 2.0f));
}

void RuleInterface::UpdateNumNeighborsText(int a_NumNeighbors)
{
	NumNeighborsText.setString(std::to_string(a_NumNeighbors));
	//NumNeighborsText.setString("sanity check");
}

void RuleInterface::SetNumNeighborsText(int a_NumNeighbors, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	NumNeighborsText.setFont(a_Font);
	NumNeighborsText.setCharacterSize(a_CharacterSize);
	NumNeighborsText.setColor(a_TextColor);
	NumNeighborsText.setStyle(sf::Text::Regular);
	UpdateNumNeighborsText(a_NumNeighbors);
	//left aligned...
	NumNeighborsText.setOrigin(sf::Vector2f(NumNeighborsText.getLocalBounds().width / 2.0f, NumNeighborsText.getLocalBounds().height / 2.0f));
}




void RuleInterface::Update(float a_DeltaTime)
{
	IncreaseNumNeighborsButton.Update(a_DeltaTime);
	DecreaseNumNeighborsButton.Update(a_DeltaTime);
	ChangeThisColorButton.Update(a_DeltaTime);
	ChangeIfColorButton.Update(a_DeltaTime);
	ChangeThenColorButton.Update(a_DeltaTime);
}

void RuleInterface::Draw()
{
	g_WINDOW->draw(ThisColorLabelText);
	ChangeThisColorButton.Draw();

	g_WINDOW->draw(NumNeighborsLabelText);
	g_WINDOW->draw(NumNeighborsText);
	IncreaseNumNeighborsButton.Draw();
	DecreaseNumNeighborsButton.Draw();
	g_WINDOW->draw(RuleLabelText);

	g_WINDOW->draw(IfColorLabelText);
	ChangeIfColorButton.Draw();

	g_WINDOW->draw(ThenColorLabelText);
	ChangeThenColorButton.Draw();
}