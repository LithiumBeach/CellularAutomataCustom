#include "Main.h"

void main()
{
	//create our rendering window
	g_WINDOW = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), DEMO_NAME);

	//update/draw timing
	sf::Clock clock;

	//delta time between frames -- to be passed into all update functions from here.
	float deltaTime = (float)clock.restart().asMilliseconds();
	sf::Clock deltaClock;

	//enable vsync
	g_WINDOW->setVerticalSyncEnabled(true);
	g_WINDOW->setFramerateLimit(256);

	SceneManager* MasterP = new SceneManager();


	//main game loop
	while (g_WINDOW->isOpen())
	{
		sf::Time dt = deltaClock.restart();
		sf::Event e;

		//poll sf::Window for all events e
		while (g_WINDOW->pollEvent(e))
		{
			//hard-coded key press events
			switch (e.type)
			{
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape)
				{
					g_WINDOW->close();
				}
				break;
			}

		}

		//update code here:
		MasterP->PreUpdate(dt.asSeconds());
		MasterP->Update(dt.asSeconds());


		//clear displayed window
		g_WINDOW->clear(g_BackBuffer_Color);

		//draw code here:
		MasterP->Draw();

		//draw the backbuffer
		g_WINDOW->display();
	}
}