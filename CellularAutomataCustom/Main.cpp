#include "Main.h"

void main()
{
	//create our rendering window
	g_WINDOW = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), DEMO_NAME);

	//update/draw timing
	sf::Clock clock;

	//delta time between frames -- to be passed into all update functions from here.
	float deltaTime = clock.restart().asSeconds();

	//enable vsync
	g_WINDOW->setVerticalSyncEnabled(true);
	g_WINDOW->setFramerateLimit(60);

	SceneManager* MasterP = new SceneManager();

	//main game loop
	while (g_WINDOW->isOpen())
	{
		sf::Event event;
		while (g_WINDOW->pollEvent(event))
		{
			switch (event.type)
			{
				//key press events
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					g_WINDOW->close();
				}
				break;
			}
		}

		//find time delta -- in seconds
		deltaTime = clock.restart().asSeconds();

		//update code here:
		//_GM->PreUpdate(deltaTime);
		//_GM->Update(deltaTime);
		//_GM->PostUpdate(deltaTime);

		MasterP->PreUpdate(deltaTime);
		MasterP->Update(deltaTime);


		//clear displayed window
		g_WINDOW->clear(g_BackBuffer_Color);

		//draw code here:
		MasterP->Draw();

		//draw the backbuffer
		g_WINDOW->display();
	}
}