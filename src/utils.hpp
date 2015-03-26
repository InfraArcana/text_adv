#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string to_str(const int IN);

int to_int(const std::string& in);

void constr_in_range(const int MIN, int& val, const int MAX);
void constr_in_range(const double MIN, double& val, const double MAX);

// The following functions will first raise val to at least MIN, then lower val to at most MAX
int get_constr_in_range(const int MIN, const int VAL, const int MAX);
int get_constr_in_range(const double MIN, const double VAL, const double MAX);

#endif // UTILS_H
