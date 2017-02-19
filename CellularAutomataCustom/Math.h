/*Math.h generated on Sat 02/18/2017 at 20:42:59.39*/
#include <SFML\System.hpp>

using namespace sf;
class Math
{
public:
	static sf::Vector2f ScalarMul(Vector2f A, Vector2f B)
	{
		return sf::Vector2f(A.x * B.x, A.y * B.y);
	}
};
 
