#include "Rule.h"
#include "StaticNamespaces.h"
#include "Math.h"

int Rule::s_RuleCount = 0;

Rule::Rule(int a_NumNeighbors, bool a_RdddingLevel[2], sf::Color a_IfColor, sf::Color a_ThenColor)
{
	++s_RuleCount;
	bool a_RingLevel[2] = {true, false};
	m_RuleData = new RuleData(a_NumNeighbors, a_RingLevel, a_IfColor, a_ThenColor);
	m_RuleData->IfColorIndex = 0;
	m_RuleData->ThenColorIndex = 0;

	m_Interface = new RuleInterface();
	
	m_Interface->SetRuleLabelText("Rule 0:", *caFonts::s_DefaultFont, 16, caColors::RuleLabelTextColor);
	InitializeInterface();

	//ptr_HandleIncreaseNumNeighborsButton = std::function<void()>();
	//ptr_HandleDecreaseNumNeighborsButton = std::function<void()>();
}

Rule::~Rule()
{
	--s_RuleCount;
}

void Rule::InitializeInterface()
{
	m_Interface->InitializeButtonsNoEvents();

	ptr_HandleIncreaseNumNeighborsButton = std::bind(&Rule::HandleIncreaseNumNeighborsButton, this);
	ptr_HandleDecreaseNumNeighborsButton = std::bind(&Rule::HandleDecreaseNumNeighborsButton, this);

	ptr_HandleAdvanceIfColorButton = std::bind(&Rule::HandleAdvanceIfColorButton, this);
	ptr_HandleReverseAdvanceIfColorButton = std::bind(&Rule::HandleReverseAdvanceIfColorButton, this);
	ptr_HandleAdvanceThenColorButton = std::bind(&Rule::HandleAdvanceThenColorButton, this);
	ptr_HandleReverseAdvanceThenColorButton = std::bind(&Rule::HandleReverseAdvanceThenColorButton, this);

	m_Interface->IncreaseNumNeighborsButton.LeftMouseButtonPressEvent = ptr_HandleIncreaseNumNeighborsButton;
	m_Interface->DecreaseNumNeighborsButton.LeftMouseButtonPressEvent = ptr_HandleDecreaseNumNeighborsButton;

	m_Interface->ChangeIfColorButton.LeftMouseButtonPressEvent = ptr_HandleAdvanceIfColorButton;
	m_Interface->ChangeIfColorButton.RightMouseButtonPressEvent = ptr_HandleReverseAdvanceIfColorButton;
	m_Interface->ChangeThenColorButton.LeftMouseButtonPressEvent = ptr_HandleAdvanceThenColorButton;
	m_Interface->ChangeThenColorButton.RightMouseButtonPressEvent = ptr_HandleReverseAdvanceThenColorButton;


	std::string ruleLabelString = "Rule " + std::to_string(s_RuleCount) + ":";
	m_Interface->SetRuleLabelText(ruleLabelString, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);
	m_Interface->SetNumNeighborsLabelText("If ", *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);
	m_Interface->SetNumNeighborsText(m_RuleData->NumNeighbors, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);


	m_Interface->SetIfColorLabelText("neighboring\ncells are:", *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);
	m_Interface->SetThenColorLabelText("then this\ncell will\nbecome:", *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);

	m_Interface->SetPosition(sf::Vector2f((float)(caSizes::WINDOW_SIZE_X * 0.01f), (float)(caSizes::WINDOW_SIZE_Y - 40)));
}
void Rule::HandleIncreaseNumNeighborsButton()
{
	//max 8 tiles around a single tile (first layer)
	if (m_RuleData->NumNeighbors < 8)
	{
		m_RuleData->NumNeighbors++;
	}
	m_Interface->UpdateNumNeighborsText(m_RuleData->NumNeighbors);
}
void Rule::HandleDecreaseNumNeighborsButton()
{
	if (m_RuleData->NumNeighbors > 0)
	{
		m_RuleData->NumNeighbors--;
	}
	m_Interface->UpdateNumNeighborsText(m_RuleData->NumNeighbors);
}


void Rule::HandleAdvanceIfColorButton()
{
	m_RuleData->IfColorIndex++;
	if (m_RuleData->IfColorIndex >= caColors::caColorsLen)
	{//wrap
		m_RuleData->IfColorIndex = 0;
	}

	m_Interface->ChangeIfColorButton.SetFill(caColors::caColors[m_RuleData->IfColorIndex]);
	m_RuleData->IfColor = caColors::caColors[m_RuleData->IfColorIndex];
}
void Rule::HandleReverseAdvanceIfColorButton()
{
	m_RuleData->IfColorIndex--;
	if (m_RuleData->IfColorIndex < 0)
	{//wrap
		m_RuleData->IfColorIndex = caColors::caColorsLen-1;
	}

	m_Interface->ChangeIfColorButton.SetFill(caColors::caColors[m_RuleData->IfColorIndex]);
	m_RuleData->IfColor = caColors::caColors[m_RuleData->IfColorIndex];
}

void Rule::HandleAdvanceThenColorButton()
{
	m_RuleData->ThenColorIndex++;
	if (m_RuleData->ThenColorIndex >= caColors::caColorsLen)
	{//wrap
		m_RuleData->ThenColorIndex = 0;
	}

	m_Interface->ChangeThenColorButton.SetFill(caColors::caColors[m_RuleData->ThenColorIndex]);
	m_RuleData->ThenColor = caColors::caColors[m_RuleData->ThenColorIndex];
}
void Rule::HandleReverseAdvanceThenColorButton()
{
	m_RuleData->ThenColorIndex--;
	if (m_RuleData->ThenColorIndex < 0)
	{//wrap
		m_RuleData->ThenColorIndex = caColors::caColorsLen - 1;
	}

	m_Interface->ChangeThenColorButton.SetFill(caColors::caColors[m_RuleData->ThenColorIndex]);
	m_RuleData->ThenColor = caColors::caColors[m_RuleData->ThenColorIndex];
}


void Rule::Update(float dt)
{
	m_Interface->Update(dt);
}

void Rule::Draw()
{
	m_Interface->Draw();
}