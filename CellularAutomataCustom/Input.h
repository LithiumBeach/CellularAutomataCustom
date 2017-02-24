/*Input.h generated on Thu 02/23/2017 at 21:54:53.15*/

#include <SFML\Graphics\RenderWindow.hpp>

extern sf::RenderWindow* g_WINDOW;

class Input
{
public:
  Input(){};
  ~Input(){};

public:
  void Update(float a_DeltaTime);
  void Draw();

};
 
