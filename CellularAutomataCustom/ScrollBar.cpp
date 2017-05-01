#include "ScrollBar.h"
#include "Math.h"
 
ScrollBar::ScrollBar()
{
}
const float c_OutlineSize = 2;
ScrollBar::ScrollBar(Vector2f a_TopCenter, Vector2f a_BottomCenter, float a_Width, float a_TargetOverflowSize, Color a_FillColor, Color a_OutlineColor)
{
	m_BG = RShape(Vector2f(a_Width, abs(a_TopCenter.y - a_BottomCenter.y)));
	m_BG.setOrigin(Vector2f(m_BG.getSize().x * 0.5f, 0));

	//pos = midpoint
	m_BG.setPosition(Vector2f(Math::Lerp(a_TopCenter, a_BottomCenter, .5f)));

	m_BG.setFillColor(a_FillColor);
	m_BG.setOutlineColor(a_OutlineColor);
	m_BG.setOutlineThickness(c_OutlineSize);



	m_OnSliderDown_ptr = std::bind(&ScrollBar::OnSliderDown, this);
	m_OnSliderUp_ptr = std::bind(&ScrollBar::OnSliderUp, this);

	m_BGHeight = (abs(a_TopCenter.y - a_BottomCenter.y));
	m_SliderHeight = m_BGHeight - (c_OutlineSize * 2.0f);

	m_TopCenter = a_TopCenter;
	m_BottomCenter = a_BottomCenter;
	
	UpperYBound = a_TopCenter.y + (c_OutlineSize)+m_BGHeight * .5f + m_SliderHeight * .5f;
	LowerYBound = a_BottomCenter.y - (c_OutlineSize)+m_BGHeight * .5f - m_SliderHeight * .5f;

	m_Slider = new Button(Vector2f(a_Width, m_SliderHeight), //size
							Vector2f(m_BG.getPosition().x, UpperYBound), //pos
							a_OutlineColor,//fill color
							a_OutlineColor,//outline color
							*(caFonts::s_DefaultFont),
							c_OutlineSize);
	m_Slider->SetLeftMouseButtonPressEvent(m_OnSliderDown_ptr);
	m_Slider->SetLeftMouseButtonReleaseEvent(m_OnSliderUp_ptr);

	//m_Slider->setOrigin(Vector2f(m_Slider->getSize().x * 0.5f, m_Slider->getSize().y * 0.5f));

	m_IsScrolling = false;

	m_TargetOverflowSize = a_TargetOverflowSize;
	m_TargetSize = a_TargetOverflowSize;
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::OnSliderDown()
{
	//printf("%d down", this);
	m_IsScrolling = true;
}

void ScrollBar::OnSliderUp()
{
	//printf("%d up", this);
	m_IsScrolling = false;
}

void ScrollBar::Update(float a_DeltaTime)
{
	if (m_IsScrolling)
	{
		Vector2f sliderPos = m_Slider->GetPosition();
		m_Slider->SetPosition(Vector2f(m_Slider->GetPosition().x, Math::Clamp((float)sf::Mouse::getPosition(*g_WINDOW).y, UpperYBound, LowerYBound)));
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


		UpperYBound = m_TopCenter.y + (c_OutlineSize)+m_BGHeight * .5f + m_SliderHeight * .5f;
		LowerYBound = m_BottomCenter.y - (c_OutlineSize)+m_BGHeight * .5f - m_SliderHeight * .5f;

		Vector2f sliderPos = m_Slider->GetPosition();
		m_Slider->SetPosition(Vector2f(m_Slider->GetPosition().x, UpperYBound));
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

void ScrollBar::Draw()
{
	g_WINDOW->draw(m_BG);
	m_Slider->Draw();
}

