#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GLUT/glut.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <algorithm>

#include "BasicMath.h"
#include "Vector3.h"
#include "trackball.h"
#include "shader.h"
#include "ppmLoader.h"

#include "Camera.h"
#include "Frustum.h"

#include "Model3D.h"
#include "Bezier.h"
#include "SceneGraph.h"
#include "Transforms.h"

#include "Terrain.h"
#include "ParticleTerrain.h"
#include "Tree.h"
#include "Planet.h"
#include "Universe.h"
#include "Moon.h"

using namespace std;

float WIN_WIDTH		= 1024.0;
float WIN_HEIGHT	= 768.0;

TransformGroup *world;
Shader *shader;
Planet *p;
Plane* frustum;
Tree* tree;

Model3D *penguin;
int pengIndice;

Translate *pengPoint;
Rotate *pengUp;
Rotate *pengDir;
Vector3 pengNormal;
Vector3 pengLoc;
Vector3 pengFront;
Vector3 pengFrontNormal;
int dir = 2;

Universe *universe;
Moon *moon;

Camera *camera;
Frustum *frust;

float ambient[4]  = { 0.1, 0.1, 0.1, 1.0 };
float diffuse[4]  = { 1, 1, 1, 1.0 };
float specular[4] = { 1, 1, 1, 1.0 };
float shininess[1] = { 30.0 };
float position[4] = {0.0, 200.0, 200.0, 1.0 };

int frames = 0;
clock_t start;

GLuint hatchingTex;
GLuint hatchingTex2;
GLuint bumpMap;

vector<Vector3*> *cameraCtrlPts, *lookatCtrlPts;
Bezier *cameraBez, *lookatBez;
vector<Vector3*> *cameraPath, *cameraTangent, *lookatPath;
float cameraPlay = 0;

bool followPenguin = false;
bool usePlanetGravity = false;
bool renderCurve = false;
bool useShader = true;
GLuint outlinesUniformLocation;
bool outlinesOn = true;
GLuint texturesUniformLocation;
bool texturesOn = true;

void load1DTexture( GLuint &tid, const char *texName) {
    FILE *in = 0;
    char line[255];
    float shaderData[32][3];
	
	in = fopen(texName, "r");
    if ( in != NULL ) {
        for (int i = 0; i < 32; ++i) {
            if (feof(in))
                break;
            fgets(line, 255, in);
            shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = atof(line);
        }
        fclose(in);
    }
    else {
        cerr << "COULD NOT LOAD " << texName << endl;
        exit(1);
    }
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_1D, tid);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, 32, 0, GL_RGB, GL_FLOAT, shaderData);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void initBezierCurve() {
    int pathRes = 200;

    cameraCtrlPts = new vector<Vector3*>();
    lookatCtrlPts = new vector<Vector3*>();

    // starting point
    cameraCtrlPts->push_back(new Vector3(0, 50, 790));        lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    cameraCtrlPts->push_back(new Vector3(0, 50, 400));        lookatCtrlPts->push_back(new Vector3(20, 10, 20));
    cameraCtrlPts->push_back(new Vector3(100, 20, 200));    lookatCtrlPts->push_back(new Vector3(180, 90, 180));
    cameraCtrlPts->push_back(new Vector3(150, 80, 150));    lookatCtrlPts->push_back(new Vector3(200, 100, 200));

    // bright side of moon
    cameraCtrlPts->push_back(new Vector3(200, 120, 100));    lookatCtrlPts->push_back(new Vector3(200, 100, 200));
    cameraCtrlPts->push_back(new Vector3(300, 150, 100));    lookatCtrlPts->push_back(new Vector3(200, 100, 200));
    cameraCtrlPts->push_back(new Vector3(250, 120, 250));    lookatCtrlPts->push_back(new Vector3(200, 100, 200));
    // dark side of moon
    cameraCtrlPts->push_back(new Vector3(200, 90, 400));    lookatCtrlPts->push_back(new Vector3(180, 90, 180));
    cameraCtrlPts->push_back(new Vector3(200, 200, 100));    lookatCtrlPts->push_back(new Vector3(20, 10, 20));
    cameraCtrlPts->push_back(new Vector3(200, 200, 0));        lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    // top right of planet
    cameraCtrlPts->push_back(new Vector3(200, 200, -100));    lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    cameraCtrlPts->push_back(new Vector3(80, 80, -200));    lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    cameraCtrlPts->push_back(new Vector3(0, 0, -200));        lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    // back of planet
    cameraCtrlPts->push_back(new Vector3(-80, -80, -200));    lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    cameraCtrlPts->push_back(new Vector3(-200, -200, -100));lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    cameraCtrlPts->push_back(new Vector3(-200, -200, 0));    lookatCtrlPts->push_back(new Vector3(0, 0, 0));
    // bottom left of planet
    cameraCtrlPts->push_back(new Vector3(-200, -200, 100));    lookatCtrlPts->push_back(new Vector3(0, 10, 0));
    cameraCtrlPts->push_back(new Vector3(-60, -60, 200));    lookatCtrlPts->push_back(new Vector3(0, 90, 60));
    cameraCtrlPts->push_back(new Vector3(0, 0, 100));        lookatCtrlPts->push_back(new Vector3(0, 100, 80));

    cameraBez = new Bezier(cameraCtrlPts, pathRes);
    cameraPath = cameraBez->getVertices();
    cameraTangent = cameraBez->getTangents();

    lookatBez = new Bezier(lookatCtrlPts, pathRes);
    lookatPath = lookatBez->getVertices();
}

