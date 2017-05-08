#include "Cell.h"
#include "StaticNamespaces.h"

extern sf::RenderWindow* g_WINDOW;

Cell::Cell(int a_m_ColorIndex = 0, Vector2f a_TilePos = Vector2f(0, 0), Vector2f a_PixelSize = Vector2f(0, 0))
	: m_TilePos(a_TilePos), m_RShape(RShape(a_PixelSize))
{
	m_ColorIndex = m_NextFrameColorIndex  = a_m_ColorIndex;
	m_RShape.setFillColor(caColors::caColors[a_m_ColorIndex]);
	m_RShape.setPosition(Math::ScalarMul(a_PixelSize, a_TilePos));
	m_RShape.setOutlineThickness(0);
}
 
Cell::~Cell()
{
}

void Cell::Update(double a_DeltaTime)
{
	//first
	m_RShape.setFillColor(caColors::caColors[m_ColorIndex]);
	m_ColorIndex = m_NextFrameColorIndex;
}

void Cell::Draw()
{
	g_WINDOW->draw(m_RShape);
}

void Cell::SetColorIndexNextFrame(int a_ColorIndex)
{
	m_NextFrameColorIndex = a_ColorIndex;
}

void Cell::SetColorIndexImmediate(int a_index)
{
	m_ColorIndex = a_index;
	if (m_ColorIndex >= caColors::caColorsLen)
	{
		m_ColorIndex = 0;
	}
	else if (m_ColorIndex < 0)
	{
		m_ColorIndex = caColors::caColorsLen - 1;
	}
	m_NextFrameColorIndex = m_ColorIndex;
	m_RShape.setFillColor(caColors::caColors[m_ColorIndex]);
}
void Cell::AdvanceAliveImmediate()
{
	m_ColorIndex++;
	if (m_ColorIndex >= caColors::caColorsLen)
	{
		m_ColorIndex = 0;
	}
	else if (m_ColorIndex < 0)
	{
		m_ColorIndex = caColors::caColorsLen - 1;
	}
	m_NextFrameColorIndex = m_ColorIndex;
	m_RShape.setFillColor(caColors::caColors[m_ColorIndex]);
}
void Cell::ReverseAdvanceAliveImmediate()
{
	m_ColorIndex--;
	if (m_ColorIndex >= caColors::caColorsLen)
	{
		m_ColorIndex = 0;
	}
	else if (m_ColorIndex < 0)
	{
		m_ColorIndex = caColors::caColorsLen - 1;
	}
	m_NextFrameColorIndex = m_ColorIndex;
	m_RShape.setFillColor(caColors::caColors[m_ColorIndex]);
}