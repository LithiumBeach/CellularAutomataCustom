#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include "SceneManager.h"

#define DEMO_NAME "BaNaNaS"

const int WINDOW_SIZE_X = 500;
const int WINDOW_SIZE_Y = 500;

//global reference to the window (reference with extern in cpp files)
//extern sf::RenderWindow* g_WINDOW;
sf::RenderWindow* g_WINDOW;

//view offset vector : like going from camera to world space in 2d and not really tho
sf::Vector2f g_VIEWPORT_OFFSET;

sf::Color g_BackBuffer_Color = sf::Color(220, 255, 255);