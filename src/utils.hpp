#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "cmn_types.hpp"

namespace utils
{

std::string to_str(const int IN);

int to_int(const std::string& in);

void constr_in_range(const int MIN, int& val, const int MAX);
void constr_in_range(const double MIN, double& val, const double MAX);

// The following functions will first raise val to at least MIN, then lower val to at most MAX
int get_constr_in_range(const int MIN, const int VAL, const int MAX);
int get_constr_in_range(const double MIN, const double VAL, const double MAX);

bool is_in_map(const Pos& p);

}

namespace dir_utils
{

//Useful lists to iterate over
const std::vector<Pos> cardinal_list
{
    Pos(-1, 0), Pos(1, 0), Pos(0, -1), Pos(0, 1)
};

const std::vector<Pos> dir_list
{
    Pos(-1,  0), Pos(1, 0), Pos(0, -1), Pos(0, 1),
    Pos(-1, -1), Pos(-1, 1), Pos(1, -1), Pos(1, 1)
};

Dir get_dir(const Pos& offset);

Pos get_offset(const Dir dir);

//Pos get_rnd_adj_pos(const Pos& origin, const bool IS_ORIGIN_ALLOWED);

void get_compass_dir_name(const Pos& from_pos, const Pos& to_pos, std::string& str_ref);

void get_compass_dir_name(const Dir dir, std::string& str_ref);

void get_compass_dir_name(const Pos& offset, std::string& str_ref);

bool is_cardinal(const Pos& d);

} //dir_utils

#endif // UTILS_H
