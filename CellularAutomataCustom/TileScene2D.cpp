#include "TileScene2D.h"
#include "StaticNamespaces.h"
#include "RuleSerializer.h"

#include <sstream>

extern sf::RenderWindow* g_WINDOW;

TileScene2D::TileScene2D()
{
	m_IsSimulating = false;
	m_IncreasePlaybackSpeedButton = Button();
	m_DecreasePlaybackSpeedButton = Button();
	m_ControlsBG = Button();

	m_PlaybackSpeedsLen = 12;
	playbackSpeedsIndex = 5;
	m_PlaybackSpeeds = new float[m_PlaybackSpeedsLen]{ 1.0f, 2.0f, 4.0f, 8.0f, 10.0f, 12.0f, 16.0f, 24.0f, 32.0f, 64.0f, 96.0f, 128.0f };

	BoardSizeIndexNextFrame = BoardSizeIndex = 3;
	BoardSizeLen = 6;

	//Resolution arrays
	TilePixelSizes = new float[BoardSizeLen]
	{
		100,
		20,
		10,
		5,
		4,
		2
	};
	BoardTileSizes = new int[BoardSizeLen]
	{
		5,
		25,
		50,
		100,
		125,
		250
	};


	FramesPerSecond = 1.0f / m_PlaybackSpeeds[playbackSpeedsIndex];

	ruleSerializer::currentLoadedRuleIndex = 0;

	m_Cells = std::vector<std::vector<Cell>>();
	m_Cells.resize(BoardTileSizes[BoardSizeIndex]);
	for (int i = 0; i < BoardTileSizes[BoardSizeIndex]; i++)
	{
		m_Cells[i].resize(BoardTileSizes[BoardSizeIndex]);
	}

	for (int y = 0; y < BoardTileSizes[BoardSizeIndex]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndex]; x++)
		{
			m_Cells[x][y] = Cell(0, Vector2f((float)x, (float)y), Vector2f(TilePixelSizes[BoardSizeIndex], TilePixelSizes[BoardSizeIndex]));
			//printf("%d,%d|", (int)m_Cells[x][y].GetTilePos().x, (int)m_Cells[x][y].GetTilePos().y);//works :)
		}
		//printf("\n");
	}

	//r-pentomino
	//m_Cells[11][10].SetAliveImmediate(true);//works :)
	//m_Cells[12][11].SetAliveImmediate(true);//works :)
	//m_Cells[10][12].SetAliveImmediate(true);//works :)
	//m_Cells[11][12].SetAliveImmediate(true);//works :)
	//m_Cells[12][12].SetAliveImmediate(true);//works :)


	//reverse r-pentomino
	m_Cells[11][10].SetColorIndexImmediate(1);//works :)
	m_Cells[10][11].SetColorIndexImmediate(1);//works :)
	m_Cells[12][12].SetColorIndexImmediate(1);//works :)
	m_Cells[11][12].SetColorIndexImmediate(1);//works :)
	m_Cells[10][12].SetColorIndexImmediate(1);//works :)

	InitializeUI();


	IntializeRules();
}

TileScene2D::~TileScene2D()
{
	delete TilePixelSizes;
	TilePixelSizes = NULL;
	delete BoardTileSizes;
	BoardTileSizes = NULL;
	delete m_PlaybackSpeeds;
	m_PlaybackSpeeds = NULL;

	DeleteRules();

	delete m_RuleScrollBar;
	m_RuleScrollBar = NULL;
}
void TileScene2D::DeleteRules()
{
	if (m_Rules != NULL)
	{
		for (size_t i = m_Rules->size() - 1; i > 0; i--)
		{
			//this is a random bug that makes absolutely zero sense, but sometimes it forgets what i was and like nulls it for no reason.
			if (i < m_Rules->size() && i >= 0)
			{
				delete m_Rules->at(i);
				m_Rules->erase(m_Rules->begin() + i);
			}
			else
			{
				break;
			}
		}
		m_Rules->clear();
		delete m_Rules;
		m_Rules = NULL;
	}
}