void load2DTexture( GLuint& tid, const char* file ) {
	glGenTextures( 1, &tid);
	int w,h;
	unsigned char * texture = loadPPM( file, w, h );
	glGenTextures( 1, &tid );
	glBindTexture( GL_TEXTURE_2D, tid );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	delete texture;
}

void setupShader() {

	GLuint hatch1Tex, hatch2Tex, hatch3Tex, hatch4Tex, edgerampTex;
	
	load2DTexture( hatch1Tex, "brush1.ppm" );
	load2DTexture( hatch2Tex, "brush2.ppm" );
	load2DTexture( hatch3Tex, "brush3.ppm" );
	load2DTexture( hatch4Tex, "brush4.ppm" );
	
	load1DTexture( edgerampTex, "edgeramp.txt" );
	
	// Load up pixel shader
	shader = new Shader("../../toon.vert", "../../toon.frag", true );
	shader->bind();
		
    int hatch1SamplerUniformLocation = glGetUniformLocationARB(shader->pid, "hatch1");
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, hatch1Tex);
    glUniform1iARB(hatch1SamplerUniformLocation, 1);

    int hatch2SamplerUniformLocation = glGetUniformLocationARB(shader->pid, "hatch2");
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, hatch2Tex);
    glUniform1iARB(hatch2SamplerUniformLocation, 2);

    int hatch3SamplerUniformLocation = glGetUniformLocationARB(shader->pid, "hatch3");
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, hatch3Tex);
    glUniform1iARB(hatch3SamplerUniformLocation, 3);

    int hatch4SamplerUniformLocation = glGetUniformLocationARB(shader->pid, "hatch4");
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, hatch4Tex);
    glUniform1iARB(hatch4SamplerUniformLocation, 4);

    int edgeSamplerUniformLocation = glGetUniformLocationARB(shader->pid, "edgeramp");
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_1D, edgerampTex);
    glUniform1iARB(edgeSamplerUniformLocation, 5);

    int windowWidthUniformLocation = glGetUniformLocationARB(shader->pid, "windowWidth");
    glUniform1iARB(windowWidthUniformLocation, WIN_WIDTH );

    int windowHeightUniformLocation = glGetUniformLocationARB(shader->pid, "windowHeight");
    glUniform1iARB(windowHeightUniformLocation, WIN_HEIGHT );

    outlinesUniformLocation = glGetUniformLocationARB(shader->pid, "outlinesOn");
    glUniform1iARB(outlinesUniformLocation, outlinesOn );

	texturesUniformLocation = glGetUniformLocationARB(shader->pid, "texturesOn");
    glUniform1iARB(texturesUniformLocation, texturesOn );
}

float getFaceAngle( Vector3& face, Vector3& test ) {
	face.normalize();
	
	Matrix4 *rot = &Matrix4::makeRotateArbitrary( deg2rad( pengUp->getAngle() ), 
												pengUp->getX(), 
												pengUp->getY(), 
												pengUp->getZ() );									
	Vector3 *tmp = &rot->multiply( face );
	tmp->normalize();
	
	test.setX( tmp->getX() );
	test.setY( tmp->getY() );
	test.setZ( tmp->getZ() );
	
	float res = acos( tmp->dot( pengFront ) );

	delete tmp;	
	delete rot;
	
	return res;
}

