/*ScrollBar.h generated on Sun 04/30/2017 at 16:23:49.32*/
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "StaticNamespaces.h"
#include "Button.h"

extern sf::RenderWindow* g_WINDOW;

class ScrollBar
{
public:
	typedef sf::RectangleShape RShape;
	typedef sf::Vector2f Vector2f;
	typedef sf::Color Color;

public:
	ScrollBar();
	ScrollBar(Vector2f a_TopCenter, Vector2f a_BottomCenter, float a_Width, float a_TargetOverflowSize = 400, Color a_FillColor = caColors::gray, Color a_OutlineColor = caColors::border_gray);
   ~ScrollBar();

private:
	float UpperYBound;
	float LowerYBound;

public:
   void Update(float a_DeltaTime);
   void Draw();

private:
	Button* m_Slider;
	void OnSliderDown();
	std::function<void()> m_OnSliderDown_ptr;
	void OnSliderUp();
	std::function<void()> m_OnSliderUp_ptr;

public:
	bool m_IsScrolling;

private:
	RShape m_BG;

public:
	Vector2f m_TopCenter;
	Vector2f m_BottomCenter;
	float m_BGHeight;
	float m_SliderHeight;

	//update the y size of the target region.
	void UpdateTargetSize(float newSize);
	//what is the maximum y size allowed before we decrease the size of the slider?
	float m_TargetOverflowSize;
	//how large is it now?
	float m_TargetSize;

public:
	float GetRatio();

private:
	Vector2f centerpos;

};
 
