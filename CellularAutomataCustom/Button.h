/*Button.h generated on Sat 03/04/2017 at 20:28:38.56*/
#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Window\Mouse.hpp>
#include <stdio.h>
#include <functional>

extern sf::RenderWindow* g_WINDOW;

class Button
{
public:
	Button(){
	};
public:
	typedef sf::Vector2f Vector2f;
	typedef sf::Color Color;
	//Button(const std::function<void()> a_ButtonPressEvent, Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor,
	//			sf::Font& a_Font, float a_OutlineThickness = 2.0f, std::string a_TextString = "", int a_CharacterSize = 12,
	//			Color a_TextColor = Color::White);

	void SetLeftMouseButtonPressEvent(const std::function<void()> a_ButtonPressEvent);
	void SetRightMouseButtonPressEvent(const std::function<void()> a_ButtonPressEvent);

	Button(Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor,
		sf::Font& a_Font, float a_OutlineThickness = 2.0f, std::string a_TextString = "", int a_CharacterSize = 12,
		Color a_TextColor = Color::White);

	~Button();

public:
	//use this for initialization lol fuck you
	//ButtonPressEvent = yourfunctiondotcom()
	std::function<void()> LeftMouseButtonPressEvent;
	std::function<void()> RightMouseButtonPressEvent;

public:
   void Update(float a_DeltaTime);
   void Draw();

private:
   //true on the frame the mouse is pressed down
	bool OnLeftMouseDown = false;
	bool OnRightMouseDown = false;
   //true on the frame the mouse is released
	bool OnLeftMouseRelease = false;
	bool OnRightMouseRelease = false;
   //is the mouse being held down over this button?
	bool IsLeftMouseDown = false;
	bool IsRightMouseDown = false;
   //is the mouse position within the shape's region?
   bool IsMouseHovering = false;

   //if the mouse press was within the bounds of the button, we will wait for it to be released to fire the event.
   //as god intended.
   bool WaitingForLeftMouseRelease = false;
   bool WaitingForRightMouseRelease = false;
private:
	bool m_WasLeftMouseDown = false;
	bool m_WasRightMouseDown = false;

private:
	sf::RectangleShape m_RS;

	bool IsMouseInRect();

private:
	sf::Text m_Text;
public:
	void SetText(std::string a_TextString);

	void SetPosition(sf::Vector2f a_pos);

	void SetOrigin(sf::Vector2f a_pos);

	void SetOutline(sf::Color a_color){ m_RS.setOutlineColor(a_color); };
	void SetFill(sf::Color a_color){ m_RS.setFillColor(a_color); };
	sf::Color GetFill(){ return m_RS.getFillColor(); }
};
 

