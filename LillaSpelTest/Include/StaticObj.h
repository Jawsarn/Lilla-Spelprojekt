#pragma once
#include <DirectXCollision.h>
#include <DirectXMath.h>

using namespace DirectX;

enum ObjectType{HOLE, WALL};

class StaticObj
{
private:
	ObjectType m_typeOfObject;
	BoundingOrientedBox m_box;
	
public:

	StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box);
	StaticObj(void);
	ObjectType GetType();
	BoundingOrientedBox* GetBox();
	~StaticObj(void);

};

