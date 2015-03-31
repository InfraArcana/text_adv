#ifndef CMN_TYPES_H
#define CMN_TYPES_H

#include <string>

#include "colors.hpp"

struct Clr_str
{
    Clr_str() : str(""), clr(clr_black) {}

    Clr_str(const std::string& text, const Clr& color) :
        str(text), clr(color) {}

    Clr_str& operator=(const Clr_str& other)
    {
        str = other.str;
        clr = other.clr;
        return *this;
    }

    std::string str;
    Clr         clr;
};

struct Pos
{
    Pos() : x(0), y(0) {}
    Pos(const int X, const int Y) : x(X), y(Y) {}
    Pos(const Pos& p) : x(p.x), y(p.y) {}
    Pos(const int V) : x(V), y(V) {}

    Pos& operator=(const Pos& p) {x = p.x; y = p.y; return *this;}

    Pos& operator/=(const int  V)   {x /= V;   y /= V;      return *this;}
    Pos& operator/=(const Pos& o)   {x /= o.x; y /= o.y;    return *this;}
    Pos& operator+=(const Pos& o)   {x += o.x; y += o.y;    return *this;}
    Pos& operator-=(const Pos& o)   {x -= o.x; y -= o.y;    return *this;}
    Pos operator+(const Pos& p)     const {return Pos(x + p.x,  y + p.y);}
    Pos operator+(const int V)      const {return Pos(x + V,    y + V);}
    Pos operator-(const Pos& p)     const {return Pos(x - p.x,  y - p.y);}
    Pos operator-(const int V)      const {return Pos(x - V,    y - V);}
    Pos operator/(const int V)      const {return Pos(x / V,    y / V);}
    Pos operator/(const Pos& p)     const {return Pos(x / p.x,  y / p.y);}
    Pos operator*(const int V)      const {return Pos(x * V,    y * V);}
    Pos operator*(const Pos& p)     const {return Pos(x * p.x,  y * p.y);}
    bool operator==(const Pos& p)   const {return x == p.x  && y == p.y;}
    bool operator!=(const Pos& p)   const {return x != p.x  || y != p.y;}
    bool operator!=(const int V)    const {return x != V    || y != V;}
    bool operator>(const Pos& p)    const {return x > p.x   && y > p.y;}
    bool operator>(const int  V)    const {return x > V     && y > V;}
    bool operator<(const Pos& p)    const {return x < p.x   && y < p.y;}
    bool operator<(const int  V)    const {return x < V     && y < V;}
    bool operator>=(const Pos&  p)  const {return x >= p.x  && y >= p.y;}
    bool operator>=(const int   V)  const {return x >= V    && y >= V;}
    bool operator<=(const Pos&  p)  const {return x <= p.x  && y <= p.y;}
    bool operator<=(const int   V)  const {return x <= V    && y <= V;}

    Pos get_signs() const
    {
        return Pos(x == 0 ? 0 : x > 0 ? 1 : -1,
                   y == 0 ? 0 : y > 0 ? 1 : -1);
    }

    Pos get_pos_with_offset(const Pos& d)   const {return Pos(x + d.x, y + d.y);}
    Pos get_pos_with_x_offset(const int DX) const {return Pos(x + DX, y);}
    Pos get_pos_with_y_offset(const int DY) const {return Pos(x, y + DY);}

    void set(const int x_, const int y_)  {x = x_;  y = y_;}
    void set(const Pos& p)                {x = p.x; y = p.y;}

    void swap(Pos& p) {Pos p_temp(p); p = *this; set(p_temp);}

    int x, y;
};

struct Rect
{
    Rect() : p0(Pos()), p1(Pos()) {}

    Rect(const Pos& p0_, const Pos& p1_) :  p0(p0_), p1(p1_) {}

    Rect(const int X0, const int Y0, const int X1, const int Y1) :
        p0(Pos(X0, Y0)), p1(Pos(X1, Y1)) {}

    Rect(const Rect& r) : p0(r.p0), p1(r.p1) {}

    int get_w()             const {return p1.x - p0.x + 1;}
    int get_h()             const {return p1.y - p0.y + 1;}
    Pos get_dims()          const {return {get_w(), get_h()};}
    int get_min_dim()       const {return std::min(get_w(), get_h());}
    int get_max_dim()       const {return std::max(get_w(), get_h());}
    Pos get_center_pos()    const {return ((p1 + p0) / 2);}

    Pos p0;
    Pos p1;
};

//struct Dice_param
//{
//    Dice_param() : rolls(0), sides(0), plus(0) {}
//
//    Dice_param(const int ROLLS, const int SIDES, const int PLUS = 0) :
//        rolls(ROLLS), sides(SIDES), plus(PLUS) {}
//
//    Dice_param(const Dice_param& other) :
//        rolls(other.rolls), sides(other.sides), plus(other.plus) {}
//
//    Dice_param& operator=(const Dice_param& other)
//    {
//        rolls = other.rolls;
//        sides = other.sides;
//        plus  = other.plus;
//        return *this;
//    }
//
//    int get_highest() const {return (rolls * sides) + plus;}
//
//    int rolls, sides, plus;
//};

struct Range
{
    Range() : lower(-1), upper(-1) {}

    Range(const int LOWER, const int UPPER) : lower(LOWER), upper(UPPER) {}

    Range(const Range& other) : lower(other.lower), upper(other.upper) {}

    int lower, upper;
};

struct Fraction
{
    Fraction() : numerator(-1), denominator(-1) {}

    Fraction(const int NUMERATOR, const int DENOMINATOR) :
        numerator(NUMERATOR), denominator(DENOMINATOR) {}

    void set(const int NUMERATOR, const int DENOMINATOR)
    {
        numerator   = NUMERATOR;
        denominator = DENOMINATOR;
    }

    int numerator, denominator;
};

enum class Dir
{
    down_left   = 1,
    down        = 2,
    down_right  = 3,
    left        = 4,
    center      = 5,
    right       = 6,
    up_left     = 7,
    up          = 8,
    up_right    = 9,
    END
};

enum class Vertical_dir {up, down};

#endif // CMN_TYPES_H