void TileScene2D::ResizeBoard()
{
	//ffuuuuuuuuuuuuuUUUUUUUUUUUUUUUUUck
	m_Cells.resize(BoardTileSizes[BoardSizeIndexNextFrame]);
	for (int i = 0; i < BoardTileSizes[BoardSizeIndexNextFrame]; i++)
	{
		m_Cells[i].resize(BoardTileSizes[BoardSizeIndexNextFrame]);
	}

	for (int y = 0; y < BoardTileSizes[BoardSizeIndexNextFrame]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndexNextFrame]; x++)
		{
			if (x < BoardTileSizes[BoardSizeIndex] && y < BoardTileSizes[BoardSizeIndex])
			{
				m_Cells[x][y] = Cell(m_Cells[x][y].GetColorIndex(), Vector2f((float)x, (float)y), Vector2f(TilePixelSizes[BoardSizeIndexNextFrame], TilePixelSizes[BoardSizeIndexNextFrame]));
			}
			else
			{
				m_Cells[x][y] = Cell(0, Vector2f((float)x, (float)y), Vector2f(TilePixelSizes[BoardSizeIndexNextFrame], TilePixelSizes[BoardSizeIndexNextFrame]));
			}
			//m_Cells[x][y] = Cell(m_ClearColorIndex, Vector2f((float)x, (float)y), Vector2f(TilePixelSizes[BoardSizeIndexNextFrame], TilePixelSizes[BoardSizeIndexNextFrame]));
			//printf("%d,%d|", (int)m_Cells[x][y].GetTilePos().x, (int)m_Cells[x][y].GetTilePos().y);//works :)
		}
		//printf("\n");
	}

	BoardSizeIndex = BoardSizeIndexNextFrame;
}

