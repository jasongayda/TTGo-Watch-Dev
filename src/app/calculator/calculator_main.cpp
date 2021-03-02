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
#include "calculator_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"


lv_obj_t *calculator_main_tile = NULL;
lv_obj_t *txtCalcDisplay = NULL;

lv_style_t calculator_main_style;

lv_task_t *_app_task;

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(refresh_32px);
LV_FONT_DECLARE(Ubuntu_72px);

static void calculator_onExit(lv_obj_t * obj, lv_event_t event);

static void calculator_setup_onEnter(lv_obj_t * obj, lv_event_t event);

static void win_close_event_cb(lv_obj_t * obj, lv_event_t event);

static void calc_button_clicked(lv_obj_t * obj, lv_event_t event);

void app_task(lv_task_t * task);


void main_setup(uint32_t tile_num) 
{
    calculator_main_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&calculator_main_style, mainbar_get_style());


    // WINDOW CONTROL
    lv_obj_t *win = lv_win_create(calculator_main_tile, NULL);
    lv_obj_set_pos(win, 0, 0);
    lv_win_set_header_height(win, HEADER_HEIGHT);
    lv_win_set_title(win, "Calculator");
    lv_win_set_content_size(win, WIN_CONTENT_W, WIN_CONTENT_H);
    lv_win_set_scrollbar_mode(win, LV_SCRLBAR_MODE_OFF);
    lv_win_set_drag(win, false);

    // Add a control button to the window header
    lv_obj_t *close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, win_close_event_cb);

     /*Add a settings button*/
    lv_win_add_btn(win, LV_SYMBOL_SETTINGS);       

    // CALCULATOR DISPLAY TEXT AREA
	txtCalcDisplay = lv_textarea_create(win, NULL);

	//Write style LV_PAGE_PART_BG for screen_txtCalcDisplay
	static lv_style_t style_screen_txtCalcDisplay_bg;
	lv_style_init(&style_screen_txtCalcDisplay_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_txtCalcDisplay_bg
	lv_style_set_radius(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_border_color(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	lv_style_set_text_color(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_text_font(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, &lv_font_montserrat_32);

	lv_style_set_text_letter_space(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	
    //lv_style_set_pad_left(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	//lv_style_set_pad_right(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	//lv_style_set_pad_top(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	//lv_style_set_pad_bottom(&style_screen_txtCalcDisplay_bg, LV_STATE_DEFAULT, 2);
	lv_obj_add_style(txtCalcDisplay, LV_PAGE_PART_BG, &style_screen_txtCalcDisplay_bg);

	//Write style LV_PAGE_PART_SCROLLABLE for screen_txtCalcDisplay
	static lv_style_t style_screen_txtCalcDisplay_scrollable;
	lv_style_init(&style_screen_txtCalcDisplay_scrollable);

	//Write style state: LV_STATE_DEFAULT for style_screen_txtCalcDisplay_scrollable
	lv_style_set_radius(&style_screen_txtCalcDisplay_scrollable, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_txtCalcDisplay_scrollable, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_txtCalcDisplay_scrollable, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_txtCalcDisplay_scrollable, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_txtCalcDisplay_scrollable, LV_STATE_DEFAULT, 255);
	//lv_obj_add_style(ui->screen_txtCalcDisplay, LV_PAGE_PART_SCROLLABLE, &style_screen_txtCalcDisplay_scrollable);

	//Write style LV_PAGE_PART_EDGE_FLASH for screen_txtCalcDisplay
	static lv_style_t style_screen_txtCalcDisplay_edge_flash;
	lv_style_init(&style_screen_txtCalcDisplay_edge_flash);

	//Write style state: LV_STATE_DEFAULT for style_screen_txtCalcDisplay_edge_flash
	//lv_style_set_radius(&style_screen_txtCalcDisplay_edge_flash, LV_STATE_DEFAULT, 0);
	//lv_style_set_bg_opa(&style_screen_txtCalcDisplay_edge_flash, LV_STATE_DEFAULT, 255);
	//lv_obj_add_style(ui->screen_txtCalcDisplay, LV_PAGE_PART_EDGE_FLASH, &style_screen_txtCalcDisplay_edge_flash);
	lv_obj_set_pos(txtCalcDisplay, 20, 0);
	lv_obj_set_size(txtCalcDisplay, 200, TXT_CALC_DISPLAY_HEIGHT);
	lv_textarea_set_text(txtCalcDisplay, "0");
    lv_textarea_set_one_line(txtCalcDisplay, true);
    
	//lv_obj_set_event_cb(txtCalcDisplay, text_area_event_cb);
	lv_textarea_set_text_align(txtCalcDisplay, LV_LABEL_ALIGN_RIGHT);

	
    // ZERO BUTTON
	lv_obj_t *btn0 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn0
	static lv_style_t style_screen_btn0_main;
	lv_style_init(&style_screen_btn0_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn0_main
	lv_style_set_radius(&style_screen_btn0_main, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&style_screen_btn0_main, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_screen_btn0_main, LV_STATE_PRESSED, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn0_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn0_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));

	lv_style_set_border_width(&style_screen_btn0_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn0_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn0_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn0_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn0, LV_BTN_PART_MAIN, &style_screen_btn0_main);
	lv_obj_set_pos(btn0, COL2_X, ROW4_Y);
	lv_obj_set_size(btn0, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn0_label = lv_label_create(btn0, NULL);
	lv_label_set_text(btn0_label, "0");
    lv_obj_set_event_cb(btn0, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn0_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn0_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);



    // ONE BUTTON
	lv_obj_t *btn1 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn1
	static lv_style_t style_screen_btn1_main;
	lv_style_init(&style_screen_btn1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn1_main
	lv_style_set_radius(&style_screen_btn1_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn1_main, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_screen_btn1_main, LV_STATE_PRESSED, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &style_screen_btn1_main);
	lv_obj_set_pos(btn1, COL1_X, ROW3_Y);
	lv_obj_set_size(btn1, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn1_label = lv_label_create(btn1, NULL);
	lv_label_set_text(btn1_label, "1");
    lv_obj_set_event_cb(btn1, calc_button_clicked);
	//lv_obj_set_style_local_text_color(ui->screen_btn1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(ui->screen_btn1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);

	// 2 BUTTON
	lv_obj_t *btn2 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn2
	static lv_style_t style_screen_btn2_main;
	lv_style_init(&style_screen_btn2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn2_main
	lv_style_set_radius(&style_screen_btn2_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn2_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_screen_btn2_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn2_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn2_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn2_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_screen_btn2_main);
	lv_obj_set_pos(btn2, COL2_X, ROW3_Y);
	lv_obj_set_size(btn2, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn2_label = lv_label_create(btn2, NULL);
	lv_label_set_text(btn2_label, "2");
    lv_obj_set_event_cb(btn2, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);

	// 3 BUTTON
	lv_obj_t *btn3 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn3
	static lv_style_t style_screen_btn3_main;
	lv_style_init(&style_screen_btn3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn3_main
	lv_style_set_radius(&style_screen_btn3_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn3_main, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_screen_btn3_main, LV_STATE_PRESSED, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn3_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn3_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn3_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn3_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn3_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn3_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &style_screen_btn3_main);
	lv_obj_set_pos(btn3, COL3_X, ROW3_Y);
	lv_obj_set_size(btn3, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn3_label = lv_label_create(btn3, NULL);
	lv_label_set_text(btn3_label, "3");
    lv_obj_set_event_cb(btn3, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn3_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn3_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


    // 4 BUTTON
	lv_obj_t *btn4 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn4
	static lv_style_t style_screen_btn4_main;
	lv_style_init(&style_screen_btn4_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn4_main
	lv_style_set_radius(&style_screen_btn4_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn4_main, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btn4_main, LV_STATE_PRESSED, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn4_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn4_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn4_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn4_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn4_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn4_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &style_screen_btn4_main);
	lv_obj_set_pos(btn4, COL1_X, ROW2_Y);
	lv_obj_set_size(btn4, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn4_label = lv_label_create(btn4, NULL);
	lv_label_set_text(btn4_label, "4");
    lv_obj_set_event_cb(btn4, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn4_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn4_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// 5 BUTTON
	lv_obj_t *btn5 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn5
	static lv_style_t style_screen_btn5_main;
	lv_style_init(&style_screen_btn5_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn5_main
	lv_style_set_radius(&style_screen_btn5_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn5_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btn5_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn5_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn5_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn5_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn5_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn5_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn5_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn5, LV_BTN_PART_MAIN, &style_screen_btn5_main);
	lv_obj_set_pos(btn5, COL2_X, ROW2_Y);
	lv_obj_set_size(btn5, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn5_label = lv_label_create(btn5, NULL);
	lv_label_set_text(btn5_label, "5");
    lv_obj_set_event_cb(btn5, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn5_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn5_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// 6 BUTTON
	lv_obj_t *btn6 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn6
	static lv_style_t style_screen_btn6_main;
	lv_style_init(&style_screen_btn6_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn6_main
	lv_style_set_radius(&style_screen_btn6_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn6_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btn6_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn6_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn6_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn6_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn6_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn6_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn6_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn6, LV_BTN_PART_MAIN, &style_screen_btn6_main);
	lv_obj_set_pos(btn6, COL3_X, ROW2_Y);
	lv_obj_set_size(btn6, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn6_label = lv_label_create(btn6, NULL);
	lv_label_set_text(btn6_label, "6");
    lv_obj_set_event_cb(btn6, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn6_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn6_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


    // 7 BUTTON
	lv_obj_t *btn7 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn7
	static lv_style_t style_screen_btn7_main;
	lv_style_init(&style_screen_btn7_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn7_main
	lv_style_set_radius(&style_screen_btn7_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn7_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btn7_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn7_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn7_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn7_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn7_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn7_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn7_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn7, LV_BTN_PART_MAIN, &style_screen_btn7_main);
	lv_obj_set_pos(btn7, COL1_X, ROW1_Y);
	lv_obj_set_size(btn7, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn7_label = lv_label_create(btn7, NULL);
	lv_label_set_text(btn7_label, "7");
    lv_obj_set_event_cb(btn7, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn7_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn7_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// 8 BUTTON
	lv_obj_t *btn8 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn8
	static lv_style_t style_screen_btn8_main;
	lv_style_init(&style_screen_btn8_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn8_main
	lv_style_set_radius(&style_screen_btn8_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn8_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_screen_btn8_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn8_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn8_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn8_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn8_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn8_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn8_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn8, LV_BTN_PART_MAIN, &style_screen_btn8_main);
	lv_obj_set_pos(btn8, COL2_X, ROW1_Y);
	lv_obj_set_size(btn8, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn8_label = lv_label_create(btn8, NULL);
	lv_label_set_text(btn8_label, "8");
    lv_obj_set_event_cb(btn8, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn8_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn8_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);

    
	// 9 BUTTON
	lv_obj_t *btn9 = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn9
	static lv_style_t style_screen_btn9_main;
	lv_style_init(&style_screen_btn9_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn9_main
	lv_style_set_radius(&style_screen_btn9_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btn9_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btn9_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btn9_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn9_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn9_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn9_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn9_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn9_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btn9, LV_BTN_PART_MAIN, &style_screen_btn9_main);
	lv_obj_set_pos(btn9, COL3_X, ROW1_Y);
	lv_obj_set_size(btn9, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btn9_label = lv_label_create(btn9, NULL);
	lv_label_set_text(btn9_label, "9");
    lv_obj_set_event_cb(btn9, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btn9_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btn9_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);

	
	// DECIMAL BUTTON
	lv_obj_t *btnDecimal = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnDecimal
	static lv_style_t style_screen_btnDecimal_main;
	lv_style_init(&style_screen_btnDecimal_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnDecimal_main
	lv_style_set_radius(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnDecimal_main, LV_STATE_DEFAULT,  LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_screen_btnDecimal_main, LV_STATE_PRESSED,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnDecimal_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnDecimal, LV_BTN_PART_MAIN, &style_screen_btnDecimal_main);
	lv_obj_set_pos(btnDecimal, COL1_X, ROW4_Y);
	lv_obj_set_size(btnDecimal, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnDecimal_label = lv_label_create(btnDecimal, NULL);
	lv_label_set_text(btnDecimal_label, ".");
    lv_obj_set_event_cb(btnDecimal, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnDecimal_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnDecimal_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// EQUALS BUTTON
	lv_obj_t *btnEquals = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnEquals
	static lv_style_t style_screen_btnEquals_main;
	lv_style_init(&style_screen_btnEquals_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnEquals_main
	lv_style_set_radius(&style_screen_btnEquals_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnEquals_main, LV_STATE_DEFAULT,  LV_COLOR_ORANGE);
	lv_style_set_bg_opa(&style_screen_btnEquals_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnEquals_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnEquals_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnEquals_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnEquals_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnEquals_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnEquals, LV_BTN_PART_MAIN, &style_screen_btnEquals_main);
	lv_obj_set_pos(btnEquals, COL5_X, ROW3_Y);
	lv_obj_set_size(btnEquals, CALC_EQUAL_BUTTON_WIDTH, CALC_EQUAL_BUTTON_HEIGHT);
	lv_obj_t *btnEquals_label = lv_label_create(btnEquals, NULL);
	lv_label_set_text(btnEquals_label, "=");
    lv_obj_set_event_cb(btnEquals, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnEquals_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnEquals_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// DIVISION BUTTON
	lv_obj_t *btnDivision = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnDivision
	static lv_style_t style_screen_btnDivision_main;
	lv_style_init(&style_screen_btnDivision_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnDivision_main
	lv_style_set_radius(&style_screen_btnDivision_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnDivision_main, LV_STATE_DEFAULT,  LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnDivision_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnDivision_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnDivision_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnDivision_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnDivision_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnDivision_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnDivision, LV_BTN_PART_MAIN, &style_screen_btnDivision_main);
	lv_obj_set_pos(btnDivision, COL4_X, ROW1_Y);
	lv_obj_set_size(btnDivision, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnDivision_label = lv_label_create(btnDivision, NULL);
	lv_label_set_text(btnDivision_label, "/");
    lv_obj_set_event_cb(btnDivision, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnDivision_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnDivision_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// MULTIPLY BUTTON
	lv_obj_t *btnMultiply = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnMultiply
	static lv_style_t style_screen_btnMultiply_main;
	lv_style_init(&style_screen_btnMultiply_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnMultiply_main
	lv_style_set_radius(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnMultiply_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnMultiply, LV_BTN_PART_MAIN, &style_screen_btnMultiply_main);
	lv_obj_set_pos(btnMultiply, COL4_X, ROW2_Y);
	lv_obj_set_size(btnMultiply, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnMultiply_label = lv_label_create(btnMultiply, NULL);
	lv_label_set_text(btnMultiply_label, "*");
    lv_obj_set_event_cb(btnMultiply, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnMultiply_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnMultiply_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// SUBTRACT BUTTON
	lv_obj_t *btnSubtract = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnSubtract
	static lv_style_t style_screen_btnSubtract_main;
	lv_style_init(&style_screen_btnSubtract_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnSubtract_main
	lv_style_set_radius(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnSubtract_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnSubtract, LV_BTN_PART_MAIN, &style_screen_btnSubtract_main);
	lv_obj_set_pos(btnSubtract, COL4_X, ROW3_Y);
	lv_obj_set_size(btnSubtract, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnSubtract_label = lv_label_create(btnSubtract, NULL);
	lv_label_set_text(btnSubtract_label, "-");
    lv_obj_set_event_cb(btnSubtract, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnSubtract_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnSubtract_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// ADD BUTTON
	lv_obj_t *btnAdd = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnAdd
	static lv_style_t style_screen_btnAdd_main;
	lv_style_init(&style_screen_btnAdd_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnAdd_main
	lv_style_set_radius(&style_screen_btnAdd_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnAdd_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnAdd_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnAdd_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnAdd_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnAdd_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnAdd_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnAdd_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnAdd, LV_BTN_PART_MAIN, &style_screen_btnAdd_main);
	lv_obj_set_pos(btnAdd, COL4_X, ROW4_Y);
	lv_obj_set_size(btnAdd, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnAdd_label = lv_label_create(btnAdd, NULL);
	lv_label_set_text(btnAdd_label, "+");
    lv_obj_set_event_cb(btnAdd, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnAdd_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnAdd_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// CLEAR BUTTON
	lv_obj_t *btnClear = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnClear
	static lv_style_t style_screen_btnClear_main;
	lv_style_init(&style_screen_btnClear_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnClear_main
	lv_style_set_radius(&style_screen_btnClear_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnClear_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnClear_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnClear_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnClear_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnClear_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnClear_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnClear_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnClear, LV_BTN_PART_MAIN, &style_screen_btnClear_main);
	lv_obj_set_pos(btnClear, COL5_X, ROW1_Y);
	lv_obj_set_size(btnClear, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnClear_label = lv_label_create(btnClear, NULL);
	lv_label_set_text(btnClear_label, "C");
    lv_obj_set_event_cb(btnClear, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnClear_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnClear_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// NEGATIVE BUTTON
	lv_obj_t *btnNegative = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnNegative
	static lv_style_t style_screen_btnNegative_main;
	lv_style_init(&style_screen_btnNegative_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnNegative_main
	lv_style_set_radius(&style_screen_btnNegative_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnNegative_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnNegative_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnNegative_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnNegative_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnNegative_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnNegative_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnNegative_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnNegative, LV_BTN_PART_MAIN, &style_screen_btnNegative_main);
	lv_obj_set_pos(btnNegative, COL3_X, ROW4_Y);
	lv_obj_set_size(btnNegative, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnNegative_label = lv_label_create(btnNegative, NULL);
	lv_label_set_text(btnNegative_label, "+/-");
    lv_obj_set_event_cb(btnNegative, calc_button_clicked);
	//lv_obj_set_style_local_text_color(btnNegative_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnNegative_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);


	// PERCENT BUTTON
	lv_obj_t *btnPercent = lv_btn_create(win, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btnPercent
	static lv_style_t style_screen_btnPercent_main;
	lv_style_init(&style_screen_btnPercent_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btnPercent_main
	lv_style_set_radius(&style_screen_btnPercent_main, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_screen_btnPercent_main, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_bg_opa(&style_screen_btnPercent_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btnPercent_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btnPercent_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btnPercent_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btnPercent_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btnPercent_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(btnPercent, LV_BTN_PART_MAIN, &style_screen_btnPercent_main);
	lv_obj_set_pos(btnPercent, COL5_X, ROW2_Y);
	lv_obj_set_size(btnPercent, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT);
	lv_obj_t *btnPercent_label = lv_label_create(btnPercent, NULL);
	lv_label_set_text(btnPercent_label, "%");
    lv_obj_set_event_cb(btnPercent, calc_button_clicked);
	//lv_obj_set_style_local_text_color(_btnPercent_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	//lv_obj_set_style_local_text_font(btnPercent_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);



    /*Add some dummy content*/
    //lv_obj_t *txt = lv_label_create(win, NULL);
    //lv_label_set_text(txt, "This is the content of the window\n\n"
    //                       "You can add control buttons to\n"
    //                       "the window header\n\n"
    //                       "The content area becomes\n"
    //                       "automatically scrollable is it's \n"
    //                       "large enough.\n\n"
    //                       "You can scroll the content\n"
    //                       "See the scroll bar on the right!");
    // EXIT BUTTON
    //lv_obj_t * exit_btn = lv_imgbtn_create(my_app_main_tile, NULL);
    
    //lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    //lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    //lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    //lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    
    //lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &my_app_main_style);
    
    //lv_obj_align(exit_btn, my_app_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    
    //lv_obj_set_event_cb(exit_btn, my_app_onExit);


    // SETUP BUTTON
    //lv_obj_t * setup_btn = lv_imgbtn_create(my_app_main_tile, NULL);
    
    //lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_RELEASED, &setup_32px);
    //lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_PRESSED, &setup_32px);
    //lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_RELEASED, &setup_32px);
    //lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_PRESSED, &setup_32px);
    
    //lv_obj_add_style(setup_btn, LV_IMGBTN_PART_MAIN, &my_app_main_style);
    
    //lv_obj_align(setup_btn, my_app_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);
    
    //lv_obj_set_event_cb(setup_btn, my_app_setup_onEnter);

    // BACKGROUND AND LABEL
    // uncomment the following block of code to remove the "myapp" label in background
    //lv_style_set_text_opa(&my_app_main_style, LV_OBJ_PART_MAIN, LV_OPA_70);
    //lv_style_set_text_font(&my_app_main_style, LV_STATE_DEFAULT, &Ubuntu_72px);
    
    //lv_obj_t *app_label = lv_label_create(my_app_main_tile, NULL);
    //lv_label_set_text(app_label, "my app");
    
    //lv_obj_reset_style_list(app_label, LV_OBJ_PART_MAIN);
    //lv_obj_add_style(app_label, LV_OBJ_PART_MAIN, &my_app_main_style);
    //lv_obj_align(app_label, my_app_main_tile, LV_ALIGN_CENTER, 0, 0);


    // create an task that runs every secound
    _app_task = lv_task_create(app_task, 1000, LV_TASK_PRIO_MID, NULL);
}


static void calc_button_clicked(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) 
    {
        lv_obj_t * label = lv_obj_get_child(obj, NULL);
        const char *mystring = "";
        mystring = lv_label_get_text(label);

        printf("Clicked By %s\n", mystring);
        lv_textarea_set_text(txtCalcDisplay, mystring);
    }
}


static void win_close_event_cb(lv_obj_t * obj, lv_event_t event)
{
    switch(event) 
    {
        case(LV_EVENT_CLICKED): 
            Serial.printf("close button clicked");      
            mainbar_jump_to_maintile( LV_ANIM_OFF );
            break;
    }
}



/*static void calculator_setup_onEnter(lv_obj_t * obj, lv_event_t event) 
{
    switch( event ) 
    {
        case( LV_EVENT_CLICKED ):       
            statusbar_hide( true );
            mainbar_jump_to_tilenumber(get_app_setup_tile_num(), LV_ANIM_ON);
            break;
    }
}*/


/*static void calculator_onExit(lv_obj_t * obj, lv_event_t event) 
{
    switch(event) 
    {
        case( LV_EVENT_CLICKED ):       
            mainbar_jump_to_maintile( LV_ANIM_OFF );
            break;
    }
}*/


void app_task(lv_task_t * task) {
    // put your code here


    
}
