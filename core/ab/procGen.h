//procGen.h
#pragma once
#include "../ew/mesh.h"
namespace ab {

	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);

}
