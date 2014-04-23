#pragma once
#include "object.h"

enum ObjectType{HOLE, WALL};

class StaticObj
{
private:
	ObjectType m_typeOfObject;
	BoundingOrientedBox m_box;
	
public:

	StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box);
	StaticObj(void);
	~StaticObj(void);

};

