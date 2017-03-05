#include "TileArray.h"
 
TileArray::TileArray()
{
}

TileArray::~TileArray()
{
}

TileArray::TileArray(unsigned int sizeX, unsigned int sizeY)
{
	//initialize array with all zeros
	m_TileInts = std::vector<std::vector<int>>(sizeY, std::vector<int>(sizeX, 0));

	for (unsigned int y = 0; y < m_TileInts.size(); y++)
	{
		for (unsigned int x = 0; x < m_TileInts[y].size(); x++)
		{
			m_TileInts[x][y] = 0;
		}
	}
}

void TileArray::PrintContents()
{
	for (unsigned int y = 0; y < m_TileInts.size(); y++)
	{
		for (unsigned int x = 0; x < m_TileInts[y].size(); x++)
		{
			printf(" %d ", m_TileInts[x][y]);
		}
		printf("\n");
	}
}

unsigned int TileArray::At(int x, int y)
{
	return m_TileInts[x][y];
}

void TileArray::Set(int x, int y, int value)
{
	m_TileInts[x][y] = value;
}