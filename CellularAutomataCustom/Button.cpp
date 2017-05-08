#include "Button.h"
#include "Math.h"
#include "StaticNamespaces.h"

Button::~Button()
{
}

Button::Button(Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor,
	sf::Font& a_Font, float a_OutlineThickness, std::string a_TextString, int a_CharacterSize, Color a_TextColor)
{
	m_RS = sf::RectangleShape(a_Size);
	m_RS.setPosition(a_Position);
	m_RS.setFillColor(a_FillColor);
	m_RS.setOutlineColor(a_OutlineColor);
	m_RS.setOutlineThickness(a_OutlineThickness);
	m_RS.setOrigin(Vector2f(m_RS.getSize().x * 0.5f, m_RS.getSize().y * 0.5f));

	//text
	m_Text.setFont(a_Font);
	m_Text.setCharacterSize(a_CharacterSize);
	m_Text.setColor(a_TextColor);
	m_Text.setStyle(sf::Text::Regular);
	m_Text.setOrigin(Vector2f(m_Text.getLocalBounds().width * .5f, m_Text.getLocalBounds().height * .5f));
	SetText(a_TextString);
}

void Button::Update(float a_DeltaTime)
{
	bool isLeftMouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool isRightMouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	IsMouseHovering = IsMouseInRect();

	//mouse is hovering over the rect, this is the first frame of mouse click.
	OnLeftMouseDown = isLeftMouseDown && !m_WasLeftMouseDown;
	OnRightMouseDown = isRightMouseDown && !m_WasRightMouseDown;

	if (IsMouseInRect())
	{
		if (!input::LMB_Consumed && OnLeftMouseDown)
		{
			WaitingForLeftMouseRelease = true;
			if (LeftMouseButtonPressEvent != NULL)
			{
				LeftMouseButtonPressEvent();
			}
			input::LMB_Consumed = true;
		}
		if (!input::RMB_Consumed && OnRightMouseDown)
		{
			WaitingForRightMouseRelease = true;
			if (RightMouseButtonPressEvent != NULL)
			{
				RightMouseButtonPressEvent();
			}
			input::RMB_Consumed = true;
		}
	}

	OnLeftMouseRelease = !isLeftMouseDown && m_WasLeftMouseDown && WaitingForLeftMouseRelease;
	OnRightMouseRelease = !isRightMouseDown && m_WasRightMouseDown && WaitingForRightMouseRelease;

	if (OnLeftMouseRelease)
	{
		//activate()
		if (LeftMouseButtonReleaseEvent != NULL)
			LeftMouseButtonReleaseEvent();
		WaitingForLeftMouseRelease = false;
	}
	if (OnRightMouseRelease)
	{
		//activate()
		if (RightMouseButtonReleaseEvent != NULL)
			RightMouseButtonReleaseEvent();
		WaitingForRightMouseRelease = false;
	}

	m_WasLeftMouseDown = isLeftMouseDown;
	m_WasRightMouseDown = isRightMouseDown;
}

bool Button::IsMouseInRect()
{
	Vector2f rs_pos = m_RS.getPosition();
	sf::Vector2i rs_half_size = sf::Vector2i((int)(m_RS.getSize().x * 0.5f), (int)(m_RS.getSize().y * 0.5f));
	return (Math::AABB(Vector2f(rs_pos.x - rs_half_size.x - m_RS.getOutlineThickness() * 2, rs_pos.y - rs_half_size.y - m_RS.getOutlineThickness() * 2),
		Vector2f(rs_pos.x + rs_half_size.x + m_RS.getOutlineThickness() * 2, rs_pos.y + rs_half_size.y + m_RS.getOutlineThickness() * 2),
						sf::Mouse::getPosition(*g_WINDOW)));
	
}

void Button::SetText(std::string a_TextString)
{
	m_Text.setString(a_TextString);
	m_Text.setOrigin(sf::Vector2f(m_Text.getLocalBounds().left + m_Text.getLocalBounds().width  * 0.5f, m_Text.getLocalBounds().top + m_Text.getLocalBounds().height  * 0.5f));
	m_Text.setPosition(m_RS.getPosition());
}

void Button::SetPosition(sf::Vector2f a_pos)
{
	m_Text.setPosition(a_pos);
	m_Text.setOrigin(sf::Vector2f(m_Text.getLocalBounds().left + m_Text.getLocalBounds().width  * 0.5f, m_Text.getLocalBounds().top + m_Text.getLocalBounds().height  * 0.5f));
	m_RS.setPosition(a_pos);
	m_RS.setOrigin(Vector2f(m_RS.getSize().x * 0.5f, m_RS.getSize().y * 0.5f));
}

void Button::Draw()
{
	g_WINDOW->draw(m_RS);
	g_WINDOW->draw(m_Text);
}




void Button::SetLeftMouseButtonReleaseEvent(const std::function<void()> ev)
{
	LeftMouseButtonReleaseEvent = ev;
}
void Button::SetRightMouseButtonReleaseEvent(const std::function<void()> ev)
{
	RightMouseButtonReleaseEvent = ev;
}

void Button::SetLeftMouseButtonPressEvent(const std::function<void()> ev)
{
	LeftMouseButtonPressEvent = ev;
}
void Button::SetRightMouseButtonPressEvent(const std::function<void()> ev)
{
	RightMouseButtonPressEvent = ev;
}

