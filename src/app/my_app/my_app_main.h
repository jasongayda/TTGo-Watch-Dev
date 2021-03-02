/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
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
#ifndef _MY_APP_MAIN_H
    #define _MY_APP_MAIN_H

    #include <TTGO.h>
    #include "lvgl/src/lv_misc/lv_color.h"

    #define HEADER_HEIGHT            26
    #define WIN_CONTENT_W            240
    #define WIN_CONTENT_H            214
    #define TXT_CALC_DISPLAY_HEIGHT  36
    #define CALC_BUTTON_HEIGHT       36
    #define CALC_BUTTON_WIDTH        36
    #define CALC_EQUAL_BUTTON_HEIGHT 77
    #define CALC_EQUAL_BUTTON_WIDTH  36

    #define ROW1_Y                   55
    #define ROW2_Y                   96
    #define ROW3_Y                   137
    #define ROW4_Y                   178

    #define COL1_X                   20
    #define COL2_X                   61
    #define COL3_X                   102
    #define COL4_X                   143
    #define COL5_X                   184

    void main_setup(uint32_t tile_num);

#endif // _MY_APP_MAIN_H