#include "Cell.h"

extern sf::RenderWindow* g_WINDOW;

Cell::Cell(bool a_IsAlive, sf::Vector2f a_TilePos, sf::Vector2f a_PixelSize)
	: m_IsAlive(a_IsAlive), m_TilePos(a_TilePos), m_RectangleShape(sf::RectangleShape(a_PixelSize))
{
	m_WillBeAliveOnUpdate = m_IsAlive;
	m_RectangleShape.setPosition(Math::ScalarMul(a_PixelSize, a_TilePos));

	UpdateFillColor();
}
 
Cell::~Cell()
{
}

void Cell::Update(float a_DeltaTime)
{
	//first
	m_IsAlive = m_WillBeAliveOnUpdate;

	//set fill color corresponding with alive or dead
	UpdateFillColor();
}

void Cell::Draw()
{
	g_WINDOW->draw(m_RectangleShape);
}

