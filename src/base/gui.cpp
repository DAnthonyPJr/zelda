//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  gui.c
//
//  System functions, input handlers, GUI stuff, etc.
//  for Zelda Classic.
//
//--------------------------------------------------------

#include "precompiled.h" //always first

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <dir.h>
#include <ctype.h>
#include "base/zc_alleg.h"

#ifdef ALLEGRO_DOS
#include <unistd.h>
#endif

#include "base/zdefs.h"
#include "zelda.h"
#include "tiles.h"
#include "base/colors.h"
#include "pal.h"
#include "base/zsys.h"
#include "qst.h"
#include "zc_sys.h"
#include "debug.h"
#include "jwin.h"
#include "base/jwinfsel.h"
#include "base/gui.h"

extern int32_t zq_screen_w, zq_screen_h;

void broadcast_dialog_message(DIALOG* dialog, int32_t msg, int32_t c)
{
	while(dialog->proc)
	{
		object_message(dialog++, msg, c);
	}
}

/****************************/
/**********  GUI  ***********/
/****************************/

// make it global so the joystick button routine can set joy_on=TRUE
DIALOG_PLAYER *player = NULL;

int32_t zc_do_dialog(DIALOG *d, int32_t f)
{
	auto oz = gui_mouse_z();
	int32_t ret=do_zqdialog(d,f);
	position_mouse_z(oz);
	return ret;
}

int32_t zc_popup_dialog(DIALOG *d, int32_t f)
{
	auto oz = gui_mouse_z();
	int32_t ret=popup_zqdialog(d,f);
	position_mouse_z(oz);
	return ret;
}

int32_t do_dialog_through_bitmap(BITMAP *buffer, DIALOG *dialog, int32_t focus_obj)
{
	auto oz = gui_mouse_z();
	BITMAP* orig_screen = screen;
	screen = buffer;
	
	int32_t ret=do_dialog(dialog, focus_obj);
	
	screen = orig_screen;
	blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
	position_mouse_z(oz);
	
	return ret;
}

int32_t zc_popup_dialog_dbuf(DIALOG *dialog, int32_t focus_obj)
{
	auto oz = gui_mouse_z();
	BITMAP* buffer = create_bitmap_ex(get_color_depth(),screen->w, screen->h);
	blit(screen, buffer, 0, 0, 0, 0, screen->w, screen->h);
	
	gui_set_screen(buffer);
	int32_t ret=popup_dialog(dialog, focus_obj);
	gui_set_screen(NULL);
	
	blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
	position_mouse_z(oz);
	return ret;
}

int32_t PopUp_dialog(DIALOG *d,int32_t f)
{
	auto oz = gui_mouse_z();
	// uses the bitmap that's already allocated
	go();
	player = init_dialog(d,f);
	
	while(update_dialog(player))
	{
		/* do nothing */
		rest(1);
	}
	
	int32_t ret = shutdown_dialog(player);
	comeback();
	position_mouse_z(oz);
	return ret;
}

int32_t popup_dialog_through_bitmap(BITMAP *, DIALOG *dialog, int32_t focus_obj)
{
	auto oz = gui_mouse_z();
	BITMAP *bmp;
	int32_t ret;
	
	bmp = create_bitmap_ex(bitmap_color_depth(screen),dialog->w+1, dialog->h+1);
	
	if(bmp)
	{
		scare_mouse();
		blit(screen, bmp, dialog->x, dialog->y, 0, 0, dialog->w+1, dialog->h+1);
		unscare_mouse();
	}
	else
		*allegro_errno = ENOMEM;
		
	ret = do_zqdialog(dialog, focus_obj);
	
	if(bmp)
	{
		scare_mouse();
		blit(bmp, screen, 0, 0, dialog->x, dialog->y, dialog->w+1, dialog->h+1);
		unscare_mouse();
		destroy_bitmap(bmp);
	}
	
	position_mouse_z(oz);
	
	return ret;
}

int32_t PopUp_dialog_through_bitmap(BITMAP *buffer,DIALOG *d,int32_t f)
{
	auto oz = gui_mouse_z();
	// uses the bitmap that's already allocated
	go();
	player = init_dialog(d,f);
	
	while(update_dialog_through_bitmap(buffer,player))
	{
		/* do nothing */
		rest(1);
	}
	
	int32_t ret = shutdown_dialog(player);
	comeback();
	position_mouse_z(oz);
	return ret;
}

int32_t update_dialog_through_bitmap(BITMAP* buffer, DIALOG_PLAYER *the_player)
{
	auto oz = gui_mouse_z();
	BITMAP* orig_screen = screen;
	int32_t result;
	screen = buffer;
	result = update_dialog(the_player);
	screen = orig_screen;
	blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
	position_mouse_z(oz);
	return result;
}

int32_t do_zqdialog(DIALOG *dialog, int32_t focus_obj)
{
	DIALOG_PLAYER *player2;
	ASSERT(dialog);
	
	popup_zqdialog_start();
	
	player2 = init_dialog(dialog, focus_obj);
	
	while(update_dialog(player2))
	{
		/* If a menu is active, we yield here, since the dialog
		* engine is shut down so no user code can be running.
		*/
		update_hw_screen(true);
		
		//if (active_menu_player2)
		//rest(1);
	}
	
	int ret = shutdown_dialog(player2);

	popup_zqdialog_end();
	return ret;
}



/* popup_dialog:
 *  Like do_dialog(), but it stores the data on the screen before drawing
 *  the dialog and restores it when the dialog is closed. The screen area
 *  to be stored is calculated from the dimensions of the first object in
 *  the dialog, so all the other objects should lie within this one.
 */
int32_t popup_zqdialog(DIALOG *dialog, int32_t focus_obj)
{
	ASSERT(dialog);

	return do_zqdialog(dialog, focus_obj);
}

int new_popup_dlg(DIALOG* dialog, int32_t focus_obj)
{
	DIALOG_PLAYER *player2 = init_dialog(dialog, focus_obj);
	
	while(update_dialog(player2))
		update_hw_screen(true);
	
	return shutdown_dialog(player2);
}

void new_gui_popup_dialog(DIALOG* dialog, int32_t focus_obj, bool& done, bool& running)
{
	running=true;
	int32_t ret=0;
	
	ASSERT(dialog);
	while(!done && ret>=0)
	{
		DIALOG_PLAYER *player2 = init_dialog(dialog, focus_obj);
		
		while(update_dialog(player2))
			update_hw_screen(true);
		
		ret = shutdown_dialog(player2);
	}
	running=false;
}


int popup_menu(MENU *menu,int x,int y)
{
	while(gui_mouse_b())
		rest(1);
	
	popup_zqdialog_start_a5();
	auto ret = jwin_do_menu(menu,x,y);
	popup_zqdialog_end_a5();
	return ret;
}
int popup_menu_abs(MENU *menu,int x,int y)
{
	int ox,oy,ow,oh;
	get_zqdialog_offset(ox,oy,ow,oh);
	return popup_menu(menu,x+ox,y+oy);
}

/*** end of gui.cpp ***/
