#include "debugger.h"
#include "instructionset.h"
#include <stdarg.h>

#ifndef I64u
#define I64u llu    //Standard C99 support. HAX
#endif // I64u

extern "C"
WINBASEAPI BOOL WINAPI GetPhysicallyInstalledSystemMemory (PULONGLONG );

GameDebugger::GameDebugger(){
    OutputFile = fopen("dev_log.txt", "w+");
    fc1 = 0;
    fc2 = 0;
	fframe = -1;
}

void GameDebugger::GetSystemSpecs(ALLEGRO_DISPLAY* mydisplay)
{
    GetSystemInfo(&SystemInfo);
    struct tm* mytime = GetDateAndTime();

        fprintf(OutputFile, "-- START OF LOG FILE --\nTIMESTAMP: %02i/%02i/%02i - %02i:%02i:%02i\n\n",1+mytime->tm_mday, 1+mytime->tm_mon,
            1900+mytime->tm_year, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);

    GetPhysicallyInstalledSystemMemory(&pMemory);
    myMemory.dwLength = sizeof(myMemory);
    GlobalMemoryStatus(&myMemory);

    fprintf(OutputFile, "-- SYSTEM SPECS --\n\n");
    printf("-- SYSTEM SPECS --\n\n");

    fprintf(OutputFile, "CPU: %s\n", cpuInstInfo.cpusignature);
    printf("CPU: %s\n", cpuInstInfo.cpusignature);

    printf("Cores: %i\n", SystemInfo.dwNumberOfProcessors);
    fprintf(OutputFile, "Cores: %i\n", SystemInfo.dwNumberOfProcessors);

    if(cpuInstInfo.PFLAGS.PYSICAL_ADDRESS_E)
    {
        fprintf(OutputFile, "Physical Address Extension (PAE) Available (x86_64)\n");
        printf("Physical Address Extension (PAE) Available (x86_64)\n");
    }

    fprintf(OutputFile, "Other processor extensions: ");
    printf("Other processor extensions: ");

    if(cpuInstInfo.PFLAGS.LONGMODE)
    {
        fprintf(OutputFile, "lm ");
        printf("lm ");
    }

    if(cpuInstInfo.PFLAGS.MMX_INSTRUCTIONS)
    {
        fprintf(OutputFile, "mmx ");
        printf("mmx ");
    }

    if(cpuInstInfo.PFLAGS.EXT_MMX)
    {
        fprintf(OutputFile, "mmxext ");
        printf("mmxext ");
    }

    if(cpuInstInfo.PFLAGS.B3DNOW)
    {
        fprintf(OutputFile, "3dnow ");
        printf("3dnow ");
    }

    if(cpuInstInfo.PFLAGS.EXT_3DNOW)
    {
        fprintf(OutputFile, "3dnowext ");
        printf("3dnowext ");
    }

    if(cpuInstInfo.PFLAGS.MULTIPROCESSOR_CAPABLE)
    {
        fprintf(OutputFile, "mp ");
        printf("mp ");
    }

    fprintf(OutputFile, "\n\n -- RAM INFORMATION -- \n\n");
    printf("\n\n -- RAM INFORMATION -- \n\n");

    printf("Physical RAM: %I64u Mb\n",pMemory/1024);
    fprintf(OutputFile, "Physical RAM: %I64u Mb\n",pMemory/1024);

    printf("Available RAM: %i Mb\n",myMemory.dwAvailPhys / 1024 / 1024);
    fprintf(OutputFile, "Available RAM: %i Mb\n",myMemory.dwAvailPhys / 1024 / 1024);

    printf("Total PageFile: %i Mb\n",myMemory.dwTotalPageFile / 1024 / 1024);
    fprintf(OutputFile, "Total PageFile: %i Mb\n",myMemory.dwTotalPageFile / 1024 / 1024);

    printf("Available PageFile: %i Mb\n",myMemory.dwAvailPageFile / 1024 / 1024);
    fprintf(OutputFile, "Available PageFile: %i Mb\n",myMemory.dwAvailPageFile / 1024 / 1024);

    printf("Total Virtual Memory: %i Mb\n",myMemory.dwTotalVirtual / 1024 / 1024);
    fprintf(OutputFile, "Total Virtual Memory: %i Mb\n",myMemory.dwTotalVirtual / 1024 / 1024);

    printf("Available Virtual Memory: %i Mb\n",myMemory.dwAvailVirtual / 1024 / 1024);
    fprintf(OutputFile, "Available Virtual Memory: %i Mb\n",myMemory.dwAvailVirtual / 1024 / 1024);

	if(IsD3D(mydisplay))
	{
		printf("Using Direct 3D\n");
		fprintf(OutputFile, "Using Direct 3D\n");
		GetD3DInfo(mydisplay);
		
	}else{
		printf("Using OpenGL 3D\n");
		fprintf(OutputFile, "Using OpenGL\n");
		GetGLInfo(mydisplay);
	}

    return;
}

