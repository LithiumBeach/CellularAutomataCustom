#include "TileScene2D.h"
#include "StaticNamespaces.h"

extern sf::RenderWindow* g_WINDOW;

TileScene2D::TileScene2D()
{
	m_IsSimulating = false;
	m_IncreasePlaybackSpeedButton = Button();
	m_DecreasePlaybackSpeedButton = Button();

	m_PlaybackSpeedsLen = 12;
	playbackSpeedsIndex = 5;
	m_PlaybackSpeeds = new float[m_PlaybackSpeedsLen]{ 1.0f, 2.0f, 4.0f, 8.0f, 10.0f, 12.0f, 16.0f, 24.0f, 32.0f, 64.0f, 96.0f, 128.0f };

	m_FPSButton = Button();
	FramesPerSecond = 1.0f / m_PlaybackSpeeds[playbackSpeedsIndex];

	m_NextFrameButton = Button();

	m_Cells = std::vector<std::vector<Cell>>();
	m_Cells.resize(BoardTileSize);
	for (int i = 0; i < BoardTileSize; i++)
	{
		m_Cells[i].resize(BoardTileSize);
	}

	for (int i = 0; i < BoardTileSize; i++)
	{
		m_Cells[i].resize(BoardTileSize);
	}

	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			m_Cells[x][y] = Cell(0, Vector2f((float)x, (float)y), Vector2f(TilePixelSize, TilePixelSize));
			//printf("%d,%d|", (int)m_Cells[x][y].GetTilePos().x, (int)m_Cells[x][y].GetTilePos().y);//works :)
		}
		//printf("\n");
	}

	//line
	//m_Cells[3][5].SetAliveImmediate(true);//works :)
	//m_Cells[4][5].SetAliveImmediate(true);//works :)
	//m_Cells[5][5].SetAliveImmediate(true);//works :)

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

	//works :)
	//for (int y = 0; y < BoardTileSize; y++)
	//{
	//	for (int x = 0; x < BoardTileSize; x++)
	//	{
	//		printf("%d|", (int)m_Cells[x][y].IsAlive());
	//	}
	//	printf("\n");
	//}

	InitializeUI();
	IntializeRules();
}

