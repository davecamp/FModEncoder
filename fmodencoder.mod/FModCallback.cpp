
#include "..\..\bah.mod\fmod.mod\include\fmod.h"
#include "IFModEncoder.h"

extern IFModEncoder* g_pEncoder = 0;
extern int g_iLock = 0;

extern"C"{
	FMOD_RESULT F_CALLBACK FModPCMCallback(FMOD_SOUND* pSound,void* data, unsigned int datalen){
		while (!__sync_bool_compare_and_swap(&g_iLock,0,1)){}

		if(g_pEncoder){
			g_pEncoder->Encode(data,datalen);
		}

		g_iLock = 0;	
		return FMOD_OK;
	}
}