void TileScene2D::InitializeUI()
{
	HandleSimulateButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandleSimulateButtonReleaseEvent, this);
	HandlePlaybackSpeedIncButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedIncButtonReleaseEvent, this);
	HandlePlaybackSpeedDecButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedDecButtonReleaseEvent, this);
	HandleNextFrameButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandleNextFrameButtonReleaseEvent, this);

	HandleClearEvent_ptr = std::bind(&TileScene2D::HandleClearEvent, this);
	HandleClearColorIncrementEvent_ptr = std::bind(&TileScene2D::HandleClearColorIncrementEvent, this);
	HandleClearColorDecrementEvent_ptr = std::bind(&TileScene2D::HandleClearColorDecrementEvent, this);

	HandleAddRuleEvent_ptr = std::bind(&TileScene2D::HandleAddRuleEvent, this);

	HandleIncreaseResButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandleIncreaseResButtonReleaseEvent, this);
	HandleDecreaseResButtonReleaseEvent_ptr = std::bind(&TileScene2D::HandleDecreaseResButtonReleaseEvent, this);

	HandleLoadPreviousRuleset_ptr = std::bind(&TileScene2D::HandleLoadPreviousRuleset, this);
	HandleLoadNextRuleset_ptr = std::bind(&TileScene2D::HandleLoadNextRuleset, this);

	HandleSaveRuleset_ptr = std::bind(&TileScene2D::HandleSaveRuleset, this);
	HandleNewRuleset_ptr = std::bind(&TileScene2D::HandleNewRuleset, this);
	HandleDeleteRuleset_ptr = std::bind(&TileScene2D::HandleDeleteRuleset, this);

	DoNothing_ptr = std::bind(&TileScene2D::DoNothing, this);

	m_SimulateButton = Button(Vector2f(184, 20), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X * 0.5f), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 100)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "SIMULATE");
	m_SimulateButton.SetLeftMouseButtonReleaseEvent(HandleSimulateButtonReleaseEvent_ptr);

	m_IncreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 4), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS+");
	m_IncreasePlaybackSpeedButton.SetLeftMouseButtonReleaseEvent(HandlePlaybackSpeedIncButtonReleaseEvent_ptr);

	m_DecreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 4), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 75)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS-");
	m_DecreasePlaybackSpeedButton.SetLeftMouseButtonReleaseEvent(HandlePlaybackSpeedDecButtonReleaseEvent_ptr);

	m_FPSButton = Button(Vector2f(0, 0), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 4), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 100)), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), *caFonts::s_DefaultFont, 2.0f, Math::to_string_with_precision(1.0f / FramesPerSecond));
	
	m_ClearButton = Button(Vector2f(86, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 10), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 100)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "Clear Board");
	m_ClearButton.SetLeftMouseButtonReleaseEvent(HandleClearEvent_ptr);
	m_ClearColorIndex = 0;
	m_ClearColorButton = Button(Vector2f(24, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 10), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 132)), caColors::caColors[m_ClearColorIndex], caColors::border_gray, *caFonts::s_DefaultFont);
	m_ClearColorButton.SetLeftMouseButtonReleaseEvent(HandleClearColorIncrementEvent_ptr);
	m_ClearColorButton.SetRightMouseButtonReleaseEvent(HandleClearColorDecrementEvent_ptr);

	m_NextFrameButton = Button(Vector2f(64, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X / 1.3f), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "Frame+");
	m_NextFrameButton.SetLeftMouseButtonReleaseEvent(HandleNextFrameButtonReleaseEvent_ptr);

	m_AddRuleButton = Button(Vector2f(80, 20), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X * 0.5f), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 130)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "ADD RULE");
	m_AddRuleButton.SetLeftMouseButtonReleaseEvent(HandleAddRuleEvent_ptr);


	//Resolution +/-
	m_IncreaseResButton = Button(Vector2f(24, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X * 0.5f + 80), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 70)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "+", 25);
	m_IncreaseResButton.SetLeftMouseButtonReleaseEvent(HandleIncreaseResButtonReleaseEvent_ptr);
	m_DecreaseResButton = Button(Vector2f(24, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X * 0.5f - 80), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 70)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "-", 35);
	m_DecreaseResButton.SetLeftMouseButtonReleaseEvent(HandleDecreaseResButtonReleaseEvent_ptr);
	//text
	m_ResText = sf::Text("\nresolution: 0px\n", *caFonts::s_DefaultFont, 13);

	std::stringstream ss;
	ss << "\nresolution: " << BoardTileSizes[BoardSizeIndexNextFrame] << "px²";
	m_ResText.setString(ss.str());

	m_ResText.setOrigin(sf::Vector2f(m_ResText.getLocalBounds().width * 0.5f, m_ResText.getLocalBounds().height *0.5f));
	m_ResText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X * 0.5f), (float)(caSizes::LEFT_WINDOW_SIZE_Y - 80));
	

	//scroll bar
	float scrollbarX = (float)(caSizes::WINDOW_SIZE_X * 0.98f);
	m_RuleScrollBar = new ScrollBar(sf::Vector2f(scrollbarX, (float)(caSizes::WINDOW_SIZE_Y * 0.15f)),
									sf::Vector2f(scrollbarX, (float)(caSizes::WINDOW_SIZE_Y * 0.95f)),
									13,
									(float)(caSizes::WINDOW_SIZE_Y * .8f)
									);

	m_ControlsBG = Button(sf::Vector2f((float)caSizes::LEFT_WINDOW_SIZE_X, 196), Vector2f((float)caSizes::LEFT_WINDOW_SIZE_X * 0.5f, (float)(caSizes::LEFT_WINDOW_SIZE_Y - 160)), caColors::g_BackBuffer_Color, caColors::g_BackBuffer_Color, *caFonts::s_DefaultFont, 0);

	rulesetBG = sf::RectangleShape(sf::Vector2f((float)(caSizes::WINDOW_SIZE_X - caSizes::LEFT_WINDOW_SIZE_X), 78.0f));
	rulesetBG.setFillColor(caColors::g_BackBuffer_Color);
	rulesetBG.setOutlineThickness(0.0f);
	rulesetBG.setPosition(sf::Vector2f((float)caSizes::LEFT_WINDOW_SIZE_X, 0));


	//ruleset management buttons
	m_LoadNextRulesetButton = Button(Vector2f(120, 22), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 80), 20.0f),
									 caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "LOAD NEXT", 14);
	m_LoadNextRulesetButton.SetLeftMouseButtonReleaseEvent(HandleLoadNextRuleset_ptr);
	
	m_LoadPreviousRulesetButton = Button(Vector2f(120, 22), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 80), 54.0f),
										 caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "LOAD PREVIOUS", 14);
	m_LoadPreviousRulesetButton.SetLeftMouseButtonReleaseEvent(HandleLoadPreviousRuleset_ptr);

	
	m_SaveRulesetButton = Button(Vector2f(180, 24), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 55.0f),
										 caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "SAVE RULESET", 16);
	m_SaveRulesetButton.SetLeftMouseButtonReleaseEvent(HandleSaveRuleset_ptr);

	m_NewRulesetButton = Button(Vector2f(120, 22), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 520), 20.0f),
										 caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "NEW RULESET", 12);
	m_NewRulesetButton.SetLeftMouseButtonReleaseEvent(HandleNewRuleset_ptr);

	m_DeleteRulesetButton = Button(Vector2f(120, 22), Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 520), 54.0f),
										 caColors::red, caColors::border_red, *caFonts::s_DefaultFont, 2.0f, "DELETE RULESET", 12);
	m_DeleteRulesetButton.SetLeftMouseButtonReleaseEvent(HandleDeleteRuleset_ptr);
	
	//ruleset name text
	m_RulesetNameText = sf::Text("", *caFonts::s_DefaultFont, 24);
	m_RulesetNameText.setString(ruleSerializer::rulesetNames[0]);

	m_RulesetNameText.setOrigin(sf::Vector2f(m_RulesetNameText.getLocalBounds().width * 0.5f, m_RulesetNameText.getLocalBounds().height *0.5f));
	m_RulesetNameText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 18.0f);
	
}

