#pragma once

#include <DirectXMath.h>

enum Type{DYNAMIC,STATIC,PLAYER};


struct Box
{
public:
	Type type;
	DirectX::XMFLOAT3 corners[4];

};