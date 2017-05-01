/*TileScene2D.h generated on Thu 02/16/2017 at 19:38:13.76*/
#pragma once

#include "TileArray.h"
#include "Cell.h"
#include "Rule.h"
#include "ScrollBar.h"

#include <vector>
#include <functional>
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
	void PreUpdateUnmanaged();
	void Update(float a_DeltaTime);
	void UpdateUnmanaged(float a_DeltaTime);
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
	bool m_WasLeftMousePressed;
	bool m_WasRightMousePressed;

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
	void IntializeRules();

	Button m_SimulateButton;
	Button m_IncreasePlaybackSpeedButton;
	Button m_DecreasePlaybackSpeedButton;

	//really just text
	Button m_FPSButton;

	Button m_NextFrameButton;

	Button m_ClearButton;
	Button m_ClearColorButton;
	int m_ClearColorIndex;
public:
	void HandleSimulateButtonReleaseEvent();
	std::function<void()> HandleSimulateButtonReleaseEvent_ptr;

	std::function<void()> DoNothing_ptr;
	void DoNothing();

	//Playback Speed + / -
	void HandlePlaybackSpeedIncButtonReleaseEvent();
	std::function<void()> HandlePlaybackSpeedIncButtonReleaseEvent_ptr;
	void HandlePlaybackSpeedDecButtonReleaseEvent();
	std::function<void()> HandlePlaybackSpeedDecButtonReleaseEvent_ptr;
	
	//Next/Previous Frame
	void HandleNextFrameButtonReleaseEvent();
	std::function<void()> HandleNextFrameButtonReleaseEvent_ptr;

	//clear to color
	void HandleClearEvent();
	std::function<void()> HandleClearEvent_ptr;
	void HandleClearColorIncrementEvent();
	std::function<void()> HandleClearColorIncrementEvent_ptr;
	void HandleClearColorDecrementEvent();
	std::function<void()> HandleClearColorDecrementEvent_ptr;

private:
	void UpdateFPSCounter();
public:
	float FramesPerSecond;

public:
	int m_PlaybackSpeedsLen;
	int playbackSpeedsIndex;
	float* m_PlaybackSpeeds;

	//RULE INTERFACE
public:
	std::vector<Rule*>* m_Rules;
	//add later.
	//std::vector<Button> m_RemoveRuleButtons;

	Button m_AddRuleButton;
	void HandleAddRuleEvent();
	std::function<void()> HandleAddRuleEvent_ptr;

private:
	void ProcessRulesAt(int x, int y);

	//SCROLLIN SCROLLIN SCROLLIN
	//https://www.youtube.com/watch?v=qCRae5mRoRE
	ScrollBar* m_RuleScrollBar;

	void UpdateRuleScrolling();

private:
	sf::RectangleShape m_ControlsBG;
};

