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

//ENGINE VERSION NUMBERS.
#define ENGINEVERSION__MAJOR_ 0
#define ENGINEVERSION__MINOR_ 1

//DISPLAYNAME: The name to show in the window.
#define _DISPLAYNAME_ "PRESTO GAME ENGINE TESTS"

//SCREEN PARAMETERS: The window size (width and height)
#define _SCREEN_W_ 640
#define _SCREEN_H_ 480

#define _FULLSCREEN_ 0
//COLOR DEPTH: How many bits compound a color. Must be 32.
#define _COLOR_DEPTH_ 32

#define _ENABLE_MOUSE_ 0
#define _ENABLE_JOYSTICK_ 0
#define _ENABLE_SOUND_ 0

//The less FPS, the more logic time, but the updates will be slower.
#define _DESIRED_FPS_ 60

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include "DEBUGGER\debugger.h"

extern ALLEGRO_DISPLAY* display;
extern GameDebugger MainDebugger;

#endif