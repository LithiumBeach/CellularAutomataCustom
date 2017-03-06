#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include "SceneManager.h"

#define DEMO_NAME "BaNaNaS"

const int WINDOW_SIZE_X = 500;
const int WINDOW_SIZE_Y = 650;

//global reference to the window (reference with extern in cpp files)
//extern sf::RenderWindow* g_WINDOW;
sf::RenderWindow* g_WINDOW;

sf::Color g_BackBuffer_Color = sf::Color(0, 0, 0);