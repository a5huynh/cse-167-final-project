#pragma once

#include <vector>

#include "BasicMath.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace std;

class Bezier
{
	int nDivSeg;
	int nSegs;
	vector<Vector3*> *controlPoints;

	vector<Vector3*> vertices;
	vector<Vector3*> tangents;

	void genCurve(void);

public:
	Bezier(vector<Vector3*> *ctrlPts, int nDiv);
	~Bezier(void);

	void render(void);
	vector<Vector3*> * getVertices(void);
	vector<Vector3*> * getTangents(void);
};
