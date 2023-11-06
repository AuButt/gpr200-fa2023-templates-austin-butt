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
		//connect to corresponding vertex @ pole
	
		//sphere rows
		int columns, start;

		columns = numSegments + 1;

		for (row = 1; row < numSegments - 1; row++) {
			for (col = 0; col < numSegments; col++) {
				start = row * columns + col;
				//Tri 1
				result.indices.push_back(start);
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns);
				//Tri 2
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns + 1);
				result.indices.push_back(start + columns);
			}
		}

		return result;
	}

	//ew::MeshData createCylinder(float height, float radius, int numSegments) {
		
	//}
	//ew::MeshData createPlane(float width, float height, int subdivisions) {
		
	//}
}