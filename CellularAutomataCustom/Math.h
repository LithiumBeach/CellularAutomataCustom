/*Math.h generated on Sat 02/18/2017 at 20:42:59.39*/
#include <SFML\System.hpp>

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
};