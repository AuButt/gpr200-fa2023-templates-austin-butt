#include "procGen.h"
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/vec3.h"

namespace ab {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::Vertex vert;
		ew::MeshData result;
		//vertices
		float thetaStep, phiStep, theta, phi, row, col;

		thetaStep = 2 * ew::PI / numSegments;
		phiStep = ew::PI / numSegments;

		for (row = 0; row <= numSegments; row++) {
			phi = row * phiStep;

			for (col = 0; col <= numSegments; col++) {
				theta = col * thetaStep;
				vert.pos.x = radius * cos(theta) * sin(phi);
				vert.pos.y = radius * cos(phi);
				vert.pos.z = radius * sin(theta) * sin(phi);
				result.vertices.push_back(vert);
			}
		}
		//sphere caps
		int poleStart, sideStart;

		poleStart = 0;
		sideStart = numSegments + 1;

		for (int i = 0; i < numSegments; i++) {
			result.indices.push_back(sideStart + i);
			result.indices.push_back(poleStart + i); //pole
			result.indices.push_back(sideStart + i + 1);
		}
		return result;
	}

	//ew::MeshData createCylinder(float height, float radius, int numSegments) {
		
	//}
	//ew::MeshData createPlane(float width, float height, int subdivisions) {
		
	//}
}