#include "Terrain.h"

#include "CollisionTerrain.h"

Terrain::Terrain(int charaID) : Object(charaID)
{

}

void Terrain::Initialize()
{
	Object::UpdateWorld();

	CollisionTerrain::RegisterTerrain(this);
}