#include "io.hpp"

#include <vector>

#include <SDL_ttf.h>

#include "init.hpp"
#include "cmn_data.hpp"

namespace io
{

namespace
{

bool            is_inited_          = false;
SDL_Window*     sdl_window_         = nullptr;
SDL_Renderer*   sdl_renderer_       = nullptr;
SDL_Texture*    sdl_screen_texture_ = nullptr;
TTF_Font*       font_               = nullptr;
int             font_h_             = 0;
SDL_Event       sdl_event_;

std::vector<std::string> msg_history_;

std::string prev_cmd_ = "";

void load_font()
{
    TRACE_FUNC_BEGIN;

    font_ = TTF_OpenFont("fonts/Vera.ttf", 14);

    if (font_ == nullptr)
    {
        TRACE("Failed to load font");
        assert(false);
    }

    TRACE_FUNC_END;
}

void cleanup_font()
{
    TRACE_FUNC_BEGIN;

    TTF_CloseFont(font_);

    TRACE_FUNC_END;
}

void set_render_clr(const Clr& clr)
{
    SDL_SetRenderDrawColor(sdl_renderer_, clr.r, clr.g, clr.b, 255);
}

void draw_line(const Rect& r, const Clr& clr = clr_black)
{
    if (!is_inited_)
    {
        return;
    }

    set_render_clr(clr);

    SDL_RenderDrawLine(sdl_renderer_, r.p0.x, r.p0.y, r.p1.x, r.p1.y);
}

void draw_cursor(const std::string& text, const Pos& cmd_text_pos, const size_t cursor_idx)
{
    if (!is_inited_)
    {
        return;
    }

    assert(cursor_idx <= text.size());

    int cursor_x = cmd_text_pos.x;

    for (size_t i = 0; i < cursor_idx; ++i)
    {
        int advance = 0;
        TTF_GlyphMetrics(font_, text[i], nullptr, nullptr, nullptr, nullptr, &advance);
        cursor_x += advance;
    }

    const int Y_PADDING = font_h_ / 10;

    draw_line({cursor_x, cmd_text_pos.y + Y_PADDING,
               cursor_x, cmd_text_pos.y + font_h_ - Y_PADDING
              });
}

bool is_inside_window(const Pos& p)
{
    if (!is_inited_)
    {
        return false;
    }

    Pos window_dims;
    SDL_GetWindowSize(sdl_window_, &window_dims.x, &window_dims.y);

    return p >= 0 && p < window_dims;
}

bool is_inside_window(const Rect& r)
{
    if (!is_inited_)
    {
        return false;
    }

    Pos window_dims;
    SDL_GetWindowSize(sdl_window_, &window_dims.x, &window_dims.y);

    return r.p0 >= 0 && r.p1 < window_dims;
}

} // Namespace

void init()
{
    TRACE_FUNC_BEGIN;
    cleanup();

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        TRACE("Failed to init SDL");
        assert(false);
    }

    if (TTF_Init() == -1)
    {
        TRACE("Failed to init SDL_ttf");
        assert(false);
    }

    load_font();

    font_h_ = TTF_FontLineSkip(font_);

    TRACE("Setting up rendering window");

    const std::string title = "Text Adventure " + game_version_str;

    const int SCR_W     = 640;
    const int NR_LINES  = 20;
    const int SCR_H     = font_h_ * NR_LINES;

    sdl_window_ = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCR_W, SCR_H,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!sdl_window_)
    {
        TRACE("Failed to create window");
        assert(false);
    }

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);

    if (!sdl_renderer_)
    {
        TRACE("Failed to create SDL renderer");
        assert(false);
    }

    sdl_screen_texture_ = SDL_CreateTexture(sdl_renderer_,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            SCR_W, SCR_H);

    if (!sdl_screen_texture_)
    {
        TRACE("Failed to create screen texture");
        assert(false);
    }

    is_inited_ = true;

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    font_h_ = 0;

    msg_history_.clear();

    prev_cmd_ = "";

    cleanup_font();

    if (sdl_renderer_)
    {
        SDL_DestroyRenderer(sdl_renderer_);
        sdl_renderer_ = nullptr;
    }

    if (sdl_window_)
    {
        SDL_DestroyWindow(sdl_window_);
        sdl_window_ = nullptr;
    }

    if (sdl_screen_texture_)
    {
        SDL_DestroyTexture(sdl_screen_texture_);
        sdl_screen_texture_ = nullptr;
    }

    TTF_Quit();

    SDL_Quit();

    is_inited_ = false;

    TRACE_FUNC_END;
}

