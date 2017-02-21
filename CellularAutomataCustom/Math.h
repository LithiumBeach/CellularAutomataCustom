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
};