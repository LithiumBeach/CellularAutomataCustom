/*Button.h generated on Sat 03/04/2017 at 20:28:38.56*/
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Mouse.hpp>

extern sf::RenderWindow* g_WINDOW;

class Button
{
public:
	typedef sf::Vector2f Vector2f;
	typedef sf::Color Color;
public:
	Button();
	Button(Vector2f a_Size, Vector2f a_Position, Color a_FillColor, Color a_OutlineColor, float a_OutlineThickness=2.0f);
   ~Button();

public:
   void Update(float a_DeltaTime);
   void Draw();

   //true on the frame the mouse is pressed down
   bool OnMouseDown = false;
   //true on the frame the mouse is released
   bool OnMouseRelease = false;
   //is the mouse being held down over this button?
   bool IsDown = false;
   //is the mouse position within the shape's region?
   bool IsMouseHovering = false;

private:
	sf::RectangleShape m_RS;

	bool IsMouseInRect();
};
 
#endif
