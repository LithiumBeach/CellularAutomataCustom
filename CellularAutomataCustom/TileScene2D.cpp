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
	HandleSimulateButtonPressEvent_ptr = std::bind(&TileScene2D::HandleSimulateButtonPressEvent, this);
	HandlePlaybackSpeedIncButtonPressEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedIncButtonPressEvent, this);
	HandlePlaybackSpeedDecButtonPressEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedDecButtonPressEvent, this);
	HandleNextFrameButtonPressEvent_ptr = std::bind(&TileScene2D::HandleNextFrameButtonPressEvent, this);
	DoNothing_ptr = std::bind(&TileScene2D::DoNothing, this);

	m_SimulateButton = Button(Vector2f(160, 20), Vector2f((float)(caSizes::WINDOW_SIZE_X / 2), (float)(caSizes::WINDOW_SIZE_Y - 100)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "SIMULATE");
	m_SimulateButton.SetLeftMouseButtonPressEvent(HandleSimulateButtonPressEvent_ptr);

	m_IncreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS+");
	m_IncreasePlaybackSpeedButton.SetLeftMouseButtonPressEvent(HandlePlaybackSpeedIncButtonPressEvent_ptr);

	m_DecreasePlaybackSpeedButton = Button(Vector2f(36, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 75)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "FPS-");
	m_DecreasePlaybackSpeedButton.SetLeftMouseButtonPressEvent(HandlePlaybackSpeedDecButtonPressEvent_ptr);

	m_FPSButton = Button(Vector2f(0, 0), Vector2f((float)(caSizes::WINDOW_SIZE_X / 4), (float)(caSizes::WINDOW_SIZE_Y - 100)), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), *caFonts::s_DefaultFont, 2.0f, Math::to_string_with_precision(1.0f / FramesPerSecond));

	m_NextFrameButton = Button(Vector2f(64, 24), Vector2f((float)(caSizes::WINDOW_SIZE_X / 1.3f), (float)(caSizes::WINDOW_SIZE_Y - 125)), caColors::gray, caColors::border_gray, *caFonts::s_DefaultFont, 2.0f, "Frame+");
	m_NextFrameButton.SetLeftMouseButtonPressEvent(HandleNextFrameButtonPressEvent_ptr);

}

void TileScene2D::IntializeRules()
{
	m_Rules = new std::vector<Rule*>();
	m_Rules->push_back(new Rule(2, new bool[2]{true, false}, sf::Color::White, sf::Color::Black));

}

void TileScene2D::DoNothing(){}

void TileScene2D::HandleSimulateButtonPressEvent()
{
	m_IsSimulating = !(m_IsSimulating);
}

void TileScene2D::HandlePlaybackSpeedIncButtonPressEvent()
{
	playbackSpeedsIndex++;
	playbackSpeedsIndex = Math::Min(playbackSpeedsIndex, m_PlaybackSpeedsLen - 1);
	FramesPerSecond = 1.0f / m_PlaybackSpeeds[playbackSpeedsIndex];
	UpdateFPSCounter();
}
void TileScene2D::HandlePlaybackSpeedDecButtonPressEvent()
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

void TileScene2D::HandleNextFrameButtonPressEvent()
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
	m_IncreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_DecreasePlaybackSpeedButton.Update(a_DeltaTime);
	m_FPSButton.Update(a_DeltaTime);
	m_NextFrameButton.Update(a_DeltaTime);
	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Update(a_DeltaTime);
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
	int ifColorIndex = m_Rules->at(0)->m_RuleData->IfColorIndex;
	int ifColorIndexCount = 0;
	for (int i = 0; i < 8; i++)
	{
		if (neighborColorIndices[i] == ifColorIndex)
		{
			ifColorIndexCount++;
		}
	}
	if (ifColorIndexCount == m_Rules->at(0)->m_RuleData->NumNeighbors)
	{
		m_Cells[x][y].SetColorIndexNextFrame(m_Rules->at(0)->m_RuleData->ThenColorIndex);
	}

	//m_Rules->at(0)->m_RuleData->RingLevel;

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
	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			m_Cells[x][y].Draw();

		}
	}

	m_SimulateButton.Draw();
	m_IncreasePlaybackSpeedButton.Draw();
	m_DecreasePlaybackSpeedButton.Draw();
	m_FPSButton.Draw();
	m_NextFrameButton.Draw();


	for (size_t i = 0; i < m_Rules->size(); i++)
	{
		m_Rules->at(i)->Draw();
	}
}