void createSceneGraph() {
	// Create world scene graph
	world = new TransformGroup( new Translate( 0, 0, 0 ) );
	
	TransformGroup *planet = new TransformGroup( new Translate( 0, 0, 0 ) );
	planet->addNode( new Shape3D( p ) );
	
	vector<Vector3*> vertices = *p->getVertices();
	vector<Vector3*> normals = *p->getNormals();

	float rangle = 0;	
	Vector3 axis;
	srand( 0 );
	int treeCount = 0;
	
	Vector3 closeNorm, last = Vector3( 0, 10, 100 );
	float dist = 9999;	
	for( int i = 0; i < vertices.size(); i++ ) {
		
		Vector3 up = Vector3( 0, 1, 0 );
		Vector3 *v = vertices[ i ];
		Vector3 *n = normals[ i ];
		
		float tmpDist = sqrt( pow( v->getX() - last.getX(), 2 ) + pow( v->getY() - last.getY(), 2 ) + 
						pow( v->getZ() - last.getZ(), 2 ) );
						
		if( dist > tmpDist ) {
			dist = tmpDist;
			pengLoc.setX( v->getX() );
			pengLoc.setY( v->getY() );
			pengLoc.setZ( v->getZ() );
			pengNormal.setX( n->getX() );
			pengNormal.setY( n->getY() );
			pengNormal.setZ( n->getZ() );
			pengIndice = i;
		}
		
		if( (rand() % 20) > 1 )
			continue;		
		
		float h = v->magnitude() - 100;
		
		if( h < -2.0 || h > 4.0 )
			continue;

		// Axis of rotation
		axis.cross( up, *n );
		axis.normalize();
		
		// Angle of rotation
		rangle = acos( up.dot( *n ) );

		TransformGroup *treeNode = new TransformGroup( new Translate( v->getX(), v->getY(), v->getZ() ) );
		TransformGroup *treeScale = new TransformGroup( new Scale( .2, .2, .2 ) );
		TransformGroup *treeRotate = new TransformGroup( 
											new Rotate( rad2deg(rangle), axis.getX(), 
													axis.getY(), axis.getZ() ) );
							
		treeRotate->addNode( new Shape3D( tree ) );
		treeScale->addNode( treeRotate );
		treeNode->addNode( treeScale );
	
		planet->addNode( treeNode );
		treeCount++;
	}
	
	Vector3 up = Vector3( 0, 1, 0 );
	// Axis of rotation
	axis.cross( up, pengNormal );
	axis.normalize();

	// Angle of rotation
	rangle = acos( up.dot( pengNormal ) );
	
	pengPoint = new Translate( pengLoc.getX(), pengLoc.getY(), pengLoc.getZ() );
	pengUp    = new Rotate( rad2deg( rangle ), axis.getX(), axis.getY(), axis.getZ() );
	pengFront = Vector3( axis );
	pengFrontNormal = Vector3( axis );
	
	//Vector3 face = Vector3( 1, 0, -1 );
	pengDir   = new Rotate( 100, 0, 1, 0 );

	TransformGroup *peng = new TransformGroup( pengPoint );
	TransformGroup *pengScale = new TransformGroup( new Scale( 3, 3, 3 ) );
	TransformGroup *pengRotate = new TransformGroup( pengUp  );
	TransformGroup *pengTrans = new TransformGroup( new Translate( 0, .5, 0 ) );
	TransformGroup *pengRotate2 = new TransformGroup( pengDir );	
											
	pengRotate2->addNode( new Shape3D( penguin ) );
	pengTrans->addNode( pengRotate2 );
	pengRotate->addNode( pengTrans );
	pengScale->addNode( pengRotate );
	peng->addNode( pengScale );
	
	planet->addNode( peng );
	
	printf("Tree Count: %d\n", treeCount );
	world->addNode( planet );	
}

void reshape(int width, int height ) {
	
	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION );
	
	glLoadIdentity();
		
	// set perspective projection viewing frustum
	gluPerspective( frust->getFOV(), frust->getAspect(), 
						frust->getNear(), frust->getFar() + 100 );

	frust->generateFrustumPlanes( camera, frustum );
}

