#include "debugger.h"
#include "instructionset.h"
#include <stdarg.h>

#ifndef I64u
#define I64u llu    //Standard C99 support. HAX
#endif // I64u

extern "C"
WINBASEAPI BOOL WINAPI GetPhysicallyInstalledSystemMemory (PULONGLONG );

//Class constructor.
GameDebugger::GameDebugger(){
    OutputFile = fopen("dev_log.txt", "w+");
    fc1 = 0;
    fc2 = 0;
	fframe = -1;
	lsecond = -1;
	mfps = 0;
	pfps = 0;
	//measuring_fps = false;
}

//Get the system information and print everything on screen (and in the log file)
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
    fprintf(OutputFile, "CPU: %s\n", cpuInstInfo.cpusignature);
    fprintf(OutputFile, "Cores: %i\n", SystemInfo.dwNumberOfProcessors);

#ifdef _DEBUG
	printf("-- SYSTEM SPECS --\n\n");
	printf("CPU: %s\n", cpuInstInfo.cpusignature);
	printf("Cores: %i\n", SystemInfo.dwNumberOfProcessors);
#endif

    if(cpuInstInfo.PFLAGS.PYSICAL_ADDRESS_E)
    {
        fprintf(OutputFile, "Physical Address Extension (PAE) Available (x86_64)\n");
#ifdef _DEBUG
        printf("Physical Address Extension (PAE) Available (x86_64)\n");
#endif
    }

    fprintf(OutputFile, "Other processor extensions: ");
    printf("Other processor extensions: ");

    if(cpuInstInfo.PFLAGS.LONGMODE)
    {
        fprintf(OutputFile, "lm ");
#ifdef _DEBUG
        printf("lm ");
#endif
    }

    if(cpuInstInfo.PFLAGS.MMX_INSTRUCTIONS)
    {
        fprintf(OutputFile, "mmx ");
#ifdef _DEBUG
        printf("mmx ");
#endif
    }

    if(cpuInstInfo.PFLAGS.EXT_MMX)
    {
        fprintf(OutputFile, "mmxext ");
#ifdef _DEBUG
        printf("mmxext ");
#endif
    }

    if(cpuInstInfo.PFLAGS.B3DNOW)
    {
        fprintf(OutputFile, "3dnow ");
#ifdef _DEBUG
        printf("3dnow ");
#endif
    }

    if(cpuInstInfo.PFLAGS.EXT_3DNOW)
    {
        fprintf(OutputFile, "3dnowext ");
#ifdef _DEBUG
        printf("3dnowext ");
#endif
    }

    if(cpuInstInfo.PFLAGS.MULTIPROCESSOR_CAPABLE)
    {
        fprintf(OutputFile, "mp ");
#ifdef _DEBUG
        printf("mp ");
#endif
    }

    fprintf(OutputFile, "\n\n -- RAM INFORMATION -- \n\n");
    fprintf(OutputFile, "Physical RAM: %I64u Mb\n",pMemory/1024);
    fprintf(OutputFile, "Available RAM: %i Mb\n",myMemory.dwAvailPhys / 1024 / 1024);
    fprintf(OutputFile, "Total PageFile: %i Mb\n",myMemory.dwTotalPageFile / 1024 / 1024);
    fprintf(OutputFile, "Available PageFile: %i Mb\n",myMemory.dwAvailPageFile / 1024 / 1024);
    fprintf(OutputFile, "Total Virtual Memory: %i Mb\n",myMemory.dwTotalVirtual / 1024 / 1024);
    fprintf(OutputFile, "Available Virtual Memory: %i Mb\n",myMemory.dwAvailVirtual / 1024 / 1024);

#ifdef _DEBUG
	printf("\n\n -- RAM INFORMATION -- \n\n");
	printf("Physical RAM: %I64u Mb\n",pMemory/1024);
    printf("Available RAM: %i Mb\n",myMemory.dwAvailPhys / 1024 / 1024);
    printf("Total PageFile: %i Mb\n",myMemory.dwTotalPageFile / 1024 / 1024);
	printf("Available PageFile: %i Mb\n",myMemory.dwAvailPageFile / 1024 / 1024);
    printf("Total Virtual Memory: %i Mb\n",myMemory.dwTotalVirtual / 1024 / 1024);
    printf("Available Virtual Memory: %i Mb\n",myMemory.dwAvailVirtual / 1024 / 1024);
#endif

	if(IsD3D(mydisplay))
	{
#ifdef _DEBUG
		printf("Using Direct 3D\n");
#endif
		fprintf(OutputFile, "Using Direct 3D\n");
		GetD3DInfo(mydisplay);
		
	}else{
#ifdef _DEBUG
		printf("Using OpenGL 3D\n");
#endif
		fprintf(OutputFile, "Using OpenGL\n");
		GetGLInfo(mydisplay);
	}

    return;
}

