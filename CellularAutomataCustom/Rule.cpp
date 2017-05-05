#include "Rule.h"
#include "StaticNamespaces.h"
#include "Math.h"

int Rule::s_RuleCount = 0;

Rule::Rule(int a_ThisColor, int a_MinNumNeighbors, int a_MaxNumNeighbors, bool a_RdddingLevel[2], int a_IfColor, int a_ThenColor, int _containerIndex)
{
	++s_RuleCount;
	bool a_RingLevel[2] = {true, false};
	m_RuleData = new RuleData(a_ThisColor, a_MinNumNeighbors, a_MaxNumNeighbors, a_RingLevel, a_IfColor, a_ThenColor);
	m_RuleData->ThisColorIndex = a_ThisColor;
	m_RuleData->IfColorIndex = a_IfColor;
	m_RuleData->ThenColorIndex = a_ThenColor;

	m_RuleData->ThisColor = caColors::caColors[a_ThisColor];
	m_RuleData->IfColor = caColors::caColors[a_IfColor];
	m_RuleData->ThenColor = caColors::caColors[a_ThenColor];

	m_Interface = new RuleInterface();
	
	std::string r0Str = "Rule 0:";
	m_Interface->SetRuleLabelText(r0Str, *caFonts::s_DefaultFont, 16, caColors::RuleLabelTextColor);
	InitializeInterface();

	containerIndex = _containerIndex;
}

Rule::~Rule()
{
	--s_RuleCount;
}