void TileScene2D::IntializeRules(int a_RulesetIndex)
{
	m_Rules = new std::vector<Rule*>();

	//at this point, the serializer has already initialized the vector of ruledatas.
	//read this vector and create rulesets accordingly.
	for (size_t i = 0; i < ruleSerializer::rulesets[a_RulesetIndex].size(); i++)
	{
		RuleData tmp = ruleSerializer::rulesets[a_RulesetIndex].at(i);
		m_Rules->push_back(new Rule(tmp.ThisColorIndex, tmp.MinNumNeighbors, tmp.MaxNumNeighbors, new bool[2]{true, false}, tmp.IfColorIndex, tmp.ThenColorIndex, i));
	}
}
//sf::Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 8), (float)(100 + (112 * (_newIndex))))
void TileScene2D::HandleAddRuleEvent()
{
	int index = m_Rules->size();
	m_Rules->push_back(new Rule(-1, 2, 2, new bool[2]{true, false}, 0, 0, index));
	m_RuleScrollBar->UpdateTargetSize((float)(100 + (112 * (Rule::s_RuleCount - 1))));
	UpdateRuleScrolling();
}

void TileScene2D::HandleRemoveRuleEvent(unsigned int index)
{
	if (index >= 0 && index < m_Rules->size())
	{
		delete m_Rules->at(index);
		m_Rules->erase(m_Rules->begin() + index);
	}

	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Refresh(i);
	}
	m_RuleScrollBar->UpdateTargetSize((float)(100 + (112 * (Rule::s_RuleCount - 1))));

	UpdateRuleScrolling();
}

void TileScene2D::DoNothing(){}

void TileScene2D::HandleSimulateButtonReleaseEvent()
{
	m_IsSimulating = !(m_IsSimulating);
}

void TileScene2D::HandlePlaybackSpeedIncButtonReleaseEvent()
{
	playbackSpeedsIndex++;
	playbackSpeedsIndex = Math::Min(playbackSpeedsIndex, m_PlaybackSpeedsLen - 1);
	FramesPerSecond = 1.0f / m_PlaybackSpeeds[playbackSpeedsIndex];
	UpdateFPSCounter();
}
void TileScene2D::HandlePlaybackSpeedDecButtonReleaseEvent()
{
	playbackSpeedsIndex--;
	playbackSpeedsIndex = Math::Max(playbackSpeedsIndex, 0);
	FramesPerSecond = 1.0f / m_PlaybackSpeeds[playbackSpeedsIndex];
	UpdateFPSCounter();
}

