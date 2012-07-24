
#include "stdafx.h"

namespace random_globals
{
	unsigned int slump[55],slump1,slump2,talpos;
}

using namespace random_globals;

unsigned int random()
{
    talpos=(talpos+1)%55;
    slump[talpos]=slump[(talpos+23)%55]+slump[(talpos+54)%55];
    return slump[talpos];
}

inline unsigned int random(unsigned int i)
{
	return random()%i; 
}

inline float random(float f) 
{
	return random()*f/0xffffffff; 
}

unsigned int R_rnd()
{
	slump1=((slump1)*1821+1)%65731;
	slump2=((slump2)*1421+1)%65609;
	return((slump1<<16) ^ slump2);
}

void R_mize(unsigned int start)
{ 
	int i;
	talpos=0;
	slump1=start%65731;
	slump2=start%65609;
    for(i=0; i<99; i++) R_rnd();
	for(i=0; i<55; i++) slump[i]=R_rnd();
	for(i=0; i<99; i++) random();
}
