/*TileScene2D.h generated on Thu 02/16/2017 at 19:38:13.76*/
#ifndef TILESCENE2D_H
#define TILESCENE2D_H

#include "TileArray.h"
#include "Cell.h"
#include "Button.h"

#include <vector>
#include <functional>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Graphics\Font.hpp>

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

public:
	bool m_IsSimulating;
	bool m_IsPaused;
private:
	bool m_WasSpacePressed;

private:
	//Interface elements
	void InitializeUI();
	Button m_SimulateButton;
	Button m_IncreasePlaybackSpeedButton;
	Button m_DecreasePlaybackSpeedButton;

	//really just text
	Button m_FPSButton;

	Button m_NextFrameButton;
public:
	void HandleSimulateButtonPressEvent();
	std::function<void()> HandleSimulateButtonPressEvent_ptr;

	std::function<void()> DoNothing_ptr;
	void DoNothing();

	//Playback Speed + / -
	void HandlePlaybackSpeedIncButtonPressEvent();
	std::function<void()> HandlePlaybackSpeedIncButtonPressEvent_ptr;
	void HandlePlaybackSpeedDecButtonPressEvent();
	std::function<void()> HandlePlaybackSpeedDecButtonPressEvent_ptr;
	
	//Next/Previous Frame
	void HandleNextFrameButtonPressEvent();
	std::function<void()> HandleNextFrameButtonPressEvent_ptr;

private:
	void UpdateFPSCounter();
public:
	float FramesPerSecond;
	sf::Font* m_DefaultFont;

public:
	int m_PlaybackSpeedsLen;
	int playbackSpeedsIndex;
	float* m_PlaybackSpeeds;
};

#endif