//Function to check if the renderer is Direct 3D (If not, it's OpenGL)
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

//Function to get the OpenGL information
void GameDebugger::GetGLInfo(ALLEGRO_DISPLAY* mydisplay)
{
    fprintf(OutputFile, "\n\n -- Display Information -- \n\n");
#ifdef _DEBUG
    printf("\n\n -- Display Information -- \n\n");
#endif

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
    fprintf(OutputFile, "Renderer: OpenGL\n\n");

#ifdef _DEBUG
	printf("GPU Vendor: %s\n", glrinfo);
    printf("Renderer: OpenGL\n\n");
#endif

    //wglDeleteContext(hRC);
    return;
}

//Function to get the renderer information from Direct 3D
void GameDebugger::GetD3DInfo(ALLEGRO_DISPLAY* mydisplay)
{
    fprintf(OutputFile, "\n\n -- Display Information -- \n\n");
#ifdef _DEBUG
    printf("\n\n -- Display Information -- \n\n");
#endif

    IDirect3DDevice9 * whatisthis = al_get_d3d_device(mydisplay);
    IDirect3D9* MyDirect3D;
    whatisthis->GetDirect3D(&MyDirect3D);
    D3DADAPTER_IDENTIFIER9 GraphicAdapter;
    MyDirect3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &GraphicAdapter);


    //printf("%s %s %s", GraphicAdapter.Driver, GraphicAdapter.DeviceName, GraphicAdapter.Description);

    fprintf(OutputFile, "Display Device: %s\n", GraphicAdapter.DeviceName);
    fprintf(OutputFile, "GPU Name: %s\n", GraphicAdapter.Description);
    fprintf(OutputFile, "Driver: %s\n", GraphicAdapter.Driver);
    fprintf(OutputFile, "Renderer: Direct3D\n\n");
    
#ifdef _DEBUG
	printf("Display Device: %s\n", GraphicAdapter.DeviceName);
	printf("GPU Name: %s\n", GraphicAdapter.Description);
	printf("Driver: %s\n", GraphicAdapter.Driver);
	printf("Renderer: Direct3D\n\n");
#endif

    return;
}

//Function to get the current date and time.
struct tm * GameDebugger::GetDateAndTime()
{
    time_t t = time(0);
    return localtime(&t);
};

//Function to measure FPS. Called once per frame.
int GameDebugger::MeasureFPS()
{
	struct tm* myTime = GetDateAndTime();
	if(lsecond < 0)
	{
		mfps = 0;
		lsecond = myTime->tm_sec;
		//measuring_fps = true;
		//return mfps;
	}

	/*if(!measuring_fps && lsecond != myTime->tm_sec)
	{
		measuring_fps = true;
		lsecond = myTime->tm_sec;
		return mfps;
	}*/

	if(lsecond == myTime->tm_sec)
	{
		mfps++;
	}else{
		mfps++;
		lsecond = -1;
		pfps = mfps;
		//measuring_fps = false;
	}
	return mfps;
}

//Function that return the current number of FPS.
int GameDebugger::GetFPS()
{
	return pfps;
}

//Function to add information to the log. Similar to printf.
void GameDebugger::Log(const char* message, const char* itype, ...)
{
    struct tm* mytime = GetDateAndTime();
#ifdef _DEBUG //The user is not going to see this unless this is the Debug configuration.
    printf("%02i:%02i:%02i f %I64u%I64u (%i fps) | %s > ",mytime->tm_hour, mytime->tm_min, mytime->tm_sec, fc1, fc2, pfps, itype);
#endif
    fprintf(OutputFile, "%02i:%02i:%02i f %I64u%I64u (%i fps) | %s > ", mytime->tm_hour, mytime->tm_min, mytime->tm_sec, fc1, fc2, pfps, itype);
    va_list arguments;
    va_start (arguments, itype);
    vprintf(message,arguments);
    vfprintf(OutputFile,message,arguments);
    va_end(arguments);

#ifdef _DEBUG
    printf("\n");
#endif

    fprintf(OutputFile, "\n");
    return;
}

//Function to count frames. Called once per frame.
//The total number of frames rendered is fc1*fc2.
void GameDebugger::CountFrame()
{
	MeasureFPS();
	if(fframe < 0)
	{
		//This is usefull to know when the program starts rendering.
		if(__DEBUG__MODE__)
			Log("First frame rendered", info_type_message);
		fframe = 1;
	}
    fc2++;
    if(fc2 == 0)
        fc1++;
    return;
}

//Class destructor.
GameDebugger::~GameDebugger()
{
	//We don't want to see this until the very end of the program.
    Log("-- END OF THE LOG FILE (Game Debugger has been destroyed) --", info_type_warning);
    if(OutputFile)
        fclose(OutputFile);
}
