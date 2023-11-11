//procGen.h
#pragma once
#include "../ew/mesh.h"
namespace ab {

	ew::MeshData createTorus(int ringDraw, int numSegments, float inRadius, float outRadius);
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);

}