void TileScene2D::UpdateFPSCounter()
{
	m_FPSButton.SetText(Math::to_string_with_precision(1.0f / FramesPerSecond));
}

void TileScene2D::HandleClearEvent()
{
	for (int y = 0; y < BoardTileSizes[BoardSizeIndex]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndex]; x++)
		{
			m_Cells[x][y].SetColorIndexImmediate(m_ClearColorIndex);
		}
	}
}

void TileScene2D::HandleClearColorIncrementEvent()
{
	m_ClearColorIndex++;
	if (m_ClearColorIndex >= caColors::caColorsLen)
	{
		m_ClearColorIndex = 0;
	}
	m_ClearColorButton.SetFill(caColors::caColors[m_ClearColorIndex]);
}
void TileScene2D::HandleClearColorDecrementEvent()
{
	m_ClearColorIndex--;
	if (m_ClearColorIndex < 0)
	{
		m_ClearColorIndex = caColors::caColorsLen - 1;
	}
	m_ClearColorButton.SetFill(caColors::caColors[m_ClearColorIndex]);
}

void TileScene2D::HandleNextFrameButtonReleaseEvent()
{
	PreUpdateUnmanaged();
	UpdateUnmanaged(0.0f);
}

void TileScene2D::HandleIncreaseResButtonReleaseEvent()
{
	if (BoardSizeIndexNextFrame < BoardSizeLen-1)
	{
		BoardSizeIndexNextFrame++;
		ResizeBoard();

		std::stringstream ss;
		ss << "\nresolution: " << BoardTileSizes[BoardSizeIndexNextFrame] << "px²";
		m_ResText.setString(ss.str());
	}
}
void TileScene2D::HandleDecreaseResButtonReleaseEvent()
{
	if (BoardSizeIndexNextFrame > 0)
	{
		BoardSizeIndexNextFrame--;
		ResizeBoard();

		std::stringstream ss;
		ss << "\nresolution: " << BoardTileSizes[BoardSizeIndexNextFrame] << "px²";
		m_ResText.setString(ss.str());
	}
}

