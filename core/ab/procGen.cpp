#include "procGen.h"
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/vec3.h"

namespace ab {
	ew::MeshData createTorus(int ringDraw, int numSegments, float inRadius, float outRadius) {
		ew::Vertex vert;
		ew::MeshData result;

		//VERTEX DATA
		//ring radius
		float phi = 0.0;
		float dp = (2 * ew::PI) / numSegments;
		//torus rad
		float theta = 0.0;
		float dt = (2 * ew::PI) / ringDraw;
		// i col j row
		for (int i = 0; i < ringDraw; i++) {
			theta = dt * i;

			for (int j = 0; j < numSegments; j++) {
				phi = dp * j;
				vert.pos.x = cos(theta) * (outRadius + cos(phi) * inRadius);
				vert.pos.y = sin(theta) * (outRadius + cos(phi) * inRadius);
				vert.pos.z = sin(phi) * inRadius;
				vert.normal = ew::Vec3(vert.pos.x, vert.pos.y, vert.pos.z);	//NORMALS
				vert.uv = ew::Vec2(i, j);	//UV
				result.vertices.push_back(vert);
			}
		}
		
		//INDICIES
		int i1, i2, i3, i4;
		for (int i = 0; i < ringDraw - 1; i++) {
			
			for (int j = 0; j < numSegments; j++) {
				i1 = i + (j * numSegments);
				i2 = (i + 1) + (j * numSegments);
				i3 = i + ((j + 1) * numSegments);
				i4 = (i + 1) + ((j + 1) * numSegments);
				result.indices.push_back(i1);
				result.indices.push_back(i3);
				result.indices.push_back(i4);

				result.indices.push_back(i1);
				result.indices.push_back(i4);
				result.indices.push_back(i2);
			}
		}

		return result;
	}

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
				vert.normal = ew::Vec3(vert.pos.x, vert.pos.y, vert.pos.z);	//NORMALS
				vert.uv = ew::Vec2(col, row);	//UV
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
		poleStart = result.vertices.size() - numSegments;
		sideStart = poleStart - numSegments - 2;

		for (int i = 0; i < numSegments; i++) {
			result.indices.push_back(sideStart + i);
			result.indices.push_back(sideStart + i + 1);
			result.indices.push_back(poleStart + i); //pole
		}

		//sphere rows indicies
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

	//int bottomcover = meshData.verticies.size() - 1 //index of last vertex
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::Vertex vert;
		ew::MeshData result;
		float topY, bottomY, thetaStep, theta;

		//Vertices
		topY = height / 2.0;	// y = 0; centered
		bottomY = -topY;
		//top center
		vert.pos = ew::Vec3(0, topY, 0);
		result.vertices.push_back(vert);
		//top ring
		thetaStep = 2.0 * ew::PI / numSegments;
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			vert.pos.x = cos(theta) * radius;
			vert.pos.z = sin(theta) * radius;
			vert.pos.y = topY;
			result.vertices.push_back(vert);
		}
		//bottom ring
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			vert.pos.x = cos(theta) * radius;
			vert.pos.z = sin(theta) * radius;
			vert.pos.y = bottomY;
			result.vertices.push_back(vert);
		}
		//bottom center
		vert.pos = ew::Vec3(0, bottomY, 0);
		result.vertices.push_back(vert);

		//Cap Vertices
		int start, center;
		//top
		start = 0;
		center = 0;

		for (int i = 0; i <= numSegments; i++) {
			result.indices.push_back(start + i);
			result.indices.push_back(center);
			result.indices.push_back(start + i + 1);
		}

		//bottom
		start = numSegments + 2;
		center = result.vertices.size() - 1;

		for (int i = 0; i <= numSegments; i++) {
			result.indices.push_back(start + i);
			result.indices.push_back(start + i + 1);
			result.indices.push_back(center);
		}

		//Indicies
		int sideStart, columns;

		sideStart = 0;
		columns = numSegments + 1;

		for (int i = 0; i < columns; i++) {
			start = sideStart + i;
			//Tri 1
				result.indices.push_back(start);
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns);
				//Tri 2
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns + 1);
				result.indices.push_back(start + columns);
		}
		return result;
	}

	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::Vertex vert;
		ew::MeshData result;
		int row, col, columns, start;

		vert.normal = ew::Vec3{ +0.0f,+1.0f,+0.0f };	//NORMAL

		//Vertices 
		for (row = 0; row <= subdivisions; row++) {

			for (col = 0; col <= subdivisions; col++) {
				vert.pos.x = width * (col / subdivisions);
				vert.pos.z = -height * (row / subdivisions);
				vert.uv = ew::Vec2(col, row);	//UV
				result.vertices.push_back(vert);
			}
		}
		//Indicies
		columns = subdivisions + 1;
		for (row = 0; row < subdivisions; row++) {
			
			for (col = 0; col < subdivisions; col++) {
				start = row * columns + col;
				//bot right
				result.indices.push_back(start);
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns);
				//top left
				result.indices.push_back(start + 1);
				result.indices.push_back(start + columns + 1);
				result.indices.push_back(start + columns);
			}
		}

		return result;
	}
}