void Rule::InitializeInterface()
{
	m_Interface->InitializeButtonsNoEvents();

	ptr_HandleIncreaseMinNumNeighborsButton = std::bind(&Rule::HandleIncreaseMinNumNeighborsButton, this);
	ptr_HandleDecreaseMinNumNeighborsButton = std::bind(&Rule::HandleDecreaseMinNumNeighborsButton, this);

	ptr_HandleIncreaseMaxNumNeighborsButton = std::bind(&Rule::HandleIncreaseMaxNumNeighborsButton, this);
	ptr_HandleDecreaseMaxNumNeighborsButton = std::bind(&Rule::HandleDecreaseMaxNumNeighborsButton, this);

	ptr_HandleAdvanceThisColorButton = std::bind(&Rule::HandleAdvanceThisColorButton, this);
	ptr_HandleReverseAdvanceThisColorButton = std::bind(&Rule::HandleReverseAdvanceThisColorButton, this);

	ptr_HandleAdvanceIfColorButton = std::bind(&Rule::HandleAdvanceIfColorButton, this);
	ptr_HandleReverseAdvanceIfColorButton = std::bind(&Rule::HandleReverseAdvanceIfColorButton, this);
	ptr_HandleAdvanceThenColorButton = std::bind(&Rule::HandleAdvanceThenColorButton, this);
	ptr_HandleReverseAdvanceThenColorButton = std::bind(&Rule::HandleReverseAdvanceThenColorButton, this);

	ptr_HandleDeleteButton = std::bind(&Rule::HandleDelete, this);

	m_Interface->IncreaseMinNumNeighborsButton.LeftMouseButtonReleaseEvent = ptr_HandleIncreaseMinNumNeighborsButton;
	m_Interface->DecreaseMinNumNeighborsButton.LeftMouseButtonReleaseEvent = ptr_HandleDecreaseMinNumNeighborsButton;

	m_Interface->IncreaseMaxNumNeighborsButton.LeftMouseButtonReleaseEvent = ptr_HandleIncreaseMaxNumNeighborsButton;
	m_Interface->DecreaseMaxNumNeighborsButton.LeftMouseButtonReleaseEvent = ptr_HandleDecreaseMaxNumNeighborsButton;

	m_Interface->ChangeThisColorButton.LeftMouseButtonReleaseEvent = ptr_HandleAdvanceThisColorButton;
	m_Interface->ChangeThisColorButton.RightMouseButtonReleaseEvent = ptr_HandleReverseAdvanceThisColorButton;

	m_Interface->ChangeIfColorButton.LeftMouseButtonReleaseEvent = ptr_HandleAdvanceIfColorButton;
	m_Interface->ChangeIfColorButton.RightMouseButtonReleaseEvent = ptr_HandleReverseAdvanceIfColorButton;
	m_Interface->ChangeThenColorButton.LeftMouseButtonReleaseEvent = ptr_HandleAdvanceThenColorButton;
	m_Interface->ChangeThenColorButton.RightMouseButtonReleaseEvent = ptr_HandleReverseAdvanceThenColorButton;

	m_Interface->DeleteSelfButton.LeftMouseButtonReleaseEvent = ptr_HandleDeleteButton;

	
	std::string ruleLabelString = "\nRule " + std::to_string(s_RuleCount) + ":\n";
	m_Interface->SetRuleLabelText(ruleLabelString, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);

	std::string nnStr = "If\nbetween";
	m_Interface->SetMinNumNeighborsLabelText(nnStr, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);
	m_Interface->SetMinNumNeighborsText(m_RuleData->MinNumNeighbors, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);

	std::string nnStr2 = "and";
	m_Interface->SetMaxNumNeighborsLabelText(nnStr2, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);
	m_Interface->SetMaxNumNeighborsText(m_RuleData->MaxNumNeighbors, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);

	const std::string concString = "\nconcerning Cells of color:\n";
	m_Interface->SetThisColorLabelText(concString, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);
	m_Interface->ChangeThisColorButton.SetText(m_RuleData->ThisColorIndex == -1 ? "ANY" : "");

	std::string ifStr = "\nneighboring\ncells are:";
	m_Interface->SetIfColorLabelText(ifStr, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);
	std::string thnStr = "\nthen this\ncell will\nbecome:\n";
	m_Interface->SetThenColorLabelText(thnStr, *(caFonts::s_DefaultFont), 22, caColors::RuleLabelTextColor);

	m_Interface->SetPosition(sf::Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 8), (float)(40 + (112 * (s_RuleCount-1)))));

	m_Interface->ChangeThisColorButton.SetFill(m_RuleData->ThisColor);
	m_Interface->ChangeIfColorButton.SetFill(m_RuleData->IfColor);
	m_Interface->ChangeThenColorButton.SetFill(m_RuleData->ThenColor);
}
void Rule::HandleIncreaseMinNumNeighborsButton()
{
	//max 8 tiles around a single tile (first layer)
	if (m_RuleData->MinNumNeighbors < 8)
	{
		m_RuleData->MinNumNeighbors++;
	}
	m_Interface->UpdateMinNumNeighborsText(m_RuleData->MinNumNeighbors);
}
void Rule::HandleDecreaseMinNumNeighborsButton()
{
	if (m_RuleData->MinNumNeighbors > 0)
	{
		m_RuleData->MinNumNeighbors--;
	}
	m_Interface->UpdateMinNumNeighborsText(m_RuleData->MinNumNeighbors);
}

void Rule::HandleIncreaseMaxNumNeighborsButton()
{
	//max 8 tiles around a single tile (first layer)
	if (m_RuleData->MaxNumNeighbors < 8)
	{
		m_RuleData->MaxNumNeighbors++;
	}
	m_Interface->UpdateMaxNumNeighborsText(m_RuleData->MaxNumNeighbors);
}
void Rule::HandleDecreaseMaxNumNeighborsButton()
{
	if (m_RuleData->MaxNumNeighbors > 0)
	{
		m_RuleData->MaxNumNeighbors--;
	}
	m_Interface->UpdateMaxNumNeighborsText(m_RuleData->MaxNumNeighbors);
}


