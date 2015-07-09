#include "core.h"

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP *background;
ALLEGRO_EVENT_QUEUE *ev_queue;
ALLEGRO_TIMER *maintimer;
ALLEGRO_AUDIO_STREAM *music;
ALLEGRO_EVENT events;
GameDebugger MainDebugger;

 ALLEGRO_BITMAP* test;

bool exitprogram = false;
bool redraw = false;

bool AudioFail = false;
bool ImageFail = false;

void ResourceLoading()
{
	test = al_load_bitmap("TEST_ASSETS\\nep.png");
	if(!test)
		MainDebugger.Log("Error loading test asset.","Error");
	return;
}

void ev_listener()
{
	bool eventTrigger = false;
	
	while(!exitprogram)
	{

				//Wait for an event to occur
	eventTrigger = al_get_next_event(ev_queue, &events);
			/* REGION - EVENT HANDLER - */

	if(eventTrigger)
	{
		switch(events.type)
		{
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			exitprogram = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			switch(events.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
						//Scape key: exit program
					exitprogram = 1;
				break;
					/* Add more keyboard events here */

					/* End Keyboard Event Handler Region*/
			}
			break;
		}
	}else{
			/* ANIMATION UPDATES */
	}

			/* End Event Handler Region */

	/*--------------- GAME LOGIC HERE ----------------------------------------------*/

			/* REGION - DRAW ON SCREEN - */
			if(redraw && al_event_queue_is_empty(ev_queue)) {
				//Clear Screen
				al_set_target_bitmap(al_get_backbuffer(display));
				al_clear_to_color(al_map_rgb(64,64,128));
				/* DRAWING ROUTINES HERE */
	/*-------------------------------------------------------------------------------*/
			//Draw draw and draw more stuff
				al_set_target_bitmap(al_get_backbuffer(display));
				al_draw_bitmap(test, 0, 0, 0);

				//Swap buffers
				al_flip_display();
				//Clear redraw flag and wait for next frame
				redraw = false;
				MainDebugger.CountFrame();
			}
			/* End Draw On Screen Region*/

	}
	MainDebugger.Log("Last frame count.", info_type_message);
	return;
}

void game_end()
{
		//Dispose all objects (bitmaps, timers, sounds, etc)
	al_destroy_timer(maintimer);
	if(_ENABLE_SOUND_ && !AudioFail){
		al_detach_audio_stream(music);
		al_drain_audio_stream(music);
	}
	if(!AudioFail){al_destroy_audio_stream(music);}
	al_destroy_display(display);
	/* End Game Finalization Region */
	//End game
    MainDebugger.Log("End of the program.", info_type_message);
	return;
}

int main(int args, char** argv)
{
	srand (time(NULL));

	if(!al_init()) {
        MainDebugger.Log("Failed to initialize Allegro! (Null pointer returned by al_init function).", info_type_error);
		al_show_native_message_box(NULL, "Error", "Allegro Init", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	/* GAME SYSTEM SETUP */
	//Install the keyboard
	al_install_keyboard();
	//Starts the image addon
	al_init_image_addon();

	if(_ENABLE_MOUSE_) {
		//Installs the mouse
		al_install_mouse();
	}

	if(_ENABLE_SOUND_) {
		//Installs the audio system
		al_install_audio();
		//Initialize audio codecs
		al_init_acodec_addon();
		//Reserve 1 sample for initialization
		al_reserve_samples(1);
	}

	if(_FULLSCREEN_)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(_SCREEN_W_, _SCREEN_H_);

	if(!display) {
        MainDebugger.Log("Failed to create a display! (Null pointer returned by al_create_display function).", info_type_error);
		al_show_native_message_box(NULL, "Error", "Display Init", "Failed to create a display! (Null pointer returned by al_create_display function).", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	//Sets the title of the window
	al_set_window_title(display, _DISPLAYNAME_);

	MainDebugger.GetSystemSpecs(display);

		//Create an event queue
	ev_queue = al_create_event_queue();

	MainDebugger.Log("Presto Game Engine Version %i . %i", info_type_message, ENGINEVERSION__MAJOR_, ENGINEVERSION__MINOR_);
#if _DEBUG
	MainDebugger.Log("DEBUG flag is enabled.",info_type_message);
#endif

		/* EVENT SYSTEM SETUP */
	//Creates the main game timer
	maintimer = al_create_timer(1.0f/_DESIRED_FPS_);
	//Start the main game timer
	al_start_timer(maintimer);
	//Register main events in the main event queue
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	al_register_event_source(ev_queue, al_get_timer_event_source(maintimer));
	al_register_event_source(ev_queue, al_get_keyboard_event_source());

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	//End

	ResourceLoading();
	ev_listener();
	game_end();
	return 0;
}