/*TileScene2D.h generated on Thu 02/16/2017 at 19:38:13.76*/
#ifndef TILESCENE2D_H
#define TILESCENE2D_H

#include "TileArray.h"
#include "Cell.h"
#include "Button.h"

#include <vector>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>

class TileScene2D
{
public:
	typedef sf::Vector2f Vector2f;
public:
	TileScene2D();
	~TileScene2D();

public:
	void PreUpdate(float a_DeltaTime);
	void Update(float a_DeltaTime);
	void Draw();

	//Rules of Life (subject to change)
private:// min/max are INCLUSIVE -- return TRUE when conditions are met
	int GetLiveNeighborsAroundIndex(int x, int y);
	bool GetBetweenMinMaxInclusive(int val, int min, int max)
	{
		return val <= max && val >= min;
	}
	bool ShouldKill(int a_NumLiveNeighbors, int a_MinLiveNeighbors, int a_MaxLiveNeighbors)
	{
		return GetBetweenMinMaxInclusive(a_NumLiveNeighbors, a_MinLiveNeighbors, a_MaxLiveNeighbors);
	}
	bool ShouldRaiseDead(int a_NumLiveNeighbors, int a_NumNeighborsNeededToRaise)
	{
		return a_NumLiveNeighbors == a_NumNeighborsNeededToRaise;
	}

private:
	std::vector<std::vector<Cell>> m_Cells;

private:
	bool m_WasMousePressed;

public:
	float TilePixelSize = 10;
	int BoardTileSize = 50;

private:
	bool m_IsSimulating = false;
	bool m_WasSpacePressed;


private:
	void InitializeUI();
	Button m_SimulateButton;
	static void HandleSimulateButtonPressEvent();
	void (*HandleSimulateButtonPressEvent_ptr)();
};

#endif