float angle = 0;
void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	
	gluLookAt( camera->getCenterOfProjection().getX(),
				camera->getCenterOfProjection().getY(),
				camera->getCenterOfProjection().getZ(),
				camera->getLookAtPoint().getX(),
				camera->getLookAtPoint().getY(),
				camera->getLookAtPoint().getZ(),
				camera->getUp().getX(),
				camera->getUp().getY(),
				camera->getUp().getZ() );
	
	glLightfv( GL_LIGHT0, GL_POSITION,		position );
	
	glUseProgram( 0 );
		
	universe->draw();
		
	glPushMatrix();
		glTranslatef( 200, 100, 200 );
		moon->draw();
	glPopMatrix();
	
	if( renderCurve ) {
		glLineWidth( 10 );
		cameraBez->render();
	}
	
	if( useShader )
		shader->bind();
		
	world->draw( camera, &frustum );
	
	glFlush();	
	glutSwapBuffers();
} 

void idle() {

	static float position = 0;

	long end = (clock() - start);

	if( end/CLOCKS_PER_SEC > 1 ) {
		printf( "FPS: %d\n", frames );
		frames = 0;
		start = clock();
	}
	
	frames++;
	
	if ( !followPenguin ) {	
		position = min(cameraPath->size() - 1.0f, max(0.0f, position + cameraPlay));

		float percentComplete = position / (cameraPath->size()-1);

		if (cameraPlay != 0) {
			camera->setCenterOfProjection( *cameraPath->at( (int)position ) );
			camera->setLookAtPoint( *lookatPath->at( (int)position ) );			
		}

		if (percentComplete == 0 || percentComplete == 1) {
			cameraPlay = 0;	
		}
	} else {
		// Center camera on penguin.
		Vector3 t = Vector3( pengFrontNormal );
		t.scale( -15 );
		t.add( t, pengLoc );
	
		Vector3 up = Vector3( pengNormal );
		up.scale( 10 );
		t.add( t, up );
		
		Vector3 t2 = Vector3( pengLoc );
		up.scale( .5 );
		t2.add( t2, up );
		
		camera->setCenterOfProjection( t );
		camera->setUp( pengNormal );
		camera->setLookAtPoint( t2 );
		frust->generateFrustumPlanes( camera, frustum );
	}
	
	display();
}

void right( int d ) {

	Vector3 up = Vector3( 0, 1, 0 );	
		
	Vector3 front;
	front = Vector3( 0, 0, -1 );
	
//	switch( d ) {
//		case 1: front = Vector3( 0, 0, -1 ); break;
//		case 3: front = Vector3( -1, 0, 0 ); break;
//		case 4: front = Vector3( 1, 0, 0 ); break;
//		case 6: front = Vector3( 0, 0, 1 ); break;
//	}
	
	// Axis of rotation
	Vector3 axis;
	axis.cross( up, pengNormal );
	axis.normalize();
	
	// Angle of rotation
	float rangle = acos( up.dot( pengNormal ) );
	
	pengUp->set( rad2deg( rangle ), axis.getX(), axis.getY(), axis.getZ() );
	pengDir->set( 90 + rad2deg( getFaceAngle( front, pengFrontNormal ) ), 0, 1, 0 );
}

int getDir() {												
	Vector3 t = Vector3( pengFront );
	return 1;
}

