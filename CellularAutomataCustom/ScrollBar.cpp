#include "ScrollBar.h"
#include "Math.h"
 
ScrollBar::ScrollBar()
{
}
const float c_OutlineSize = 2;
ScrollBar::ScrollBar(Vector2f a_TopCenter, Vector2f a_BottomCenter, float a_Width, float a_TargetOverflowSize, Color a_FillColor, Color a_OutlineColor)
{
	m_BG = RShape(Vector2f(a_Width, abs(a_BottomCenter.y - a_TopCenter.y)));
	m_BG.setOrigin(Vector2f(m_BG.getSize().x * 0.5f, m_BG.getSize().y * 0.5f));

	//pos = midpoint
	centerpos = Vector2f(Math::Lerp(a_TopCenter, a_BottomCenter, .5f));
	m_BG.setPosition(centerpos);

	m_BG.setFillColor(a_FillColor);
	m_BG.setOutlineColor(a_OutlineColor);
	m_BG.setOutlineThickness(c_OutlineSize);



	m_OnSliderDown_ptr = std::bind(&ScrollBar::OnSliderDown, this);
	m_OnSliderUp_ptr = std::bind(&ScrollBar::OnSliderUp, this);

	m_TopCenter = a_TopCenter;
	m_BottomCenter = a_BottomCenter;

	m_BGHeight = (abs(a_TopCenter.y - a_BottomCenter.y));

	m_SliderHeight = m_BGHeight - (c_OutlineSize * 2.0f);
	
	UpperYBound = centerpos.y - (c_OutlineSize) + (m_SliderHeight * .5f);
	LowerYBound = centerpos.y + (c_OutlineSize) - (m_SliderHeight * .5f);

	m_Slider = new Button(	Vector2f(a_Width, m_SliderHeight), //size
							centerpos, //pos
							a_OutlineColor,//fill color
							a_OutlineColor,//outline color
							*(caFonts::s_DefaultFont),
							c_OutlineSize);
	m_Slider->SetLeftMouseButtonPressEvent(m_OnSliderDown_ptr);
	m_Slider->SetLeftMouseButtonReleaseEvent(m_OnSliderUp_ptr);

	m_Slider->SetPosition(centerpos);

	m_IsScrolling = false;

	m_TargetOverflowSize = a_TargetOverflowSize;
	//UpdateTargetSize(a_TargetOverflowSize);
	//m_TargetSize = a_TargetOverflowSize;
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::OnSliderDown()
{
	//printf("%d down", this);
	if (m_SliderHeight < m_BGHeight - (2*c_OutlineSize))
	{
		m_IsScrolling = true;
	}
}

void ScrollBar::OnSliderUp()
{
	//printf("%d up", this);
	m_IsScrolling = false;
}

bool isScrollingForAFrame = false;
const float scrollSpeed = 0.05f;
void ScrollBar::Update(float a_DeltaTime)
{

	if (m_IsScrolling || isScrollingForAFrame)
	{
		Vector2f sliderPos = m_Slider->GetPosition();
		m_Slider->SetPosition(Vector2f(centerpos.x, Math::Clamp((float)sf::Mouse::getPosition(*g_WINDOW).y, LowerYBound, UpperYBound)));
		isScrollingForAFrame = false;
	}

	if (abs(input::MouseWheelDelta) > 0.0f && m_SliderHeight < (m_BGHeight - 2.0f * c_OutlineSize))
	{
		m_Slider->SetPosition(Vector2f(centerpos.x, Math::Clamp(m_Slider->GetPosition().y + (-input::MouseWheelDelta * (m_SliderHeight)) * scrollSpeed, LowerYBound, UpperYBound)));
	}

	m_Slider->Update(a_DeltaTime);
}

void ScrollBar::UpdateTargetSize(float newSize)
{
	m_TargetSize = newSize;
	float sliderSizeRatio = newSize > m_TargetOverflowSize ? (m_TargetOverflowSize / newSize) : 1;

	if (sliderSizeRatio < .95f)
	{
		float prevSliderHeight = m_SliderHeight;

		m_SliderHeight = (sliderSizeRatio * m_BGHeight) + (c_OutlineSize * 2.0f);

		float diff = prevSliderHeight - m_SliderHeight;

		m_Slider->SetSize(Vector2f(m_Slider->GetSize().x, m_SliderHeight));

		UpperYBound = centerpos.y - c_OutlineSize + m_BGHeight * .5f - m_SliderHeight * .5f;
		LowerYBound = centerpos.y + c_OutlineSize - m_BGHeight * .5f + m_SliderHeight * .5f;

		Vector2f sliderPos = Vector2f(centerpos.x, Math::Lerp(LowerYBound, UpperYBound, GetRatio()) - diff / 2.0f);
		m_Slider->SetPosition(sliderPos);
	}
}

float ScrollBar::GetRatio()
{
	if (LowerYBound == UpperYBound)
	{
		return 0;
	}
	return Math::InverseLerp(LowerYBound, UpperYBound, m_Slider->GetPosition().y);
}

void ScrollBar::ScrollForAFrame()
{
	isScrollingForAFrame = true;
}

void ScrollBar::Draw()
{
	g_WINDOW->draw(m_BG);
	m_Slider->Draw();
}

