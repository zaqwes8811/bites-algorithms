// #include "g_abstractions.h"
#ifndef GLOBAL_ABSTRACTIONS
#define GLOBAL_ABSTRACTIONS
	//typedef UCHAR uchar;
	// абстракция к winapi
	typedef void (__stdcall * atom_lock)(LPCRITICAL_SECTION);
	typedef void (__stdcall * atom_unlock)(LPCRITICAL_SECTION);
	typedef void (__stdcall * init_locker)(LPCRITICAL_SECTION);
	typedef void (__stdcall * free_locker)(LPCRITICAL_SECTION);
#endif