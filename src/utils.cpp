#include "utils.hpp"

//#include <regex>
#include <algorithm>
#include <stdio.h>
#include <assert.h>

#include "cmn_data.hpp"

namespace utils
{

//std::vector<std::string> split_to_words(const std::string& str)
//{
//    std::regex re("\\s+");
//    return {std::sregex_token_iterator(str.begin(), str.end(), re, -1), std::sregex_token_iterator()};
//}

std::vector<std::string> split_str(const std::string& str, int delimiter(int))
{
    std::vector<std::string> result;

    const auto  it_end  = str.end();
    auto        it      = str.begin();

    while (it != it_end)
    {
        // Find start of next word
        it = std::find_if_not(it, it_end, delimiter);

        if (it == it_end)
        {
            // End of string reached
            break;
        }

        // Find end of word
        auto word_end_it = std::find_if(it, it_end, delimiter);

        result.push_back(std::string(it, word_end_it));
        it = word_end_it;
    }
    return result;
}

std::string to_str(const int IN)
{
    char str[15];
    sprintf(str, "%d", IN);
    return std::string(str);
}

int to_int(const std::string& in)
{
    const int   sign    = in[0] == '-' ? -1 : 1;
    size_t      offset  = sign == -1 ? 1 : 0;
    int         out     = 0;

    for (size_t i = offset; in[i] != '\0'; ++i)
    {
        out = (out * 10) + (in[i] - '0');
    }

    return out * sign;
}

void constr_in_range(const int MIN, int& val, const int MAX)
{
    if (MAX >= MIN)
    {
        val = std::min(MAX, std::max(val, MIN));
    }
}

void constr_in_range(const double MIN, double& val, const double MAX)
{
    if (MAX > MIN)
    {
        val = std::min(MAX, std::max(val, MIN));
    }
}

int get_constr_in_range(const int MIN, const int VAL, const int MAX)
{
    if (MAX < MIN)
    {
        return -1;
    }

    return std::min(MAX, std::max(VAL, MIN));
}

int get_constr_in_range(const double MIN, const double VAL, const double MAX)
{
    if (MAX < MIN)
    {
        return -1;
    }

    return std::min(MAX, std::max(VAL, MIN));
}

bool is_in_map(const Pos& p)
{
    return p.x >= 0 && p.y >= 0 && p.x < MAP_W && p.y < MAP_H;
}

}

namespace dir_utils
{

namespace
{

const std::string compass_dir_names[3][3] =
{
    {"NW", "N", "NE"},
    {"W",  "",  "E",},
    {"SW", "S", "SE"}
};

const double PI_DB            = 3.14159265;
const double ANGLE_45_DB      = 2 * PI_DB / 8;
const double ANGLE_45_HALF_DB = ANGLE_45_DB / 2.0;

const double edge[4] =
{
    ANGLE_45_HALF_DB + (ANGLE_45_DB * 0),
    ANGLE_45_HALF_DB + (ANGLE_45_DB * 1),
    ANGLE_45_HALF_DB + (ANGLE_45_DB * 2),
    ANGLE_45_HALF_DB + (ANGLE_45_DB * 3)
};

} //namespace

Dir get_dir(const Pos& offset)
{
    assert(offset.x >= -1 && offset.y >= -1 && offset.x <= 1 && offset.y <= 1);

    if (offset.y == -1)
    {
        return offset.x == -1 ? Dir::up_left :
               offset.x ==  0 ? Dir::up :
               offset.x ==  1 ? Dir::up_right :
               Dir::END;
    }

    if (offset.y == 0)
    {
        return offset.x == -1 ? Dir::left :
               offset.x ==  0 ? Dir::center :
               offset.x ==  1 ? Dir::right :
               Dir::END;
    }

    if (offset.y == 1)
    {
        return offset.x == -1 ? Dir::down_left :
               offset.x ==  0 ? Dir::down :
               offset.x ==  1 ? Dir::down_right :
               Dir::END;
    }

    return Dir::END;
}

Pos get_offset(const Dir dir)
{
    assert(dir != Dir::END);

    switch (dir)
    {
    case Dir::down_left:   return Pos(-1, 1);

    case Dir::down:       return Pos(0, 1);

    case Dir::down_right:  return Pos(1, 1);

    case Dir::left:       return Pos(-1, 0);

    case Dir::center:     return Pos(0, 0);

    case Dir::right:      return Pos(1, 0);

    case Dir::up_left:     return Pos(-1, -1);

    case Dir::up:         return Pos(0, -1);

    case Dir::up_right:    return Pos(1, -1);

    case Dir::END:        return Pos(0, 0);
    }

    return Pos(0, 0);
}

//Pos get_rnd_adj_pos(const Pos& origin, const bool IS_ORIGIN_ALLOWED)
//{
//    if (IS_ORIGIN_ALLOWED)
//    {
//        const int ELEMENT = rnd::range(0, dir_list.size()); //Treat origin as extra element
//        return ELEMENT == int(dir_list.size()) ? origin : (origin + dir_list[ELEMENT]);
//    }
//    else
//    {
//        return origin + dir_list[rnd::range(0, dir_list.size() - 1)];
//    }
//}

void get_compass_dir_name(const Pos& from_pos, const Pos& to_pos, std::string& str_ref)
{
    str_ref = "";

    const Pos offset(to_pos - from_pos);
    const double ANGLE_DB = atan2(-offset.y, offset.x);

    if (ANGLE_DB        <  -edge[2] && ANGLE_DB >  -edge[3])
    {
        str_ref = "SW";
    }
    else if (ANGLE_DB <= -edge[1] && ANGLE_DB >= -edge[2])
    {
        str_ref = "S";
    }
    else if (ANGLE_DB <  -edge[0] && ANGLE_DB >  -edge[1])
    {
        str_ref = "SE";
    }
    else if (ANGLE_DB >= -edge[0] && ANGLE_DB <=  edge[0])
    {
        str_ref = "E";
    }
    else if (ANGLE_DB >   edge[0] && ANGLE_DB <   edge[1])
    {
        str_ref = "NE";
    }
    else if (ANGLE_DB >=  edge[1] && ANGLE_DB <=  edge[2])
    {
        str_ref = "N";
    }
    else if (ANGLE_DB >   edge[2] && ANGLE_DB <   edge[3])
    {
        str_ref = "NW";
    }
    else
    {
        str_ref = "W";
    }
}

void get_compass_dir_name(const Dir dir, std::string& str_ref)
{

    const Pos& offset = get_offset(dir);
    str_ref = compass_dir_names[offset.x + 1][offset.y + 1];
}

void get_compass_dir_name(const Pos& offset, std::string& str_ref)
{
    str_ref = compass_dir_names[offset.x + 1][offset.y + 1];
}

bool is_cardinal(const Pos& d)
{
    assert(d.x >= -1 && d.x <= 1 && d.y >= -1 && d.y <= 1);
    return d != 0 && (d.x == 0 || d.y == 0);
}

} //dir_utils

