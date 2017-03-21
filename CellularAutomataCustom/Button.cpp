#include "Button.h"
#include "Math.h"

Button::Button()
{
}

Button::~Button()
{
}

Button::Button(void (*a_ButtonPressEvent)(), Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor, float a_OutlineThickness, std::string a_TextString)
{
	m_RS = sf::RectangleShape(a_Size);
	m_RS.setOrigin(Vector2f(a_Size.x / 2, a_Size.y / 2));
	m_RS.setPosition(a_Position);
	m_RS.setFillColor(a_FillColor);
	m_RS.setOutlineColor(a_FillColor);
	m_RS.setOutlineThickness(a_OutlineThickness);
	ButtonPressEvent = a_ButtonPressEvent;

	//text
	SetText(a_TextString);
}

void Button::Update(float a_DeltaTime)
{
	bool isMouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	IsMouseHovering = IsMouseInRect();

	//mouse is hovering over the rect, this is the first frame of mouse left click.
	OnMouseDown = isMouseDown && !m_WasMouseDown;

	if (OnMouseDown && IsMouseInRect())
	{
		WaitingForRelease = true;
	}

	OnMouseRelease = !isMouseDown && m_WasMouseDown && WaitingForRelease;

	if (OnMouseRelease)
	{
		//activate()
		(*ButtonPressEvent)();
		WaitingForRelease = false;
	}

	m_WasMouseDown = isMouseDown;
}

bool Button::IsMouseInRect()
{
	Vector2f rs_pos = m_RS.getPosition();
	sf::Vector2i rs_half_size = sf::Vector2i((m_RS.getSize().x / 2), (m_RS.getSize().y / 2));
	return (Math::AABB(	Vector2f(rs_pos.x - rs_half_size.x, rs_pos.y - rs_half_size.y),
						Vector2f(rs_pos.x + rs_half_size.x, rs_pos.y + rs_half_size.y),
						sf::Mouse::getPosition(*g_WINDOW)));
	
}

void Button::SetText(std::string a_TextString)
{
	m_Text.setString(a_TextString);
	m_Text.setOrigin(sf::Vector2f(m_Text.getGlobalBounds().width / 2.0f, m_Text.getGlobalBounds().height / 2.0f));
	m_Text.setPosition(m_RS.getPosition());
}

void Button::Draw()
{
	g_WINDOW->draw(m_RS);
	g_WINDOW->draw(m_Text);
}


