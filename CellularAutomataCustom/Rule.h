/*Rule.h generated on Mon 03/27/2017 at 21:05:29.23*/
#ifndef RULE_H
#define RULE_H

#include <SFML\Graphics\RenderWindow.hpp>

extern sf::RenderWindow* g_WINDOW;

class Rule
{
public:
   Rule();
   ~Rule();

public:
   void Update(float a_DeltaTime);
   void Draw();

};
 
#endif
