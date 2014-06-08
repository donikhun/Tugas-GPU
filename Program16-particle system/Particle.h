#ifndef PARTICLE_H_
typedef struct
{
	float lifetime; // total lifetime of the particle
	float decay; // decay speed of the particle
	float r, g, b; // color values of the particle
	float xpos, ypos, zpos; // position of the particle
	float xspeed, yspeed, zspeed; // speed of the particle
	boolean active; // is particle active or not?
} PARTICLE;
void CreateParticle(int i);
void InitParticle();
void EvolveParticle();
#endif