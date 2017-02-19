#include "TileScene2D.h"

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
			m_Cells[x][y] = Cell(false, sf::Vector2f((float)x, (float)y), sf::Vector2f(TilePixelSize, TilePixelSize));
			//printf("%d,%d|", (int)m_Cells[x][y].GetTilePos().x, (int)m_Cells[x][y].GetTilePos().y);//works :)
		}
		printf("\n");
	}

	//line
	//m_Cells[3][5].SetAliveImmediate(true);//works :)
	//m_Cells[4][5].SetAliveImmediate(true);//works :)
	//m_Cells[5][5].SetAliveImmediate(true);//works :)

	//r-pentomino
	m_Cells[11][10].SetAliveImmediate(true);//works :)
	m_Cells[12][11].SetAliveImmediate(true);//works :)
	m_Cells[10][12].SetAliveImmediate(true);//works :)
	m_Cells[11][12].SetAliveImmediate(true);//works :)
	m_Cells[12][12].SetAliveImmediate(true);//works :)

	//works :)
	//for (int y = 0; y < BoardTileSize; y++)
	//{
	//	for (int x = 0; x < BoardTileSize; x++)
	//	{
	//		printf("%d|", (int)m_Cells[x][y].IsAlive());
	//	}
	//	printf("\n");
	//}
}

TileScene2D::~TileScene2D()
{
}


static const float FramesPerSecond = 0.01f;
float fpsCount = 0.0f;
void TileScene2D::PreUpdate(float a_DeltaTime)
{
	fpsCount += a_DeltaTime;
	if (fpsCount > FramesPerSecond)
	{
		for (int y = 0; y < BoardTileSize; y++)
		{
			for (int x = 0; x < BoardTileSize; x++)
			{
				int liveNeighborCount = GetLiveNeighborsAroundIndex(x, y);
				bool shouldBeAlive = false;
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
}

void TileScene2D::Update(float a_DeltaTime)
{
	if (fpsCount > FramesPerSecond)
	{
		for (int y = 0; y < BoardTileSize; y++)
		{
			for (int x = 0; x < BoardTileSize; x++)
			{
				m_Cells[x][y].Update(a_DeltaTime);

			}
		}
	}
	sf::Rect<float> window_bounds = sf::Rect<float>(g_WINDOW->getPosition().x, g_WINDOW->getPosition().y, g_WINDOW->getSize().x, g_WINDOW->getSize().y);
	//quick mouse input -- @TODO: check bounds of mouse.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_WasMousePressed)
	{
		m_Cells[sf::Mouse::getPosition(*g_WINDOW).x / TilePixelSize][sf::Mouse::getPosition(*g_WINDOW).y / TilePixelSize].SetAliveImmediate(true);
	}
	m_WasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

int TileScene2D::GetLiveNeighborsAroundIndex(int x, int y)
{
	int nCount = 0;

	if (x > 1)
	{
		nCount += m_Cells[x - 1][y].IsAlive();
	}
	else
	{
		nCount += m_Cells[BoardTileSize - 1][y].IsAlive();
	}

	if (x < BoardTileSize - 1)
	{
		nCount += m_Cells[x + 1][y].IsAlive();
	}
	else
	{
		nCount += m_Cells[0][y].IsAlive();
	}

	if (y > 1)
	{
		nCount += m_Cells[x][y - 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[x][BoardTileSize - 1].IsAlive();
	}

	if (y < BoardTileSize - 1)
	{
		nCount += m_Cells[x][y + 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[x][0].IsAlive();
	}

	if (x > 1 && y > 1)
	{
		nCount += m_Cells[x - 1][y - 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[BoardTileSize - 1][BoardTileSize - 1].IsAlive();
	}

	if (x > 1 && y < BoardTileSize - 1)
	{
		nCount += m_Cells[x - 1][y + 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[BoardTileSize - 1][0].IsAlive();
	}


	if (x < BoardTileSize - 1 && y > 1)
	{
		nCount += m_Cells[x + 1][y - 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[0][BoardTileSize - 1].IsAlive();
	}


	if (x < BoardTileSize - 1 && y < BoardTileSize - 1)
	{
		nCount += m_Cells[x + 1][y + 1].IsAlive();
	}
	else
	{
		nCount += m_Cells[0][0].IsAlive();
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

}