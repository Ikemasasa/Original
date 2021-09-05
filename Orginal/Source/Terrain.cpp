#include "Terrain.h"

#include "CollisionTerrain.h"

Terrain::Terrain(int charaID) : Object(charaID)
{
	
}

void Terrain::Initialize()
{
	//SetScale(Vector3(0.01f, 0.01f, 0.01f));
	Object::UpdateWorld();

	CollisionTerrain::RegisterTerrain(this);
}