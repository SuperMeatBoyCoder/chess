#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>


#define debug(x) std::cout << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << (#x) << " = " << x << std::endl
#define log(x) std::cout << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << x << std::endl

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
