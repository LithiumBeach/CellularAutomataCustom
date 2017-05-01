#include "StaticNamespaces.h"

namespace caColors
{
	sf::Color g_BackBuffer_Color = sf::Color(20, 23, 20);

	sf::Color RuleLabelTextColor = sf::Color(240, 235, 255);
	
	sf::Color IncreaseButtonFill = sf::Color( 0, 255, 0);
	sf::Color IncreaseButtonOutline = sf::Color(200, 255, 200);
	sf::Color DecreaseButtonFill = sf::Color(255, 0, 0);
	sf::Color DecreaseButtonOutline = sf::Color(255, 200, 200);

	sf::Color gray = sf::Color(30, 30, 30);
	sf::Color border_gray = sf::Color(200, 200, 200);

	sf::Color red = sf::Color(255, 0, 0);
	sf::Color green = sf::Color(0, 255, 0);
	sf::Color blue = sf::Color(0, 0, 255);

	
	sf::Color caColors[caColorsLen] =
	{
		sf::Color(255, 255, 255),
		sf::Color(0, 0, 0),

		sf::Color(255, 0, 0),
		sf::Color(0, 255, 0),
		sf::Color(0, 0, 255),

		sf::Color(0, 255, 255),
		sf::Color(255, 0, 255),
		sf::Color(255, 255, 0),
	};

}



namespace caSizes
{
	int WINDOW_SIZE_X = 500;
	int WINDOW_SIZE_Y = 650;
}



namespace caFonts
{
	sf::Font* s_DefaultFont;

	bool Load()
	{
		s_DefaultFont = new sf::Font();
		if (!s_DefaultFont->loadFromFile("../assets/arial.ttf"))
		{
			printf("\n\n!nerror loading font!\n\n");
			return false;
		}

		return true;
	}
}