void Rule::HandleAdvanceThisColorButton()
{
	m_RuleData->ThisColorIndex++;
	if (m_RuleData->ThisColorIndex >= caColors::caColorsLen)
	{//wrap
		m_RuleData->ThisColorIndex = -1;

		//temp: switch transparent to rainbow image.
		m_Interface->ChangeThisColorButton.SetFill(caColors::transparent);
		m_RuleData->ThisColor = caColors::transparent;
		m_Interface->ChangeThisColorButton.SetText("ANY");
	}
	else
	{
		m_Interface->ChangeThisColorButton.SetFill(caColors::caColors[m_RuleData->ThisColorIndex]);
		m_RuleData->ThisColor = caColors::caColors[m_RuleData->ThisColorIndex];
		m_Interface->ChangeThisColorButton.SetText("");
	}
}
void Rule::HandleReverseAdvanceThisColorButton()
{
	m_RuleData->ThisColorIndex--;
	if (m_RuleData->ThisColorIndex >= 0)
	{
		m_Interface->ChangeThisColorButton.SetFill(caColors::caColors[m_RuleData->ThisColorIndex]);
		m_RuleData->ThisColor = caColors::caColors[m_RuleData->ThisColorIndex];
		m_Interface->ChangeThisColorButton.SetText("");
	}
	else if (m_RuleData->ThisColorIndex < -1)
	{//wrap
		m_RuleData->ThisColorIndex = caColors::caColorsLen - 1;
		m_Interface->ChangeThisColorButton.SetFill(caColors::caColors[m_RuleData->ThisColorIndex]);
		m_RuleData->ThisColor = caColors::caColors[m_RuleData->ThisColorIndex];
		m_Interface->ChangeThisColorButton.SetText("");
	}
	else if (m_RuleData->ThisColorIndex == -1)
	{
		//temp: switch transparent to rainbow image.
		m_Interface->ChangeThisColorButton.SetFill(caColors::transparent);
		m_RuleData->ThisColor = caColors::transparent;
		m_Interface->ChangeThisColorButton.SetText("ANY");
	}
	else
	{
		printf("\ntry harder\n");
	}
}


void Rule::HandleAdvanceIfColorButton()
{
	m_RuleData->IfColorIndex++;
	if (m_RuleData->IfColorIndex >= caColors::caColorsLen)
	{//wrap
		m_RuleData->IfColorIndex = 0;
	}

	m_RuleData->IfColor = caColors::caColors[m_RuleData->IfColorIndex];
	m_Interface->ChangeIfColorButton.SetFill(m_RuleData->IfColor);
}
void Rule::HandleReverseAdvanceIfColorButton()
{
	m_RuleData->IfColorIndex--;
	if (m_RuleData->IfColorIndex < 0)
	{//wrap
		m_RuleData->IfColorIndex = caColors::caColorsLen-1;
	}

	m_RuleData->IfColor = caColors::caColors[m_RuleData->IfColorIndex];
	m_Interface->ChangeIfColorButton.SetFill(m_RuleData->IfColor);
}

void Rule::HandleAdvanceThenColorButton()
{
	m_RuleData->ThenColorIndex++;
	if (m_RuleData->ThenColorIndex >= caColors::caColorsLen)
	{//wrap
		m_RuleData->ThenColorIndex = 0;
	}

	m_RuleData->ThenColor = caColors::caColors[m_RuleData->ThenColorIndex];
	m_Interface->ChangeThenColorButton.SetFill(m_RuleData->ThenColor);
}
void Rule::HandleReverseAdvanceThenColorButton()
{
	m_RuleData->ThenColorIndex--;
	if (m_RuleData->ThenColorIndex < 0)
	{//wrap
		m_RuleData->ThenColorIndex = caColors::caColorsLen - 1;
	}

	m_RuleData->ThenColor = caColors::caColors[m_RuleData->ThenColorIndex];
	m_Interface->ChangeThenColorButton.SetFill(m_RuleData->ThenColor);
}

void Rule::HandleDelete()
{//I will never apologize for this.
	cheating::buttonIndexToRemove = containerIndex;
}


void Rule::Update(float dt)
{
	m_Interface->Update(dt);
}

void Rule::Draw()
{
	m_Interface->Draw();
}

void Rule::Refresh(int _newIndex)
{
	containerIndex = _newIndex;
	std::string ruleLabelString = "\nRule " + std::to_string(_newIndex+1) + ":\n";
	m_Interface->SetRuleLabelText(ruleLabelString, *(caFonts::s_DefaultFont), 16, caColors::RuleLabelTextColor);
}