bool GameDebugger::IsD3D(ALLEGRO_DISPLAY* mydisplay)
{
    IDirect3DDevice9 * whatisthis = al_get_d3d_device(mydisplay);
    if(!whatisthis)
        return false;


    HRESULT isDirect3D = NULL;
    isDirect3D = whatisthis->SetRenderState(D3DRS_LIGHTING,false);

    if(isDirect3D == D3D_OK)
        return true;
    return false;
}

void GameDebugger::GetGLInfo(ALLEGRO_DISPLAY* mydisplay)
{
    fprintf(OutputFile, "\n\n -- Display Information -- \n\n");
    printf("\n\n -- Display Information -- \n\n");
    char* glrinfo = NULL;
    HWND winHandle = al_get_win_window_handle(mydisplay);
    HDC myAdapter = GetDC(winHandle);

    //HGLRC hRC = wglCreateContext(myAdapter);
    HGLRC hRC = wglGetCurrentContext();
    if(!wglMakeCurrent(myAdapter, hRC))
    {
        Log("Could not retrieve OpenGL information!\nHWND: 0x%0x HDC: 0x%0x HGLRC: 0x%0x \n\n", info_type_error, winHandle, myAdapter, hRC);
        return;
    }

    glrinfo = (char*)glGetString(GL_RENDERER);
    fprintf(OutputFile, "GPU Vendor: %s\n", glrinfo);
    printf("GPU Vendor: %s\n", glrinfo);
    fprintf(OutputFile, "Renderer: OpenGL\n\n");
    printf("Renderer: OpenGL\n\n");

    //wglDeleteContext(hRC);
    return;
}

void GameDebugger::GetD3DInfo(ALLEGRO_DISPLAY* mydisplay)
{
    fprintf(OutputFile, "\n\n -- Display Information -- \n\n");
    printf("\n\n -- Display Information -- \n\n");

    IDirect3DDevice9 * whatisthis = al_get_d3d_device(mydisplay);
    IDirect3D9* MyDirect3D;
    whatisthis->GetDirect3D(&MyDirect3D);
    D3DADAPTER_IDENTIFIER9 GraphicAdapter;
    MyDirect3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &GraphicAdapter);


    //printf("%s %s %s", GraphicAdapter.Driver, GraphicAdapter.DeviceName, GraphicAdapter.Description);

    fprintf(OutputFile, "Display Device: %s\n", GraphicAdapter.DeviceName);
    printf("Display Device: %s\n", GraphicAdapter.DeviceName);

    fprintf(OutputFile, "GPU Name: %s\n", GraphicAdapter.Description);
    printf("GPU Name: %s\n", GraphicAdapter.Description);

    fprintf(OutputFile, "Driver: %s\n", GraphicAdapter.Driver);
    printf("Driver: %s\n", GraphicAdapter.Driver);

    fprintf(OutputFile, "Renderer: Direct3D\n\n");
    printf("Renderer: Direct3D\n\n");
    return;
}

struct tm * GameDebugger::GetDateAndTime()
{
    time_t t = time(0);
    return localtime(&t);
};

void GameDebugger::Log(const char* message, const char* itype, ...)
{
    struct tm* mytime = GetDateAndTime();
    printf("%02i:%02i:%02i f %I64u%I64u | %s > ",mytime->tm_hour, mytime->tm_min, mytime->tm_sec, fc1, fc2,itype);
    fprintf(OutputFile, "%02i:%02i:%02i f %I64u%I64u | %s > ", mytime->tm_hour, mytime->tm_min, mytime->tm_sec, fc1, fc2, itype);
    va_list arguments;
    va_start (arguments, itype);
    vprintf(message,arguments);
    vfprintf(OutputFile,message,arguments);
    va_end(arguments);
    printf("\n");
    fprintf(OutputFile, "\n");
    return;
}

void GameDebugger::CountFrame()
{
	if(fframe < 0)
	{
		Log("First frame rendered", info_type_message);
		fframe = 1;
	}
    fc2++;
    if(fc2 == 0)
        fc1++;
    return;
}

GameDebugger::~GameDebugger()
{
    Log("GameDebugger instance has been deleted.", info_type_warning);
    if(OutputFile)
        fclose(OutputFile);
}
