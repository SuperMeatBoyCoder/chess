#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cassert>

namespace Chess {
std::ofstream file_log("log.txt");

#define log(x) << x << std::endl
#define debug(x) file_log << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << (#x) << " = " << x << std::endl

template <class T1, class T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& this_pair)
{
    out << '(' << this_pair.first << ' ' << this_pair.second << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& this_vector)
{
    for (size_t i = 0; i < this_vector.size(); i++) {
        out << this_vector[i] << ' ';
    }
    return out;
}

const int NORMAL_MOVE = 0;
const int CASTLE = 1;
const int EN_PASSANT = 2;
const int PROMOTION = 3;
}