void TileScene2D::HandleLoadPreviousRuleset()
{
	if (ruleSerializer::numRulesets <= 0)
	{
		return;
	}
	HandleSaveRuleset();
	DeleteRules();
	Rule::s_RuleCount = 0;

	ruleSerializer::currentLoadedRuleIndex--;
	if (ruleSerializer::currentLoadedRuleIndex < 0)
	{
		ruleSerializer::currentLoadedRuleIndex = ruleSerializer::numRulesets - 1;
	}

	IntializeRules(ruleSerializer::currentLoadedRuleIndex);

	m_RulesetNameText.setString(ruleSerializer::rulesetNames[ruleSerializer::currentLoadedRuleIndex]);
	m_RulesetNameText.setOrigin(sf::Vector2f(m_RulesetNameText.getLocalBounds().width * 0.5f, m_RulesetNameText.getLocalBounds().height *0.5f));
	m_RulesetNameText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 18.0f);
}
void TileScene2D::HandleLoadNextRuleset()
{
	if (ruleSerializer::numRulesets <= 0)
	{
		return;
	}
	HandleSaveRuleset();
	DeleteRules();
	Rule::s_RuleCount = 0;

	ruleSerializer::currentLoadedRuleIndex++;
	if (ruleSerializer::currentLoadedRuleIndex > (int)ruleSerializer::numRulesets-1)
	{
		ruleSerializer::currentLoadedRuleIndex = 0;
	}

	IntializeRules(ruleSerializer::currentLoadedRuleIndex);
	m_RulesetNameText.setString(ruleSerializer::rulesetNames[ruleSerializer::currentLoadedRuleIndex]);
	m_RulesetNameText.setOrigin(sf::Vector2f(m_RulesetNameText.getLocalBounds().width * 0.5f, m_RulesetNameText.getLocalBounds().height *0.5f));
	m_RulesetNameText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 18.0f);
}
void TileScene2D::HandleSaveRuleset()
{
	printf("save");

	std::vector<RuleData> rds;

	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		rds.push_back(*(m_Rules->at(i)->m_RuleData));
	}
	
	ruleSerializer::SaveToFile(ruleSerializer::currentLoadedRuleIndex, rds);

	ruleSerializer::rulesets[ruleSerializer::currentLoadedRuleIndex] = rds;
}
void TileScene2D::HandleNewRuleset()
{
	if (ruleSerializer::numRulesets >= ruleSerializer::rulesetsMaxLen-1)
	{
		return;
	}
	HandleSaveRuleset();

	ruleSerializer::SaveToFile(ruleSerializer::numRulesets, std::vector<RuleData>());

	DeleteRules();
	Rule::s_RuleCount = 0;
	ruleSerializer::currentLoadedRuleIndex = ruleSerializer::numRulesets - 1;
	IntializeRules(ruleSerializer::currentLoadedRuleIndex);
	m_RulesetNameText.setString(ruleSerializer::rulesetNames[ruleSerializer::currentLoadedRuleIndex]);
	m_RulesetNameText.setOrigin(sf::Vector2f(m_RulesetNameText.getLocalBounds().width * 0.5f, m_RulesetNameText.getLocalBounds().height *0.5f));
	m_RulesetNameText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 18.0f);
}
void TileScene2D::HandleDeleteRuleset()
{
	//simply never delete the last ruleset..
	if (ruleSerializer::numRulesets <= 1)
	{
		return;
	}

	ruleSerializer::DeleteFile(ruleSerializer::currentLoadedRuleIndex);
	
	//if (ruleSerializer::currentLoadedRuleIndex >= ruleSerializer::numRulesets)
	//{
	//	ruleSerializer::currentLoadedRuleIndex = ruleSerializer::numRulesets-1;
	//}
	HandleLoadPreviousRuleset();
	
	m_RulesetNameText.setString(ruleSerializer::rulesetNames[ruleSerializer::currentLoadedRuleIndex]);
	m_RulesetNameText.setOrigin(sf::Vector2f(m_RulesetNameText.getLocalBounds().width * 0.5f, m_RulesetNameText.getLocalBounds().height *0.5f));
	m_RulesetNameText.setPosition((float)(caSizes::LEFT_WINDOW_SIZE_X + 295), 18.0f);
}

float fpsCountPre = 0.0f;
int numWaitFramesPre = 0;
void TileScene2D::PreUpdate(float a_DeltaTime)
{
	//cheat event listener...
	if (cheating::buttonIndexToRemove != -1)
	{
		HandleRemoveRuleEvent(cheating::buttonIndexToRemove);
		cheating::buttonIndexToRemove = -1;
	}
	//...don't judge me -- it's the effect of a messaging system in a fraction of the time.
	//(not proud its on an update but what is pride?)

	if (m_IsSimulating)
	{
		fpsCountPre += a_DeltaTime;

		if (fpsCountPre > FramesPerSecond)
		{
			PreUpdateUnmanaged();

			while (fpsCountPre > FramesPerSecond)
			{
				fpsCountPre -= FramesPerSecond;
			}
			numWaitFramesPre = 0;
		}
		else
		{
			numWaitFramesPre++;
		}
	}

	//call Update from here...
	Update(a_DeltaTime);

	//BUTTON UPDATE

	m_SimulateButton.Update(a_DeltaTime);
	m_IncreaseResButton.Update(a_DeltaTime);
	m_DecreaseResButton.Update(a_DeltaTime);
	m_AddRuleButton.Update(a_DeltaTime);
	m_IncreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_DecreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_FPSButton.Update(a_DeltaTime);
	m_NextFrameButton.Update(a_DeltaTime);
	m_ClearColorButton.Update(a_DeltaTime);
	m_ClearButton.Update(a_DeltaTime);
	m_RuleScrollBar->Update(a_DeltaTime);
	m_ControlsBG.Update(a_DeltaTime);
	m_LoadPreviousRulesetButton.Update(a_DeltaTime);
	m_LoadNextRulesetButton.Update(a_DeltaTime);
	m_SaveRulesetButton.Update(a_DeltaTime);
	m_NewRulesetButton.Update(a_DeltaTime);
	m_DeleteRulesetButton.Update(a_DeltaTime);

	if (m_RuleScrollBar->m_IsScrolling || abs(input::MouseWheelDelta) > 0)
	{
		UpdateRuleScrolling();
	}


	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Update(a_DeltaTime);
	}
}