void keyboard( unsigned char key, int x, int y ) {

	float moveScale = 1;
	Vector3 lookDir, left, up;
	Vector3 tmp, tmp2;
			
	lookDir.sub( camera->getLookAtPoint(), camera->getCenterOfProjection() );
	up = Vector3( camera->getUp() );

	left.cross( up, lookDir );
	
	lookDir.normalize();
	lookDir.scale(moveScale);
	left.normalize();
	left.scale(moveScale);
	up.normalize();
	up.scale(moveScale);

	if( key == 'g' ) {
		useShader = !useShader;
		if( useShader ) {
			glEnable( GL_LIGHTING );
			glEnable( GL_COLOR_MATERIAL );
			glEnable( GL_TEXTURE_2D );
		} else {
			glDisable( GL_LIGHTING );
			glDisable( GL_COLOR_MATERIAL );
			glDisable( GL_TEXTURE_2D );
		}
	} else if ( key == 'n' ) {
		p->toggleNormals();
	} else if ( key == 'w' ) {
		if ( !followPenguin ) {
			tmp.add( camera->getCenterOfProjection(), lookDir );
			tmp2.add( camera->getLookAtPoint(), lookDir );
			camera->setCenterOfProjection( tmp );
			camera->setLookAtPoint( tmp2 );
		} else {
			p->nextPoint( pengIndice, getDir(), pengLoc, pengNormal );
			pengPoint->set( pengLoc.getX(), pengLoc.getY(), pengLoc.getZ() );		
			right( 1 );
		}
	} else if ( key == 's' ) {
		if( !followPenguin ) {
			tmp.sub( camera->getCenterOfProjection(), lookDir );
			tmp2.sub( camera->getLookAtPoint(), lookDir );
			camera->setCenterOfProjection( tmp );
			camera->setLookAtPoint( tmp2 );
		} else {
			p->nextPoint( pengIndice, 6, pengLoc, pengNormal );
			pengPoint->set( pengLoc.getX(), pengLoc.getY(), pengLoc.getZ() );		
			right( 6 );
		}
	} else if ( key == 'a' ) {
		if( !followPenguin ) {
			tmp.add( camera->getCenterOfProjection(), left );
			tmp2.add( camera->getLookAtPoint(), left );
			camera->setCenterOfProjection( tmp );
			camera->setLookAtPoint( tmp2 );
		} else  {
			p->nextPoint( pengIndice, 3, pengLoc, pengNormal );
			pengPoint->set( pengLoc.getX(), pengLoc.getY(), pengLoc.getZ() );		
			right( 3 );
		}
	} else if ( key == 'd' ) {
		if( !followPenguin ) {
			tmp.sub( camera->getCenterOfProjection(), left );
			tmp2.sub( camera->getLookAtPoint(), left );
			camera->setCenterOfProjection( tmp );
			camera->setLookAtPoint( tmp2 );
		} else {
			Vector3 save = Vector3( pengLoc );
			p->nextPoint( pengIndice, 4, pengLoc, pengNormal );
			pengPoint->set( pengLoc.getX(), pengLoc.getY(), pengLoc.getZ() );		
			right( 4 );
		}
	} else if ( key =='q' ) {
	} else if ( key == ',' ) { 
		cameraPlay--;
	} else if ( key == '.' ) {
		cameraPlay++;
	} else if ( key == '/' ) {
		if( cameraPlay == 0 ) {
			cameraPlay = 1;
		} else {
			cameraPlay = 0;
		}
	} else if ( key == 'p' ) {
		p->toggleVertices();
	} else if ( key == 'c' ) {
		renderCurve = !renderCurve;
	} else if ( key =='`' ) {
		followPenguin = !followPenguin;
	} else if ( key =='o' ) {
		outlinesOn = !outlinesOn;
		glUniform1iARB(outlinesUniformLocation, outlinesOn );
	} else if ( key == 't' ) {
		texturesOn = !texturesOn;
		glUniform1iARB(texturesUniformLocation, texturesOn );
	} else if ( key == 'r' ) {
		delete world;
		delete p;
		
		world = NULL;
		p = NULL;
		
		srand( 139041390 );
		p = new Planet( rand() % 1000 );
		tree = new Tree();
		createSceneGraph();
	} else if ( key == 'b' ) {
		moon->toggleShader();
	}
}

int main (int argc, char * argv[]) {

	initTrackBall();
	
	initBezierCurve();
	
	Vector3 eye  = Vector3( 0, 50, 790 );
	Vector3 look = Vector3( 0, 0, 0   );
	Vector3 up   = Vector3( 0, 1, 0   );
	
	frust = new Frustum( 60, WIN_WIDTH/WIN_HEIGHT, 1, 2000 );
	camera = new Camera( eye, look, up ); 
	frustum = new Plane[6];
	
	penguin = new Model3D("penguin2.obj");
			
	// initialize GLUT
	glutInit(&argc, argv);
	
	// open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   	

	// set initial window size
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );

	// open window and set window title
	glutCreateWindow("Final Project");
	
	// enable depth buffering
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_DEPTH_TEST );	
	glEnable( GL_BLEND );
	glEnable( GL_POLYGON_SMOOTH );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glPolygonMode( GL_FRONT, GL_FILL );
	glShadeModel(GL_SMOOTH);						// set shading to smooth		
		
	glClear(GL_DEPTH_BUFFER_BIT);					// clear depth buffer
	
	glLightfv( GL_LIGHT0, GL_POSITION,		position );
	glLightfv( GL_LIGHT0, GL_AMBIENT,		ambient );
	glLightfv( GL_LIGHT0, GL_SPECULAR,		specular );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,		diffuse );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	// set clear color to black
	glClearColor( 1, 1, 1, 1 );
	glEnable( GL_TEXTURE_2D );	

	setupShader();
	universe = new Universe();
	moon = new Moon( 30 );
	p = new Planet();
	tree = new Tree();
	createSceneGraph();
	
	// Install callback functions:
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( handleMouse );
	glutMotionFunc( handleRotate );
	
	start = clock();
	glutMainLoop();
		 
	return 0;	
}
