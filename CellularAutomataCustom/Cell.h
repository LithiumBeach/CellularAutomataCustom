/*Cell.h generated on Sat 02/18/2017 at 18:35:16.57*/

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

#include "Math.h"

class Cell
{
public:
	typedef sf::Vector2f Vector2f;
	typedef sf::Color Color;
	typedef sf::RectangleShape RShape;

public:
	Cell(){};
	Cell(int a_ColorIndex, Vector2f a_TilePos, Vector2f a_PixelSize);

	~Cell();

public:
	void Update(double a_DeltaTime);
	void Draw();

private:
	RShape m_RShape;
	Vector2f m_TilePos;
public:
	Vector2f GetTilePos(){ return m_TilePos; }

public:
	int m_ColorIndexOnUpdate;
	//pre-update
	void SetColorIndexNextFrame(int a_ColorIndex);
	//update
	//void SetAliveImmediate(int a_ColorIndex);
	void AdvanceAliveImmediate();
	void ReverseAdvanceAliveImmediate();

public:
	void SetColorIndexImmediate(int a_ColorIndex);

private:
	int m_ColorIndex;
	int m_NextFrameColorIndex;

public:
	int GetColorIndex(){ return m_ColorIndex; }
};