void update_screen()
{
    if (is_inited_)
    {
        SDL_RenderPresent(sdl_renderer_);
    }
}

void clear_screen()
{
    if (is_inited_)
    {
        set_render_clr(clr_white_high);
        SDL_RenderClear(sdl_renderer_);
    }
}

Rect draw_text(const std::string& str, const Pos& pos, const Clr& clr, Vertical_dir dir, Allow_wrap_text allow_wrap)
{
    if (!is_inited_)
    {
        return Rect();
    }

    assert(!str.empty());

    int window_w, window_h;
    SDL_GetWindowSize(sdl_window_, &window_w, &window_h);

    SDL_Surface* font_srf = nullptr;

    if (allow_wrap == Allow_wrap_text::yes)
    {
        font_srf = TTF_RenderText_Blended_Wrapped(font_, str.c_str(), clr, window_w);
    }
    else // Wrap not allowed
    {
        font_srf = TTF_RenderText_Blended(font_, str.c_str(), clr);
    }

    assert(font_srf);

    SDL_Rect sdl_dst_rect = {pos.x, pos.y, font_srf->w, font_srf->h};

    if (dir == Vertical_dir::up)
    {
        // Write above the given position
        sdl_dst_rect.y = pos.y - sdl_dst_rect.h + 1;
    }

    Rect ret_area(sdl_dst_rect.x,
                  sdl_dst_rect.y,
                  sdl_dst_rect.x + sdl_dst_rect.w - 1,
                  sdl_dst_rect.y + sdl_dst_rect.h - 1);

    SDL_Texture* font_texture = SDL_CreateTextureFromSurface(sdl_renderer_, font_srf);

    SDL_RenderCopy(sdl_renderer_, font_texture, nullptr, &sdl_dst_rect);

    SDL_FreeSurface(font_srf);

    SDL_DestroyTexture(font_texture);

    return ret_area;
}

//Rect draw_text_above(const std::string& str, const Pos& pos, const Clr& clr)
//{
//    if (!is_inited_)
//    {
//        return Rect();
//    }
//
//    assert(!str.empty());
//
//    int window_w = 0;
//    SDL_GetWindowSize(sdl_window_, &window_w, nullptr);
//
//    SDL_Surface* font_srf       = TTF_RenderText_Blended_Wrapped(font_, str.c_str(), clr, window_w);
//    SDL_Texture* font_texture   = SDL_CreateTextureFromSurface(sdl_renderer_, font_srf);
//
//    SDL_Rect sdl_dst_rect {pos.x, pos.y - font_srf->h + 1, font_srf->w, font_srf->h};
//
//    SDL_FreeSurface(font_srf);
//
//    if (sdl_dst_rect.y < 0)
//    {
//        return Rect(-1, -1, -1, -1);
//    }
//
//    SDL_RenderCopy(sdl_renderer_, font_texture, nullptr, &sdl_dst_rect);
//
//    return Rect(sdl_dst_rect.x,
//                sdl_dst_rect.y,
//                sdl_dst_rect.x + sdl_dst_rect.w - 1,
//                sdl_dst_rect.y + sdl_dst_rect.y - 1);
//}

// int draw_text_centered(const string& str, const Panel panel, const Pos& pos,
//                       const Clr& clr, const Clr& bg_clr,
//                       const bool IS_PIXEL_POS_ADJ_ALLOWED)
// {
//    const int LEN         = str.size();
//    const int LEN_HALF    = LEN / 2;
//    const int X_POS_LEFT  = pos.x - LEN_HALF;
//
//    const Pos cell_dims(config::get_cell_w(), config::get_cell_h());
//
//    Pos px_pos = get_px_pos_for_cell_in_panel(panel, Pos(X_POS_LEFT, pos.y));
//
//    if (IS_PIXEL_POS_ADJ_ALLOWED)
//    {
//        const int PIXEL_X_ADJ = LEN_HALF * 2 == LEN ? cell_dims.x / 2 : 0;
//        px_pos += Pos(PIXEL_X_ADJ, 0);
//    }
//
//    const int W_TOT_PIXEL = LEN * cell_dims.x;
//
//    SDL_Rect sdl_rect =
//    {
//        (Sint16)px_pos.x, (Sint16)px_pos.y,
//        (Uint16)W_TOT_PIXEL, (Uint16)cell_dims.y
//    };
//
//    SDL_FillRect(screen_srf_, &sdl_rect, SDL_MapRGB(screen_srf_->format, bg_clr.r, bg_clr.g, bg_clr.b));
//
//    for (int i = 0; i < LEN; ++i)
//    {
//        if (px_pos.x < 0 || px_pos.x >= config::get_screen_px_w())
//        {
//            return X_POS_LEFT;
//        }
//
//        draw_glyph_at_px(str[i], px_pos, clr, false, bg_clr);
//        px_pos.x += cell_dims.x;
//    }
//
//    return X_POS_LEFT;
// }

