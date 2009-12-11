/*
 *  SceneGraph.cpp
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/11/09.
 */

#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "SceneGraph.h"

Node::Node() {
	childNodes = NULL;
	numChildren = 0;
	nodeId = 0;
}

Node::Node( int ID ) {
	childNodes = NULL;
	numChildren = 0;
	nodeId = ID;
}

Node::~Node() {
	if( !hasChildren() )
		return;
		
	for( int i = 0; i < numChildren; i++ ) {
		// delete allocated data
		delete childNodes[i];
	}
	
	// delete allocated array;
	free(childNodes);
}

int Node::getSize() {
	return numChildren;
}

bool Node::hasChildren() {
	return childNodes != NULL;
}

Node** Node::getChildren() {
	return childNodes;
}

void Node::print() {
	printf( "Node::%d\n", nodeId );
	printChildren();
}

void Node::printChildren() {
	for( int i = 0; i < numChildren; i++ ) {
		childNodes[i]->print();
	}
}

Group::Group() : Node() {}
Group::Group( int nodeId ) : Node( nodeId ) {}

void Group::addNode( Node* node ) {

	// If the list is empty, create new one on the heap
	if( childNodes == NULL ) {
		childNodes = (Node**)calloc( 1, sizeof( Node * ) );
		childNodes[0] = node;
		numChildren++;
	
	// If the list has already been created, reallocate space
	} else {
		numChildren++;
		childNodes = (Node**)realloc( childNodes, (numChildren) * sizeof(Node*) );
		childNodes[numChildren-1] = node;
	}
	
}

void Group::draw( Camera* cam, Plane** planes ) {
	for( int i = 0; i < getSize(); i++ ) {
		childNodes[i]->draw( cam, planes );
	}
}

void Group::print() {
	printf( "Group::%d\n", this->nodeId );
	printChildren();
}

TransformGroup::TransformGroup( Transform* m ) {
	transform = m;
	compiled = false;
}

TransformGroup::~TransformGroup() {
	delete transform;
}

void TransformGroup::compile() {
	dList = glGenLists(1);
	
	glNewList( dList, GL_COMPILE );
		draw( NULL, NULL );
	glEndList();
	compiled = true;
}

void TransformGroup::draw( Camera* cam, Plane** planes ) {
	if( compiled ) {
		glCallList( dList );
	} else {
		glPushMatrix();
		transform->apply();
		for( int i = 0; i < getSize(); i++ ) {
			childNodes[i]->draw( cam, planes );
		}
		glPopMatrix();
	}
}

void TransformGroup::print() {
	printChildren();
}

Shape3D::Shape3D( Object3D* obj ) { object = obj; }
Shape3D::~Shape3D() { }
void Shape3D::print() { object->print(); }

void Shape3D::draw( Camera* cam, Plane** planes ) {

	if( object == NULL )
		return;

	// Modify center of bounding sphere to adhere to matrix transformation.
	if( object->getBoundingSphere() != NULL ) {
		
		// Occlusion culling
		Matrix4 matrix = Matrix4();
		glGetFloatv( GL_MODELVIEW_MATRIX, matrix.getPointer() );
		
		Vector3 p1 = Vector3( -100, 100, 0 );
		Vector3 p2 = Vector3( -100, -100, 0 );
		Vector3 p3 = Vector3( 100, -100, 0 );
		Vector3 p4 = Vector3( 100, 100, 0 );
		
		Vector3 c = Vector3( 0, 0, 0 );
		Vector4 *tmp, *result;
		
		Plane test = Plane();
		test.setPoint( 0, 0, 0 );
		
		Vector3 n = Vector3( cam->getCenterOfProjection() );
		n.normalize();
		
		test.setNormal( n.getX(), n.getY(), n.getZ() );
		test.setNormal( 0, 0, 1 );
		test.setWidth( 200 );
		test.setHeight( 200 );
		
		BoundingSphere *p = object->getBoundingSphere();
		c = p->getCenter();
		
		tmp = new Vector4( c.getX(), c.getY(), c.getZ(), 1 );
		result = &matrix.multiply( *tmp );
		result->dehomogenize();
		
		c.setX( result->get( 0 ) );
		c.setY( result->get( 1 ) );
		c.setZ( result->get( 2 ) );
	
		// Clean up
		delete result;
		delete tmp;
		
		float dist = test.distance( c );
		if ( dist < -( cam->getDistFromOrigin() + 5 ) )
			return;
			
		// Frustum culling
		if( planes != NULL ) {
			for( int i = 0; i < 6; i++ ) {
				dist = (*planes)[i].distance( c );
				if( dist > 0 )
					return;
			}
		}
	}
	
	// Object level culling
	object->draw();
}