void TileScene2D::UpdateRuleScrolling()
{
	float r = m_RuleScrollBar->GetRatio();
	if (r < 1.05f && r > -0.05f && (r < 0.499f || r > 0.501f))
	{
		for (size_t i = 0; i < m_Rules->size(); i++)
		{
			float scrollPos = ((r) * (m_RuleScrollBar->m_TargetSize - m_RuleScrollBar->m_TargetOverflowSize));

			m_Rules->at(i)->m_Interface->SetPosition(sf::Vector2f((float)(caSizes::LEFT_WINDOW_SIZE_X + 8),
				(float)(100 + (112 * i)) - scrollPos));
		}
	}
}

void TileScene2D::PreUpdateUnmanaged()
{
	for (int y = 0; y < BoardTileSizes[BoardSizeIndex]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndex]; x++)
		{
			//rulesets  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//DO NOT PROCESS COLORS. EVER. WHO CARES. USE THE COLOR INDEX, WHICH WILL LATER CORRESPOND TO THE INDEX AT caColors::caColors[INDEX]
			ProcessRulesAt(x, y);
		}
	}
}

void TileScene2D::ProcessRulesAt(int x, int y)
{
	//figure out what's around this cell
	int neighborColorIndices[8];
	int c = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (j == 0 && i == 0){ continue; }

			neighborColorIndices[c] = m_Cells[Math::mod((x + i), BoardTileSizes[BoardSizeIndex])][Math::mod((y + j), BoardTileSizes[BoardSizeIndex])].GetColorIndex();
			c++;
		}
	}



	//decide.
	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		//check if self color criteria is met for this rule:
		//if we don't care about color OR we are the right color
		if (m_Rules->at(i)->m_RuleData->ThisColorIndex == -1 || m_Cells[x][y].GetColorIndex() == m_Rules->at(i)->m_RuleData->ThisColorIndex)
		{
			int ifColorIndex = m_Rules->at(i)->m_RuleData->IfColorIndex;
			int ifColorIndexCount = 0;
			for (int j = 0; j < 8; j++)
			{
				if (neighborColorIndices[j] == ifColorIndex)
				{
					ifColorIndexCount++;
				}
			}
			if (ifColorIndexCount >= m_Rules->at(i)->m_RuleData->MinNumNeighbors && ifColorIndexCount <= m_Rules->at(i)->m_RuleData->MaxNumNeighbors)
			{
				m_Cells[x][y].SetColorIndexNextFrame(m_Rules->at(i)->m_RuleData->ThenColorIndex);
			}
		}
		//if this rule isnt compatible with this cell, continue to next rule.
	}

	//m_Rules->at(0)->m_RuleData->RingLevel;

	//Game of Life
			//if (m_Cells[x][y].IsAlive())
			//{
			//	shouldBeAlive = liveNeighborCount >= 2 && liveNeighborCount <= 3;
			//}
			//else
			//{
			//	shouldBeAlive = liveNeighborCount == 3;
			//}


			//m_Cells[x][y].SetAliveNextFrame(shouldBeAlive);


	//m_Cells[x][y].SetColorIndexNextFrame(-1?);
}


float fpsCountPost = 0.0f;
int numWaitFramesPost = 0;
bool hasBegunDragging = false;
sf::Vector2f initialMouseDownCellIndex;
int initialMouseDownColorIndex;

