#include "io.hpp"

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
SDL_Event       sdl_event_;

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

    TRACE("Setting up rendering window");

    const std::string title = "Text Adventure " + game_version_str;

    const int SCR_W = 640; // config::get_screen_px_w();
    const int SCR_H = 480; // config::get_screen_px_h();

    sdl_window_ = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCR_W, SCR_H,
                                   SDL_WINDOW_SHOWN);

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

    load_font();

    is_inited_ = true;

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

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

void draw_cursor(const std::string& text, const Pos& text_pos, const size_t cursor_idx)
{
    assert(cursor_idx <= text.size());

    set_render_clr(clr_black);

    int cursor_x = text_pos.x;

    for (size_t i = 0; i < cursor_idx; ++i)
    {
        int advance = 0;
        TTF_GlyphMetrics(font_, text[i], nullptr, nullptr, nullptr, nullptr, &advance);
        cursor_x += advance;
    }

    const int LINE_H = TTF_FontLineSkip(font_);

    const int Y_PADDING = LINE_H / 10;

    SDL_RenderDrawLine(sdl_renderer_,
                       cursor_x,
                       text_pos.y + Y_PADDING,
                       cursor_x,
                       text_pos.y + LINE_H - Y_PADDING);
}

void draw_text(const std::string& str, const Pos& pos, const Clr& clr, const Clr& bg_clr)
{
    if (is_inited_)
    {
        if (str.empty())
        {
            return;
        }

        SDL_Surface* font_srf       = TTF_RenderText_Blended(font_, str.c_str(), clr);
        SDL_Texture* font_texture   = SDL_CreateTextureFromSurface(sdl_renderer_, font_srf);

        SDL_Rect sdl_dst_rect {pos.x, pos.y, font_srf->w, font_srf->h};

        SDL_FreeSurface(font_srf);

        SDL_RenderCopy(sdl_renderer_, font_texture, nullptr, &sdl_dst_rect);
    }
}

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

void get_cmd(std::string& cmd_txt)
{
    if (!is_inited_)
    {
        return;
    }

    cmd_txt = "";

    size_t cursor_pos = 0;

    SDL_StartTextInput();

    bool is_done = false;

    while (!is_done)
    {
        // Draw
        clear_screen();

        const Pos text_pos(1, 1);

        const std::string prompt_text = "> ";

        const std::string render_text = prompt_text + cmd_txt;

        draw_text(render_text, text_pos, clr_black);

        const Uint32 CURSOR_PERIOD = 1000;

        if ((SDL_GetTicks() % CURSOR_PERIOD) > (CURSOR_PERIOD / 2))
        {
            draw_cursor(render_text, text_pos, prompt_text.size() + cursor_pos);
        }

        update_screen();

        // Sleep for a moment so the process doesn't claim all CPU time
        sleep(1);

        // Get input
        const bool DID_POLL_EVENT = SDL_PollEvent(&sdl_event_);

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
                // TODO: Draw
                break;

            default:
                break;
            }
        }
        break;

        case SDL_QUIT:
            cmd_txt = "";
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
                    is_done = true;
                    TRACE("Command sent by user: %s", cmd_txt.c_str());
                    break;

                case SDLK_LEFT:
                    --cursor_pos;
                    break;

                case SDLK_RIGHT:
                    ++cursor_pos;
                    break;

                case SDLK_HOME:
                    cursor_pos = 0;
                    break;

                case SDLK_END:
                    cursor_pos = cmd_txt.size();
                    break;

                default:
                    break;
                }
            }
        }
        break;

        case SDL_TEXTINPUT:
            cmd_txt.insert(cursor_pos, sdl_event_.text.text);
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
    if (is_inited_)
    {
        if (DURATION == 1)
        {
            SDL_Delay(DURATION);
        }
        else
        {
            const Uint32 WAIT_UNTIL = SDL_GetTicks() + DURATION;

            while (SDL_GetTicks() < WAIT_UNTIL)
            {
                SDL_PumpEvents();
            }
        }
    }
}

} // io
