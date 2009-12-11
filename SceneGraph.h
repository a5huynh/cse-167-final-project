/*
 *  SceneGraph.h
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/11/09.
 */
#pragma once

#include "Matrix4.h"
#include "Object3D.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include "Transforms.h"
#include "Camera.h"

class Node {
		
	public:
		Node();
		Node( int );
		virtual ~Node();
	
		Node ** getChildren();
		bool hasChildren();
		int getSize();
		
		virtual void draw( Camera*, Plane** ) = 0;
		virtual void print();
		virtual void printChildren();
	
	protected:
		Node ** childNodes;
		int numChildren;
		int nodeId;
};

class Group : public Node {
	public:
		Group();
		Group( int );
		
		void addNode( Node* );
		bool removeNode( Node* );

		virtual void draw( Camera*, Plane** );
		virtual void print();
};

class TransformGroup : public Group {	
	public:
		TransformGroup( Transform* );
		~TransformGroup();
				
		virtual void draw( Camera*, Plane** );
		virtual void print();
		
		void compile();
		
	private:
		GLuint dList;
		bool compiled;
		Transform *transform;
};

class Leaf : public Node {
};

class Shape3D : public Leaf {
	public:
		Shape3D( Object3D* );
		~Shape3D();
		
		virtual void draw( Camera*, Plane** );
		virtual void print();
	
		struct BoundingSphere* getBoundingSphere();
			
	private:
		Object3D *object;
		BoundingSphere *bSphere;
};
