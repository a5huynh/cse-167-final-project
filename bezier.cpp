#include "Bezier.h"
#include <GLUT/glut.h>
#include <iostream>

Bezier::Bezier(vector<Vector3*> *ctrlPts, int nDiv)
{
	nDivSeg = nDiv;
	nSegs = (ctrlPts->size() - 1) / 3;
	controlPoints = ctrlPts;

	if (nSegs*3 + 1 != ctrlPts->size()) {
		cerr << "\nERROR CREATING BEZIER CURVE - Invalid number of control points" << endl;
		return;
	}

	genCurve();
}

Bezier::~Bezier(void)
{
	vertices.clear();
}

void Bezier::genCurve(void) {
	float t;

	Vector4 tVec;
	Vector4 dtVec;
	Vector4 xt;
	Vector4 dxt;

	Matrix4 cMat;

	// Initialize Berstein matrix
	Matrix4 bMat = Matrix4();
	bMat.set(0, 0, -1);
	bMat.set(0, 1,  3);
	bMat.set(0, 2, -3);
	bMat.set(0, 3,  1);
	bMat.set(1, 0,  3);
	bMat.set(1, 1, -6);
	bMat.set(1, 2,  3);
	bMat.set(2, 0, -3);
	bMat.set(2, 1,  3);
	bMat.set(3, 0,  1);

	// w value of each point is 1
	Matrix4 gMat = Matrix4();
	gMat.set(3, 0, 1);
	gMat.set(3, 1, 1);
	gMat.set(3, 2, 1);
	gMat.set(3, 3, 1);

	for (int seg = 0; seg < nSegs; ++seg) {
		// Load control points into G matrix
		gMat.set(0, 0, controlPoints->at(seg*3 + 0)->getX());
		gMat.set(1, 0, controlPoints->at(seg*3 + 0)->getY());
		gMat.set(2, 0, controlPoints->at(seg*3 + 0)->getZ());
		gMat.set(0, 1, controlPoints->at(seg*3 + 1)->getX());
		gMat.set(1, 1, controlPoints->at(seg*3 + 1)->getY());
		gMat.set(2, 1, controlPoints->at(seg*3 + 1)->getZ());
		gMat.set(0, 2, controlPoints->at(seg*3 + 2)->getX());
		gMat.set(1, 2, controlPoints->at(seg*3 + 2)->getY());
		gMat.set(2, 2, controlPoints->at(seg*3 + 2)->getZ());
		gMat.set(0, 3, controlPoints->at(seg*3 + 3)->getX());
		gMat.set(1, 3, controlPoints->at(seg*3 + 3)->getY());
		gMat.set(2, 3, controlPoints->at(seg*3 + 3)->getZ());
		
		cMat.multiply( gMat, bMat );

		for (int n = 0; n < nDivSeg; ++n) {
			t = n * (1.f/nDivSeg);
			tVec = Vector4(t*t*t, t*t, t, 1);
			dtVec = Vector4(3*t*t, 2*t, 1, 0);
			
			cMat.multiply( tVec, xt );
			cMat.multiply( dtVec, dxt );

			vertices.push_back(new Vector3( xt.getX(), xt.getY(), xt.getZ() ) );
			tangents.push_back(new Vector3( dxt.getX(), xt.getY(), xt.getZ() ) );						
		}
	}
	t = 1.f;
	tVec = Vector4(t*t*t, t*t, t, 1);
	dtVec = Vector4(3*t*t, 2*t, 1, 0);
	
	cMat.multiply( tVec, xt );
	cMat.multiply( dtVec, dxt );

	vertices.push_back(new Vector3(xt.getX(), xt.getY(), xt.getZ()));
	tangents.push_back(new Vector3(dxt.getX(), dxt.getY(), dxt.getZ()));
}

void Bezier::render(void) {
	vector<Vector3*>::iterator i;

	glColor3f(1, 0, 0);

	glBegin(GL_LINE_STRIP);
	for (i = vertices.begin(); i < vertices.end(); ++i) {
		Vector3 *v = *i;
		glVertex3f(v->getX(), v->getY(), v->getZ());
	}
	glEnd();
}

vector<Vector3*> * Bezier::getVertices(void) {
	return &vertices;
}

vector<Vector3*> * Bezier::getTangents(void) {
	return &tangents;
}
