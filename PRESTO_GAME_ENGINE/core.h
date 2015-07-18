/*********************************************************************/
/*                    - PRESTO: 2D GAME ENGINE -                     */
/*                     By Aaron C.d.C (c) 2015                       */
/*                   SOME RIGHTS MAY BE RESERVED                     */
/*                                                                   */
/*    This piece of software is freeware and open source. You, under */
/*    any circumstances, shall not sell or make profit, in any way,  */
/*    of this software.                                              */
/*                                                                   */
/*    Some portions of this software rely on the ALLEGRO 5 library,  */
/*    and the OpenAL and OpenGL/Direct X libraries, as well as the	 */
/*    Windows API, in order to work. These libraries may have their	 */
/*    own license. That license must be distributed with the		 */
/*	  binaries.														 */
/*                                                                   */
/*    This software is distributed "as it is", and without any kind  */
/*    of warranty. When making changes, you should also keep this    */
/*    information within the source, and always keep the original    */
/*    license.                                                       */
/*********************************************************************/

#ifndef __CORE__H__
#define __CORE__H__

/*  DEBUG FLAG  */
/* When compiling under the Debug configuration, this flag is always ON. */
/* For the Release configuration, 1 enables it and 0 dissables it. */
/* If this is an alpha/beta release, set it to 1. */
/* If this is a stable release, set it to 0. */
#define _ENABLE_DEBUG_MODE_ 1;

//ENGINE VERSION NUMBERS. DO NOT MODIFY THIS!
#define ENGINEVERSION__MAJOR_ 0
#define ENGINEVERSION__MINOR_ 1

//DISPLAYNAME: The name to show in the window.
#define _DISPLAYNAME_ "PRESTO GAME ENGINE TESTS"

//SCREEN PARAMETERS: The window size (width and height)
#define _SCREEN_W_ 640
#define _SCREEN_H_ 480

//Full screen switch. 1 = Full screen, 0 = Window.
#define _FULLSCREEN_ 0

//COLOR DEPTH: How many bits compound a color. Must be 32.
#define _COLOR_DEPTH_ 32

//Flags to enable or dissable different drivers. If we are not
//using something, better leave it to 0.
#define _ENABLE_MOUSE_ 0
#define _ENABLE_JOYSTICK_ 0
#define _ENABLE_SOUND_ 0

//The less FPS, the more logic time, but the updates will be slower.
//Generaly, we want at least 60 FPS. But for more complex games we can
//lower it a bit, if we don't need excessive input precission (such as
//in fighting games, or shooters). Usually anything in between 30 and
// 60 should be fine, but preferable 60.
#define _DESIRED_FPS_ 60

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include "DEBUGGER\debugger.h"
#include "RESMANAGER\resmanager.h"

/* Global variables to be used in the whole program */
extern ALLEGRO_DISPLAY* display;
extern GameDebugger MainDebugger;
extern bool __DEBUG__MODE__;

#endif