TileScene2D::~TileScene2D()
{
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

	DoNothing_ptr = std::bind(&TileScene2D::DoNothing, this);

	m_SimulateButton = Button(Vector2f(160, 20), Vector2f((float)(caSizes::WINDOW_SIZE_X / 2), (float)(caSizes::WINDOW_SIZE_Y - 100)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "SIMULATE");
	m_SimulateButton.SetLeftMouseButtonReleaseEvent(HandleSimulateButtonReleaseEvent_ptr);

	m_IncreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS+");
	m_IncreasePlaybackSpeedButton.SetLeftMouseButtonReleaseEvent(HandlePlaybackSpeedIncButtonReleaseEvent_ptr);

	m_DecreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 75)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS-");
	m_DecreasePlaybackSpeedButton.SetLeftMouseButtonReleaseEvent(HandlePlaybackSpeedDecButtonReleaseEvent_ptr);

	m_FPSButton = Button(Vector2f(0, 0), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 100)), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), *caFonts::s_DefaultFont, 2.0f, Math::to_string_with_precision(1.0f / FramesPerSecond));
	
	m_ClearButton = Button(Vector2f(86, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 10), (float)(caSizes::WINDOW_SIZE_Y - 100)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "Clear Board");
	m_ClearButton.SetLeftMouseButtonReleaseEvent(HandleClearEvent_ptr);
	m_ClearColorIndex = 0;
	m_ClearColorButton = Button(Vector2f(24, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 10), (float)(caSizes::WINDOW_SIZE_Y - 132)), caColors::caColors[m_ClearColorIndex], caColors::border_gray, *caFonts::s_DefaultFont);
	m_ClearColorButton.SetLeftMouseButtonReleaseEvent(HandleClearColorIncrementEvent_ptr);
	m_ClearColorButton.SetRightMouseButtonReleaseEvent(HandleClearColorDecrementEvent_ptr);

	m_NextFrameButton = Button(Vector2f(64, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 1.3f), (float)(caSizes::WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "Frame+");
	m_NextFrameButton.SetLeftMouseButtonReleaseEvent(HandleNextFrameButtonReleaseEvent_ptr);

	m_AddRuleButton = Button(Vector2f(80, 20), Vector2f((float)(caSizes::WINDOW_SIZE_X / 2), (float)(caSizes::WINDOW_SIZE_Y - 126)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "ADD RULE");
	m_AddRuleButton.SetLeftMouseButtonReleaseEvent(HandleAddRuleEvent_ptr);


	//scroll bar
	float scrollbarX = (float)(caSizes::WINDOW_SIZE_X * 0.98f);
	m_RuleScrollBar = new ScrollBar(sf::Vector2f(scrollbarX, (float)(caSizes::WINDOW_SIZE_Y - 167)), sf::Vector2f(scrollbarX, (float)(caSizes::WINDOW_SIZE_Y + 70)), 13, (float)(caSizes::WINDOW_SIZE_Y - 40 + 112));


	m_ControlsBG = sf::RectangleShape(sf::Vector2f(caSizes::WINDOW_SIZE_X, 100));
	m_ControlsBG.setFillColor(caColors::g_BackBuffer_Color);
	m_ControlsBG.setOutlineThickness(0);
	m_ControlsBG.setPosition(0, (float)(caSizes::WINDOW_SIZE_Y - 150));

}

void TileScene2D::IntializeRules()
{
	m_Rules = new std::vector<Rule*>();
	m_Rules->push_back(new Rule(2, new bool[2]{true, false}, sf::Color::White, sf::Color::Black));

}

void TileScene2D::HandleAddRuleEvent()
{
	m_Rules->push_back(new Rule(2, new bool[2]{true, false}, sf::Color::White, sf::Color::Black));
	m_RuleScrollBar->UpdateTargetSize((float)(caSizes::WINDOW_SIZE_Y - 40 + (112 * (Rule::s_RuleCount-1))));
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
	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
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

float fpsCountPre = 0.0f;
int numWaitFramesPre = 0;
void TileScene2D::PreUpdate(float a_DeltaTime)
{
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
	m_AddRuleButton.Update(a_DeltaTime);
	m_IncreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_DecreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_FPSButton.Update(a_DeltaTime);
	m_NextFrameButton.Update(a_DeltaTime);
	m_ClearColorButton.Update(a_DeltaTime);
	m_ClearButton.Update(a_DeltaTime);
	m_RuleScrollBar->Update(a_DeltaTime);
	if (m_RuleScrollBar->m_IsScrolling)
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
	for (size_t i = 0; i < m_Rules->size(); i++)
	{// - m_RuleScrollBar->m_TargetOverflowSize
		float scrollPos = ((1.0f - m_RuleScrollBar->GetRatio()) * (m_RuleScrollBar->m_TargetSize - m_RuleScrollBar->m_TargetOverflowSize));
		m_Rules->at(i)->m_Interface->SetPosition(sf::Vector2f((float)(caSizes::WINDOW_SIZE_X * 0.01f), 
															  (float)(caSizes::WINDOW_SIZE_Y - 40 + 112 * i) - scrollPos));
	}
}

void TileScene2D::PreUpdateUnmanaged()
{
	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			//RULES  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

			neighborColorIndices[c] = m_Cells[Math::mod((x + i), BoardTileSize)][Math::mod((y + j), BoardTileSize)].GetColorIndex();
			c++;
		}
	}



	//decide.
	for (size_t i = 0; i < m_Rules->size(); i++)
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
		if (ifColorIndexCount == m_Rules->at(i)->m_RuleData->NumNeighbors)
		{
			m_Cells[x][y].SetColorIndexNextFrame(m_Rules->at(i)->m_RuleData->ThenColorIndex);
		}
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


int TileScene2D::GetLiveNeighborsAroundIndex(int x, int y)
{
	
	return -1;
}



float fpsCountPost = 0.0f;
int numWaitFramesPost = 0;
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


	//printf("%f | \n", m_RuleScrollBar->GetRatio());


	//quick mouse input -- @TODO: check bounds of mouse.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_WasLeftMousePressed)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSize));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSize));
		if (x < BoardTileSize && x >= 0 && y < BoardTileSize && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].AdvanceAliveImmediate();
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !m_WasRightMousePressed)
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSize));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSize));
		if (x < BoardTileSize && x >= 0 && y < BoardTileSize && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].ReverseAdvanceAliveImmediate();
		}
	}

	//input mgr...
	m_WasLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_WasRightMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	m_WasSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void TileScene2D::UpdateUnmanaged(float a_DeltaTime)
{
	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			m_Cells[x][y].Update(a_DeltaTime);

		}
	}
}

void TileScene2D::Draw()
{

	//draw rules underneath everything.
	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Draw();
	}


	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			m_Cells[x][y].Draw();

		}
	}

	g_WINDOW->draw(m_ControlsBG);

	m_SimulateButton.Draw();
	m_AddRuleButton.Draw();
	m_IncreasePlaybackSpeedButton.Draw();
	m_DecreasePlaybackSpeedButton.Draw();
	m_FPSButton.Draw();
	m_NextFrameButton.Draw();
	m_ClearColorButton.Draw();
	m_ClearButton.Draw();
	m_RuleScrollBar->Draw();
}