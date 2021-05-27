#include "Terrain.h"
#include "Define.h"
#include "lib/GeometricPrimitive.h"

Terrain::Terrain(int charaID) : Actor(charaID, Actor::TERRAIN)
{
	
}

void Terrain::Initialize()
{
	//SetScale(Vector3(0.01f, 0.01f, 0.01f));
	Actor::UpdateWorld();
}