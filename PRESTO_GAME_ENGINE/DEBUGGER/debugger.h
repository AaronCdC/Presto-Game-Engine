#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__


#include "instructionset.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <windows.h>
#include <gl/gl.h>
#include <d3d9.h>

//#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_windows.h"
#include "allegro5/allegro_direct3d.h"

#define info_type_message "Log"
#define info_type_warning "Warning"
#define info_type_error "Error"

class GameDebugger {
private:
    SYSTEM_INFO SystemInfo;
    FILE* OutputFile;
    unsigned long long int fc1;
    unsigned long long int fc2;
	int fframe;
public:
	InstructionSet cpuInstInfo;
    GameDebugger();
    unsigned long long int pMemory;
    MEMORYSTATUS myMemory;
    void GetSystemSpecs(ALLEGRO_DISPLAY* mydisplay);
    bool IsD3D(ALLEGRO_DISPLAY* mydisplay);
    void GetGLInfo(ALLEGRO_DISPLAY* mydisplay);
    void GetD3DInfo(ALLEGRO_DISPLAY* mydisplay);
    struct tm * GetDateAndTime();
    void Log(const char* message, const char* itype, ...);
    void CountFrame();
    ~GameDebugger();
};

#endif // __DEBUGGER_H__