void get_cmd(std::string& out, const std::string& msg)
{
    if (!is_inited_)
    {
        return;
    }

    assert(!msg.empty());

    msg_history_.push_back(msg);

    const size_t CULL_MSG_HISTORY_AT_N_MSGS = 160;
    const size_t NR_MSGS_TO_KEEP_AFTER_CULL = 80;

    if (msg_history_.size() >= CULL_MSG_HISTORY_AT_N_MSGS)
    {
        TRACE("Culling message history (Keeping %d messages)", NR_MSGS_TO_KEEP_AFTER_CULL);

        const size_t NR_MSGS_TO_ERASE = CULL_MSG_HISTORY_AT_N_MSGS - NR_MSGS_TO_KEEP_AFTER_CULL;
        msg_history_.erase(begin(msg_history_), begin(msg_history_) + NR_MSGS_TO_ERASE - 1);
    }

    out = "";

    size_t cursor_pos = 0;

    SDL_StartTextInput();

    bool is_done = false;

    while (!is_done)
    {
        // Get current input
        const bool DID_POLL_EVENT = SDL_PollEvent(&sdl_event_);

        if (!DID_POLL_EVENT)
        {
            // Sleep for a moment so the process doesn't claim all CPU time
            sleep(1);
        }

        // Draw
        clear_screen();

        int window_w, window_h;
        SDL_GetWindowSize(sdl_window_, &window_w, &window_h);

//        const int INFO_SEPARATOR_LINE_Y = font_h_ + 1;
//
//        draw_line({0, INFO_SEPARATOR_LINE_Y, window_w, INFO_SEPARATOR_LINE_Y});

        Pos text_pos(0, window_h - 1);

        const std::string prompt_str = ">";

        const std::string cmd_render_str = prompt_str + out;

        Rect msg_rect = draw_text(cmd_render_str, text_pos, clr_black, Vertical_dir::up, Allow_wrap_text::no);

//        const int SEPARATOR_LINE_Y = msg_rect.p0.y - 1;
//
//        draw_line({0, SEPARATOR_LINE_Y, window_w, SEPARATOR_LINE_Y});

        const Uint32 CURSOR_PERIOD = 1000;

        if (sdl_event_.type == SDL_KEYDOWN || (SDL_GetTicks() % CURSOR_PERIOD) > (CURSOR_PERIOD / 2))
        {
            draw_cursor(cmd_render_str + ' ', msg_rect.p0, prompt_str.size() + cursor_pos);
        }

        for (auto it = msg_history_.rbegin(); it != msg_history_.rend(); ++it)
        {
            const std::string cur_msg = *it;

            text_pos.y = msg_rect.p0.y - 1;

            if (cur_msg[0] != '>')
            {
                text_pos.y -= font_h_;
            }

            msg_rect = draw_text(*it, text_pos, clr_black, Vertical_dir::up, Allow_wrap_text::yes);

            if (msg_rect.p0.y < 0)
            {
                break;
            }
        }

        update_screen();

        // Handle input
        if (!DID_POLL_EVENT)
        {
            continue;
        }

        switch (sdl_event_.type)
        {

        case SDL_WINDOWEVENT:
        {
            switch (sdl_event_.window.event)
            {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_RESTORED:
                break;

            case SDL_WINDOWEVENT_RESIZED:
                int new_w, new_h;
                SDL_GetWindowSize(sdl_window_, &new_w, &new_h);
                SDL_SetWindowSize(sdl_window_, std::max(new_w, MIN_WINDOW_W), std::max(new_h, MIN_WINDOW_H));
                break;

            default:
                break;
            }
        }
        break;

        case SDL_QUIT:
            out = "exit";
            is_done = true;
            break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            break;

        case SDL_KEYDOWN:
        {
            const SDL_Keycode sdl_key = sdl_event_.key.keysym.sym;

            // Do not return shift, control or alt as separate key events
            if (
                sdl_key == SDLK_LSHIFT ||
                sdl_key == SDLK_RSHIFT ||
                sdl_key == SDLK_LCTRL  ||
                sdl_key == SDLK_RCTRL  ||
                sdl_key == SDLK_LALT   ||
                sdl_key == SDLK_RALT)
            {
                continue;
            }

            Uint16 mod = SDL_GetModState();

            const bool IS_CTRL_HELD = mod & KMOD_CTRL;

            if (sdl_key >= SDLK_F1 && sdl_key <= SDLK_F9)
            {
                continue;
            }
            else // Not an F key
            {
                switch (sdl_key)
                {
                case SDLK_RETURN:
                case SDLK_RETURN2:
                case SDLK_KP_ENTER:
                    if (out.empty())
                    {
                        out = prev_cmd_;
                        cursor_pos = out.size();
                    }
                    else // A command is entered
                    {
                        prev_cmd_ = out;
                        is_done = true;
                        TRACE("Command sent by user: %s", out.c_str());
                        msg_history_.push_back(cmd_render_str);
                    }
                    break;

                case SDLK_ESCAPE:
                    out.clear();
                    cursor_pos = 0;
                    break;

                case SDLK_LEFT:
                    if (cursor_pos > 0)
                    {
                        if (IS_CTRL_HELD)
                        {
                            bool is_seek_done = false;

                            while (!is_seek_done)
                            {
                                --cursor_pos;

                                if (cursor_pos == 0)
                                {
                                    is_seek_done = true;
                                }
                                else // Cursor positon not zero
                                {
                                    const bool CUR_IS_SPACE = out[cursor_pos]       == ' ';
                                    const bool NXT_IS_SPACE = out[cursor_pos - 1]   == ' ';

                                    is_seek_done = !CUR_IS_SPACE && NXT_IS_SPACE;
                                }
                            }
                        }
                        else // Ctrl not held
                        {
                            --cursor_pos;
                        }
                    }
                    break;

                case SDLK_RIGHT:
                    if (cursor_pos < out.size())
                    {
                        if (IS_CTRL_HELD)
                        {
                            bool is_seek_done = false;
                            while (!is_seek_done)
                            {
                                ++cursor_pos;

                                if (cursor_pos == out.size())
                                {
                                    is_seek_done = true;
                                }
                                else // Cursor positon not at the right end
                                {
                                    const bool CUR_IS_SPACE = out[cursor_pos]       == ' ';
                                    const bool PRV_IS_SPACE = out[cursor_pos - 1]   == ' ';

                                    is_seek_done = CUR_IS_SPACE && !PRV_IS_SPACE;
                                }
                            }
                        }
                        else // Ctrl not held
                        {
                            ++cursor_pos;
                        }
                    }
                    break;

                case SDLK_HOME:
                    cursor_pos = 0;
                    break;

                case SDLK_END:
                    cursor_pos = out.size();
                    break;

                case SDLK_BACKSPACE:
                    if (!out.empty() && cursor_pos > 0)
                    {
                        out.erase(begin(out) + cursor_pos - 1);
                        --cursor_pos;
                    }
                    break;

                case SDLK_DELETE:
                    if (cursor_pos < out.size())
                    {
                        out.erase(begin(out) + cursor_pos);
                    }
                    break;

                case SDLK_c:
                    if (IS_CTRL_HELD)
                    {
                        out = "";
                        cursor_pos = 0;
                    }
                    break;

                default:
                    break;
                }
            }
        }
        break;

        case SDL_TEXTINPUT:
            out.insert(cursor_pos, sdl_event_.text.text);
            ++cursor_pos;
            break;

        default:
            break;

        } // End of event type switch
    } // End of while loop

    SDL_StopTextInput();
}

void sleep(const Uint32 DURATION)
{
    if (!is_inited_)
    {
        return;
    }

    if (DURATION == 1)
    {
        SDL_Delay(DURATION);
    }
    else // Duration > 1
    {
        const Uint32 WAIT_UNTIL = SDL_GetTicks() + DURATION;

        while (SDL_GetTicks() < WAIT_UNTIL)
        {
            SDL_PumpEvents();
        }
    }
}

} // io
