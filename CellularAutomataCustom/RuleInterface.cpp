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
	
	IncreaseNumNeighborsButton = Button(Vector2f(32, 32), Vector2f((float)(caSizes::WINDOW_SIZE_X / 1.2f), (float)(caSizes::WINDOW_SIZE_Y - 125)), caColors::green, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "+");
	DecreaseNumNeighborsButton = Button(Vector2f(32, 32), Vector2f((float)(caSizes::WINDOW_SIZE_X / 1.2f), (float)(caSizes::WINDOW_SIZE_Y - 75)), caColors::red, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "-");

}

void RuleInterface::SetRuleLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	RuleLabelText.setFont(a_Font);
	RuleLabelText.setCharacterSize(a_CharacterSize);
	RuleLabelText.setColor(a_TextColor);
	RuleLabelText.setStyle(sf::Text::Bold);
	RuleLabelText.setString(a_Words);
}

RuleInterface::~RuleInterface()
{
}

void RuleInterface::SetPosition(sf::Vector2f a_Pos)
{
	//left aligned -- origin
	RuleLabelText.setOrigin(sf::Vector2f(0, RuleLabelText.getGlobalBounds().height / 2.0f));
	RuleLabelText.setPosition(a_Pos);


	NumNeighborsLabelText.setPosition(a_Pos + sf::Vector2f(50, 50));
	IncreaseNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(75, 25));
	NumNeighborsText.setPosition(a_Pos + sf::Vector2f(75, 50));
	DecreaseNumNeighborsButton.SetPosition(a_Pos + sf::Vector2f(75, 75));
}

void RuleInterface::SetNumNeighborsLabelText(std::string a_Words, sf::Font& a_Font, int a_CharacterSize, sf::Color a_TextColor)
{
	NumNeighborsLabelText.setFont(a_Font);
	NumNeighborsLabelText.setCharacterSize(a_CharacterSize);
	NumNeighborsLabelText.setColor(a_TextColor);
	NumNeighborsLabelText.setStyle(sf::Text::Bold);
	NumNeighborsLabelText.setString(a_Words);
	//left aligned...
	NumNeighborsLabelText.setOrigin(sf::Vector2f(0, NumNeighborsLabelText.getGlobalBounds().height / 2.0f));
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
	NumNeighborsText.setStyle(sf::Text::Bold);
	UpdateNumNeighborsText(a_NumNeighbors);
	//left aligned...
	NumNeighborsText.setOrigin(sf::Vector2f(NumNeighborsText.getGlobalBounds().width / 2.0f, NumNeighborsText.getGlobalBounds().height / 2.0f));
}




void RuleInterface::Update(float a_DeltaTime)
{
	IncreaseNumNeighborsButton.Update(a_DeltaTime);
	DecreaseNumNeighborsButton.Update(a_DeltaTime);
}

void RuleInterface::Draw()
{
	g_WINDOW->draw(NumNeighborsLabelText);
	g_WINDOW->draw(NumNeighborsText);
	IncreaseNumNeighborsButton.Draw();
	DecreaseNumNeighborsButton.Draw();
	g_WINDOW->draw(RuleLabelText);
}