/****************************************************************************
 *   03/02/2021
 *   Copyright  2021 Jason Gayda
 *   Email: jason.gayda@gmail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "calculator.h"
#include "calculator_settings.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"


lv_obj_t *calculator_setup_tile = NULL;
lv_style_t calculator_setup_style;


static void app_onExit(lv_obj_t * obj, lv_event_t event);
static void win_close_event_cb(lv_obj_t * obj, lv_event_t event);

void settings_setup(uint32_t tile_num) 
{
    calculator_setup_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&calculator_setup_style, mainbar_get_style());

    lv_style_set_bg_color(&calculator_setup_style, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&calculator_setup_style, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width(&calculator_setup_style, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style(calculator_setup_tile, LV_OBJ_PART_MAIN, &calculator_setup_style);

    // WINDOW CONTROL
    lv_obj_t *win = lv_win_create(calculator_setup_tile, NULL);
    lv_win_set_title(win, "Window Title");
    lv_win_set_content_size(win, 235, 235);

    // Add a control button to the window header
    lv_obj_t *close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, win_close_event_cb);

     /*Add a setup button*/
    lv_win_add_btn(win, LV_SYMBOL_SETTINGS);       

    /*Add some dummy content*/
    lv_obj_t *txt = lv_label_create(win, NULL);
    lv_label_set_text(txt, "This is the content of the window\n\n"
                           "You can add control buttons to\n"
                           "the window header\n\n"
                           "The content area becomes\n"
                           "automatically scrollable is it's \n"
                           "large enough.\n\n"
                           "You can scroll the content\n"
                           "See the scroll bar on the right!");


    // EXIT BUTTON
    /*lv_obj_t *exit_cont = lv_obj_create(my_app_setup_tile, NULL);
    lv_obj_set_size(exit_cont, lv_disp_get_hor_res(NULL), 40);
    lv_obj_add_style(exit_cont, LV_OBJ_PART_MAIN, &my_app_setup_style);
    lv_obj_align(exit_cont, my_app_setup_tile, LV_ALIGN_IN_TOP_MID, 0, 10);
    
    lv_obj_t *exit_btn = lv_imgbtn_create(exit_cont, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &my_app_setup_style );
    lv_obj_align(exit_btn, exit_cont, LV_ALIGN_IN_TOP_LEFT, 10, 0);
    lv_obj_set_event_cb(exit_btn, app_onExit);
    
    

    */
}


static void win_close_event_cb(lv_obj_t * obj, lv_event_t event)
{
    switch(event) 
    {
        case(LV_EVENT_CLICKED): 
            Serial.printf("close button clicked");      
            mainbar_jump_to_tilenumber(get_app_main_tile_num(), LV_ANIM_ON);
            break;
    }
}


static void app_onExit(lv_obj_t * obj, lv_event_t event) 
{
    switch(event) 
    {
        case(LV_EVENT_CLICKED):       
            mainbar_jump_to_tilenumber(get_app_main_tile_num(), LV_ANIM_ON);
            break;
    }
}