void TileScene2D::Update(float a_DeltaTime)
{
	//MAIN UPDATE LOOP
	if (m_IsSimulating)
	{
		fpsCountPost += a_DeltaTime;
		if (fpsCountPost > FramesPerSecond)
		{
			UpdateUnmanaged(a_DeltaTime);

			while (fpsCountPost > FramesPerSecond)
			{
				fpsCountPost -= FramesPerSecond;
			}
			numWaitFramesPost = 0;
		}
	}
	else
	{
		numWaitFramesPost++;
	}

	//handle dragging
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_WasLeftMousePressed)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSizes[BoardSizeIndex]));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSizes[BoardSizeIndex]));
		if (x < BoardTileSizes[BoardSizeIndex] && x >= 0 && y < BoardTileSizes[BoardSizeIndex] && y >= 0)
		{
			initialMouseDownCellIndex = sf::Vector2f((float)x, (float)y);
			initialMouseDownColorIndex = m_Cells[(unsigned int)x][(unsigned int)y].GetColorIndex();
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_WasLeftMousePressed)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSizes[BoardSizeIndex]));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSizes[BoardSizeIndex]));
		if (x < BoardTileSizes[BoardSizeIndex] && x >= 0 && y < BoardTileSizes[BoardSizeIndex] && y >= 0)
		{
			Vector2f tmp = sf::Vector2f(x, y);
			if (initialMouseDownCellIndex != tmp)
			{
				hasBegunDragging = true;
			}
		}
	}

	if (hasBegunDragging)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSizes[BoardSizeIndex]));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSizes[BoardSizeIndex]));
		if (x < BoardTileSizes[BoardSizeIndex] && x >= 0 && y < BoardTileSizes[BoardSizeIndex] && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].SetColorIndexImmediate(initialMouseDownColorIndex);
		}
	}

	//quick mouse input -- @TODO: check bounds of mouse.
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_WasLeftMousePressed && !hasBegunDragging)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSizes[BoardSizeIndex]));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSizes[BoardSizeIndex]));
		if (x < BoardTileSizes[BoardSizeIndex] && x >= 0 && y < BoardTileSizes[BoardSizeIndex] && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].AdvanceAliveImmediate();
		}
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && m_WasRightMousePressed && !hasBegunDragging)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSizes[BoardSizeIndex]));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSizes[BoardSizeIndex]));
		if (x < BoardTileSizes[BoardSizeIndex] && x >= 0 && y < BoardTileSizes[BoardSizeIndex] && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].ReverseAdvanceAliveImmediate();
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		hasBegunDragging = false;
	}

	//input mgr...
	m_WasLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_WasRightMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	m_WasSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void TileScene2D::UpdateUnmanaged(float a_DeltaTime)
{
	for (int y = 0; y < BoardTileSizes[BoardSizeIndex]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndex]; x++)
		{
			m_Cells[x][y].Update(a_DeltaTime);

		}
	}
}

void TileScene2D::Draw()
{

	//draw rulesets underneath everything.
	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Draw();
	}

	m_ControlsBG.Draw();
	g_WINDOW->draw(rulesetBG);
	m_LoadPreviousRulesetButton.Draw();
	m_LoadNextRulesetButton.Draw();
	m_SaveRulesetButton.Draw();
	m_NewRulesetButton.Draw();
	m_DeleteRulesetButton.Draw();
	g_WINDOW->draw(m_RulesetNameText);

	for (int y = 0; y < BoardTileSizes[BoardSizeIndex]; y++)
	{
		for (int x = 0; x < BoardTileSizes[BoardSizeIndex]; x++)
		{
			m_Cells[x][y].Draw();

		}
	}

	m_SimulateButton.Draw();
	m_IncreaseResButton.Draw();
	m_DecreaseResButton.Draw();
	g_WINDOW->draw(m_ResText);
	m_AddRuleButton.Draw();
	m_IncreasePlaybackSpeedButton.Draw();
	m_DecreasePlaybackSpeedButton.Draw();
	m_FPSButton.Draw();
	m_NextFrameButton.Draw();
	m_ClearColorButton.Draw();
	m_ClearButton.Draw();
	m_RuleScrollBar->Draw();
}