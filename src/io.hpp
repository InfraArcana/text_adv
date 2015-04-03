#ifndef IO_H
#define IO_H

#include <string>

#include <SDL.h>

#include "cmn_types.hpp"

enum Allow_Wrap_Text {no, yes};

namespace io
{

void init();
void cleanup();

void update_screen();

void clear_screen();

void get_cmd(std::string& out, const std::string& msg);

void sleep(const Uint32 DURATION);

Rect draw_text(const std::string& str, const Pos& pos, const Clr& clr,
               Vertical_dir dir = Vertical_dir::down,
               Allow_Wrap_Text allow_wrap = Allow_Wrap_Text::no);

//void draw_text(const std::string& str, const Pos& pos, const Clr& clr);

// int draw_text_centered(const std::string& str, const Panel panel, const Pos& pos,
//                        const Clr& clr, const Clr& bg_clr = clr_black,
//                        const bool IS_PIXEL_POS_ADJ_ALLOWED = true);

} // io

#endif // IO_H
