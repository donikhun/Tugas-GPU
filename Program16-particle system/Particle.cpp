#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include "particle.h"
const int maxparticle = 2000; // set maximum number of particles
PARTICLE particle[maxparticle];
void CreateParticle(int i)
{
	particle[i].lifetime = (float)(500000 * rand() / RAND_MAX) / 500000.0;
	particle[i].decay = 0.001;
	particle[i].r = 0.7;
	particle[i].g = 0.7;
	particle[i].b = 1.0;
	particle[i].xpos = 0.0;
	particle[i].ypos = 0.0;
	particle[i].zpos = 0.0;
	particle[i].xspeed = 10 * (0.0005 - (float)(100 * rand() / RAND_MAX) / 100000.0);
	particle[i].yspeed = 0.01 - (float)(100 * rand() / RAND_MAX) / 100000.0;
	particle[i].zspeed = 0.0005 - (float)(100 * rand() / RAND_MAX) / 100000.0;
	particle[i].active = true;
}
//---------------------------------------------------------------------------
void InitParticle()
{
	for (int i = 0; i <= maxparticle; i++){ // initialize the particleparameters
			CreateParticle(i);
		particle[i].active = false; // set all particles inactive
	}
}
//---------------------------------------------------------------------------
void EvolveParticle()
{
	for (int i = 0; i <= maxparticle; i++){ // evolve the particle parameters
		particle[i].lifetime -= particle[i].decay;
		particle[i].xpos += particle[i].xspeed;
		particle[i].ypos += particle[i].yspeed;
		particle[i].zpos += particle[i].zspeed;
		particle[i].yspeed -= 0.00010;
	}
}