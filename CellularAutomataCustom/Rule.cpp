#include "Rule.h"
#include "StaticNamespaces.h"

int Rule::s_RuleCount = 0;

Rule::Rule(int a_NumNeighbors, bool a_RdddingLevel[2], sf::Color a_IfColor, sf::Color a_ThenColor)
{
	++s_RuleCount;
	bool a_RingLevel[2] = {true, false};
	m_RuleData = new RuleData(a_NumNeighbors, a_RingLevel, a_IfColor, a_ThenColor);
	m_Interface = new RuleInterface();
	
	m_Interface->SetRuleLabelText("Rule 0:", *caFonts::s_DefaultFont, 16, caColors::RuleLabelTextColor);
	InitializeInterface();

	ptr_HandleIncreaseNumNeighborsButton = std::function<void()>();
	ptr_HandleDecreaseNumNeighborsButton = std::function<void()>();
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

	m_Interface->IncreaseNumNeighborsButton.ButtonPressEvent = ptr_HandleIncreaseNumNeighborsButton;
	m_Interface->DecreaseNumNeighborsButton.ButtonPressEvent = ptr_HandleDecreaseNumNeighborsButton;



	m_Interface->SetRuleLabelText("Rule " + s_RuleCount, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);
	m_Interface->SetNumNeighborsLabelText("If ", *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);
	m_Interface->SetNumNeighborsText(m_RuleData->NumNeighbors, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);

	m_Interface->SetPosition(sf::Vector2f((float)(caSizes::WINDOW_SIZE_X / 64), (float)(caSizes::WINDOW_SIZE_Y - 40)));
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

void Rule::Update(float dt)
{
	m_Interface->Update(dt);
}

void Rule::Draw()
{
	m_Interface->Draw();
}