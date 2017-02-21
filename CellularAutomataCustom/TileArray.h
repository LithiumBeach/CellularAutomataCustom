/*TileArray.h generated on Thu 02/16/2017 at 19:32:39.78*/
#include <vector>

class TileArray
{
public:
	TileArray();
	TileArray(unsigned int sizeX, unsigned int sizeY);
	TileArray(unsigned int size){ TileArray(size, size); }
	~TileArray();

public:
	void PrintContents();

	//get at cartesian coordinates
	unsigned int At(int x, int y);
	void Set(int x, int y, int value);

	int GetWidth(){ return m_TileInts[0].size(); }
	int GetHeight(){ return m_TileInts.size(); }

private:
	//this class is only concerned with managing an array of integers. nothing about drawing, or any other crap in here.
	std::vector<std::vector<int>> m_TileInts;


};

