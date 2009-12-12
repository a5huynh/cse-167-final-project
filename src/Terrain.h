/*
 *  Terrain.h
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/4/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

class Terrain {
	public:
		Terrain( int size );
		~Terrain();
		
		void generate( int, float, float, float );
		
		float* getHeightMap();
		float getData( int, int );
		
		int getWidth();
		
	private:
		float *heightMap;
		int width;
		float roughness;		// Roughness of generated height map ( more hills, mountains, etc )
		float k;				// smoothing constant
		
		void genTerrain( int, int, int, int, float );
		void smooth();
		
		float genDisp( float );
		
		void setData( int, int, float );
};