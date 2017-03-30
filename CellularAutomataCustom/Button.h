/*Button.h generated on Sat 03/04/2017 at 20:28:38.56*/
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Window\Mouse.hpp>
#include <stdio.h>

extern sf::RenderWindow* g_WINDOW;

class Button
{
public:
	typedef sf::Vector2f Vector2f;
	typedef sf::Color Color;
public:
	Button();
	Button(void(*a_ButtonPressEvent)(), Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor,
				sf::Font& a_Font, float a_OutlineThickness = 2.0f, std::string a_TextString = "", int a_CharacterSize = 12,
				Color a_TextColor = Color::White);
   ~Button();

public:
   void Update(float a_DeltaTime);
   void Draw();

private:
   //true on the frame the mouse is pressed down
   bool OnMouseDown = false;
   //true on the frame the mouse is released
   bool OnMouseRelease = false;
   //is the mouse being held down over this button?
   bool IsDown = false;
   //is the mouse position within the shape's region?
   bool IsMouseHovering = false;

   //if the mouse press was within the bounds of the button, we will wait for it to be released to fire the event.
   //as god intended.
   bool WaitingForRelease = false;

public:
	//ButtonPressEvent = yourfunctiondotcom()
	void (*ButtonPressEvent)();

private:
	bool m_WasMouseDown = false;

private:
	sf::RectangleShape m_RS;

	bool IsMouseInRect();

private:
	sf::Text m_Text;
public:
	void SetText(std::string a_TextString);
};
 
#endif
