/*StaticNamespaces.h generated on Mon 04/17/2017 at 16:29:19.26*/
#pragma once

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Font.hpp>

namespace caSizes
{
	extern int WINDOW_SIZE_X;
	extern int WINDOW_SIZE_Y;
}

namespace caColors
{
	extern sf::Color RuleLabelTextColor;

	extern sf::Color IncreaseButtonFill;
	extern sf::Color IncreaseButtonOutline;
	extern sf::Color DecreaseButtonFill;
	extern sf::Color DecreaseButtonOutline;
	extern sf::Color gray;
	extern sf::Color border_gray;

	extern sf::Color red;
	extern sf::Color green;
	extern sf::Color blue;
}

namespace caFonts
{
	extern sf::Font* s_DefaultFont;
	extern bool Load();
}