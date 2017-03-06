#include "Button.h"
#include "Math.h"

Button::Button()
{
}

Button::~Button()
{
}

Button::Button(Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor, float a_OutlineThickness)
{
	m_RS = sf::RectangleShape(a_Size);
	m_RS.setOrigin(Vector2f(a_Size.x / 2, a_Size.y / 2));
	m_RS.setPosition(a_Position);
	m_RS.setFillColor(a_FillColor);
	m_RS.setOutlineColor(a_FillColor);
	m_RS.setOutlineThickness(a_OutlineThickness);
}

void Button::Update(float a_DeltaTime)
{

}

bool Button::IsMouseInRect()
{
	Vector2f rs_pos = m_RS.getPosition();
	sf::Vector2i rs_half_size = sf::Vector2i((m_RS.getSize().x / 2), (m_RS.getSize().y / 2));
	return (Math::AABB(	Vector2f(rs_pos.x - rs_half_size.x, rs_pos.y - rs_half_size.y),
						Vector2f(rs_pos.x + rs_half_size.x, rs_pos.y + rs_half_size.y),
						sf::Mouse::getPosition(*g_WINDOW)));
	
}

void Button::Draw()
{
	g_WINDOW->draw(m_RS);
}


