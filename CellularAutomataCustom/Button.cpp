#include "Button.h"
#include "Math.h"

Button::Button()
{
}

Button::~Button()
{
}

Button::Button(	std::function<void()> a_ButtonPressEvent, Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor,
				sf::Font& a_Font, float a_OutlineThickness, std::string a_TextString, int a_CharacterSize, Color a_TextColor)
{
	m_RS = sf::RectangleShape(a_Size);
	m_RS.setPosition(a_Position);
	m_RS.setFillColor(a_FillColor);
	m_RS.setOutlineColor(a_OutlineColor);
	m_RS.setOutlineThickness(a_OutlineThickness);
	m_RS.setOrigin(Vector2f(m_RS.getSize().x * 0.5f, m_RS.getSize().y * 0.5f));
	ButtonPressEvent = a_ButtonPressEvent;

	//text
	m_Text.setFont(a_Font);
	m_Text.setCharacterSize(a_CharacterSize);
	m_Text.setColor(a_TextColor);
	m_Text.setStyle(sf::Text::Bold);
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
		if (ButtonPressEvent != NULL)
			ButtonPressEvent();
		WaitingForRelease = false;
	}

	m_WasMouseDown = isMouseDown;
}

bool Button::IsMouseInRect()
{
	Vector2f rs_pos = m_RS.getPosition();
	sf::Vector2i rs_half_size = sf::Vector2i((int)(m_RS.getSize().x * 0.5f), (int)(m_RS.getSize().y * 0.5f));
	return (Math::AABB(	Vector2f(rs_pos.x - rs_half_size.x, rs_pos.y - rs_half_size.y),
						Vector2f(rs_pos.x + rs_half_size.x, rs_pos.y + rs_half_size.y),
						sf::Mouse::getPosition(*g_WINDOW)));
	
}

void Button::SetText(std::string a_TextString)
{
	m_Text.setString(a_TextString);
	m_Text.setOrigin(sf::Vector2f(m_Text.getLocalBounds().left + m_Text.getLocalBounds().width / 2.0f, m_Text.getLocalBounds().top + m_Text.getLocalBounds().height / 2.0f));
	m_Text.setPosition(m_RS.getPosition());
}

void Button::Draw()
{
	g_WINDOW->draw(m_RS);
	g_WINDOW->draw(m_Text);
}


