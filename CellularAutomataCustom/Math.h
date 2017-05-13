/*Math.h generated on Sat 02/18/2017 at 20:42:59.39*/
#include <SFML/System.hpp>

#include <sstream>
#include <iomanip>

class Math
{
public:
	static sf::Vector2f ScalarMul(sf::Vector2f A, sf::Vector2f B)
	{
		return sf::Vector2f(A.x * B.x, A.y * B.y);
	}

	template<typename T>
	static T mod(T a, T a_mod)
	{
		T t = a % a_mod;
		return t >= 0 ? t : t + a_mod;
	}
	
	template<typename T, typename T1, typename T2>
	static bool AABB(sf::Vector2<T> a_TopLeft, sf::Vector2<T1> a_BottomRight, sf::Vector2<T2> a_Point)
	{//assume y axis is 0 at top, increasing downwards
		return	a_Point.x > a_TopLeft.x &&
				a_Point.y > a_TopLeft.y &&
				a_Point.x < a_BottomRight.x &&
				a_Point.y < a_BottomRight.y;
				
	}

	template<typename T>
	static T Max(T a, T b)
	{
		return a >= b ? a : b;
	}
	template<typename T>
	static T Min(T a, T b)
	{
		return a <= b ? a : b;
	}

	template <typename T>
	static std::string to_string_with_precision(const T a_value, const int n = 0)
	{
		std::ostringstream out;
		out << std::setprecision(n) << a_value;
		return out.str();
	}

	template <typename T>
	static T Clamp(T val, T min, T max)
	{
		if (val < min) val = min;
		if (val > max) val = max;
		return val;
	}

	template <typename T>
	static T Lerp(T a, T b, float t)
	{
		return (a + t*(b - a));
	}
	static sf::Vector2f Lerp(sf::Vector2f a, sf::Vector2f b, float t)
	{
		return sf::Vector2f(a.x + t*(b.x - a.x), a.y + t*(b.y - a.y));
	}

	template <typename T>
	static float InverseLerp(T a, T b, T v)
	{
		return (v - a) / (b - a);
		//return a + (1.0 / ((b - a) * v));
	}
};
