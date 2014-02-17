#include "iostream"
using namespace std;

class CPoint
{
public:
	int x;
	int y;
};


int main()
{
	CPoint* pt[10];
	CPoint** pt2;
	pt2 = new CPoint*[10];
	pt2[2]  = new CPoint[4];
	pt2[2][0].x = 34;
	pt2[2][0].y = 32;
}