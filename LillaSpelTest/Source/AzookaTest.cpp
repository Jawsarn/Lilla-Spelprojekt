#include "AzookaTest.h"

using namespace std;
using namespace DirectX;


AzookaTest::AzookaTest(void)
{
}


AzookaTest::~AzookaTest(void)
{
}

void AzookaTest::Run()
{
	BoundingOrientedBox testBox = BoundingOrientedBox();
	BoundingOrientedBox testBox1 = BoundingOrientedBox();
	vector<XMFLOAT3> testBoxCorners;
	vector<XMFLOAT3> testBoxCorners1;
	testBoxCorners1.push_back(XMFLOAT3(0,0,0));
	testBoxCorners1.push_back(XMFLOAT3(1,0,0));
	testBoxCorners1.push_back(XMFLOAT3(0,1,0));
	testBoxCorners1.push_back(XMFLOAT3(1,1,0));

	testBoxCorners1.push_back(XMFLOAT3(0,0,1));
	testBoxCorners1.push_back(XMFLOAT3(1,0,1));
	testBoxCorners1.push_back(XMFLOAT3(0,1,1));
	testBoxCorners1.push_back(XMFLOAT3(1,1,1));



	testBoxCorners.push_back(XMFLOAT3(0,0,0));
	testBoxCorners.push_back(XMFLOAT3(1,0,0));
	testBoxCorners.push_back(XMFLOAT3(0,1,0));
	testBoxCorners.push_back(XMFLOAT3(1,1,0));
	
	testBoxCorners.push_back(XMFLOAT3(0,0,1));
	testBoxCorners.push_back(XMFLOAT3(1,0,1));
	testBoxCorners.push_back(XMFLOAT3(0,1,1));
	testBoxCorners.push_back(XMFLOAT3(1,1,1));

	testBox.CreateFromPoints(testBox, 8, &testBoxCorners[0], sizeof(XMFLOAT3));
	testBox1.CreateFromPoints(testBox1, 8, &testBoxCorners1[0], sizeof(XMFLOAT3));
	if(testBox.Intersects(testBox1))
	{
		return;
	}
}