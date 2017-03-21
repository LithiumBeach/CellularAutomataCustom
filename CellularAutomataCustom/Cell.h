/*Cell.h generated on Sat 02/18/2017 at 18:35:16.57*/

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

#include "Math.h"

class Cell
{
public:
	Cell(){};
	Cell(bool a_IsAlive, sf::Vector2f a_TilePos, sf::Vector2f a_PixelSize);

	~Cell();

public:
	void Update(double a_DeltaTime);
	void Draw();

private:
	sf::Vector2f m_TilePos;
	bool m_IsAlive;
	bool m_WillBeAliveOnUpdate;
	sf::RectangleShape m_RectangleShape;
public:
	sf::Vector2f GetTilePos(){ return m_TilePos; }
	bool IsAlive(){ return m_IsAlive; }
	//pre-update
	void SetAliveNextFrame(bool a_val){ m_WillBeAliveOnUpdate = a_val; }
	//update
	void SetAliveImmediate(bool a_val){ m_IsAlive = a_val; UpdateFillColor(); }
	void ToggleAliveImmediate(){ m_IsAlive = !m_IsAlive; UpdateFillColor(); }

private:
	void UpdateFillColor(){ m_RectangleShape.setFillColor(m_IsAlive ? ALIVE_COLOR : DEAD_COLOR); }

public:
	void SetFillColorDirect(sf::Color a_color = sf::Color::Magenta){ m_RectangleShape.setFillColor(a_color); }

public:
	sf::Color ALIVE_COLOR = sf::Color::Black;
	sf::Color DEAD_COLOR = sf::Color::White;
};