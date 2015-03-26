#include "utils.hpp"

#include <stdio.h>
#include <assert.h>

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
