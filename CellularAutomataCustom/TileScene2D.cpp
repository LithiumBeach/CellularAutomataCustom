#include "TileScene2D.h"

extern sf::RenderWindow* g_WINDOW;
extern const int WINDOW_SIZE_X = 500;
extern const int WINDOW_SIZE_Y = 650;

TileScene2D::TileScene2D()
{
	m_Cells = std::vector<std::vector<Cell>>();
	m_Cells.resize(BoardTileSize);


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

void TileScene2D::InitializeUI()
{
	m_SimulateButton = Button(Vector2f(100, 20), Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y - 100), sf::Color(100, 100, 255), sf::Color(10, 10, 10));
}


static const float FramesPerSecond = 1.0f/20.0f;
float fpsCount = 0.0f;
void TileScene2D::PreUpdate(float a_DeltaTime)
{
	fpsCount += a_DeltaTime;
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_WasSpacePressed)
	if (fpsCount > FramesPerSecond)
	{
		for (int y = 0; y < BoardTileSize; y++)
		{
			for (int x = 0; x < BoardTileSize; x++)
			{
				int liveNeighborCount = GetLiveNeighborsAroundIndex(x, y);
				bool shouldBeAlive = false;

				//RULES:
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

	//BUTTON UPDATE
	m_SimulateButton.Update(a_DeltaTime);
}

void TileScene2D::Update(float a_DeltaTime)
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_WasSpacePressed)
	if (fpsCount > FramesPerSecond)
	{
		for (int y = 0; y < BoardTileSize; y++)
		{
			for (int x = 0; x < BoardTileSize; x++)
			{
				m_Cells[x][y].Update(a_DeltaTime);

			}
		}
		fpsCount = 0;
	}

	//quick mouse input -- @TODO: check bounds of mouse.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_WasMousePressed)
	{
		float x = (int)((float)(sf::Mouse::getPosition(*g_WINDOW).x) / TilePixelSize);
		float y = (int)((float)(sf::Mouse::getPosition(*g_WINDOW).y) / TilePixelSize);
		if (x < BoardTileSize && x >= 0 && y < BoardTileSize && y >= 0)
		{
			m_Cells[(unsigned int)x][(unsigned int)y].ToggleAliveImmediate();
		}
	}

	//input mgr...
	m_WasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_WasSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
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
}