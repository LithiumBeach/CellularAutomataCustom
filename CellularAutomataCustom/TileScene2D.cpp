#include "TileScene2D.h"

extern sf::RenderWindow* g_WINDOW;
extern const int WINDOW_SIZE_X = 500;
extern const int WINDOW_SIZE_Y = 650;

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

	m_DefaultFont = new sf::Font();
	if (!m_DefaultFont->loadFromFile("../assets/arial.ttf"))
	{
		printf("\n\n!nerror loading font!\n\n");
	}


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
			m_Cells[x][y] = Cell(false, Vector2f((float)x, (float)y), Vector2f(TilePixelSize, TilePixelSize));
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
	m_Cells[11][10].SetAliveImmediate(true);//works :)
	m_Cells[10][11].SetAliveImmediate(true);//works :)
	m_Cells[12][12].SetAliveImmediate(true);//works :)
	m_Cells[11][12].SetAliveImmediate(true);//works :)
	m_Cells[10][12].SetAliveImmediate(true);//works :)

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
}

TileScene2D::~TileScene2D()
{
}

const sf::Color gray = sf::Color(30, 30, 30);
const sf::Color border_gray = sf::Color(200, 200, 200);
void TileScene2D::InitializeUI()
{
	HandleSimulateButtonPressEvent_ptr = std::bind(&TileScene2D::HandleSimulateButtonPressEvent, this);
	HandlePlaybackSpeedIncButtonPressEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedIncButtonPressEvent, this);
	HandlePlaybackSpeedDecButtonPressEvent_ptr = std::bind(&TileScene2D::HandlePlaybackSpeedDecButtonPressEvent, this);
	HandleNextFrameButtonPressEvent_ptr = std::bind(&TileScene2D::HandleNextFrameButtonPressEvent, this);
	DoNothing_ptr = std::bind(&TileScene2D::DoNothing, this);

	m_SimulateButton = Button(HandleSimulateButtonPressEvent_ptr, Vector2f(160, 20), Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y - 100), gray, border_gray, *m_DefaultFont, 2.0f, "SIMULATE");
	m_IncreasePlaybackSpeedButton = Button(HandlePlaybackSpeedIncButtonPressEvent_ptr, Vector2f(36, 24), Vector2f(WINDOW_SIZE_X / 4, WINDOW_SIZE_Y - 125), gray, border_gray, *m_DefaultFont, 2.0f, "FPS+");
	m_DecreasePlaybackSpeedButton = Button(HandlePlaybackSpeedDecButtonPressEvent_ptr, Vector2f(36, 24), Vector2f(WINDOW_SIZE_X / 4, WINDOW_SIZE_Y - 75), gray, border_gray, *m_DefaultFont, 2.0f, "FPS-");
	m_FPSButton = Button(DoNothing_ptr, Vector2f(0, 0), Vector2f(WINDOW_SIZE_X / 4, WINDOW_SIZE_Y - 100), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), *m_DefaultFont, 2.0f, Math::to_string_with_precision(1.0f / FramesPerSecond));
	m_NextFrameButton = Button(HandleNextFrameButtonPressEvent_ptr, Vector2f(64, 24), Vector2f(WINDOW_SIZE_X / 1.3f, WINDOW_SIZE_Y - 125), gray, border_gray, *m_DefaultFont, 2.0f, "Frame+");
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
}

void TileScene2D::PreUpdateUnmanaged()
{
	for (int y = 0; y < BoardTileSize; y++)
	{
		for (int x = 0; x < BoardTileSize; x++)
		{
			int liveNeighborCount = GetLiveNeighborsAroundIndex(x, y);
			bool shouldBeAlive = false;

			//RULES  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			if (m_Cells[x][y].IsAlive())
			{
				shouldBeAlive = liveNeighborCount >= 2 && liveNeighborCount <= 3;
			}
			else
			{
				shouldBeAlive = liveNeighborCount == 3;
			}


			m_Cells[x][y].SetAliveNextFrame(shouldBeAlive);
		}
	}
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSize));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSize));
		if (x < BoardTileSize && x >= 0 && y < BoardTileSize && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].SetAliveImmediate(true);
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		float x = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSize));
		float y = (float)((int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSize));
		if (x < BoardTileSize && x >= 0 && y < BoardTileSize && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].SetAliveImmediate(false);
		}
	}

	//input mgr...
	m_WasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
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

int TileScene2D::GetLiveNeighborsAroundIndex(int x, int y)
{
	int nCount = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (j == 0 && i == 0){ continue; }

			nCount += m_Cells[Math::mod((x + i), BoardTileSize)][Math::mod((y + j), BoardTileSize)].IsAlive();
		}

	}
	return nCount;
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
}