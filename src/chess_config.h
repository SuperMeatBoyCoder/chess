#ifndef CHESS_CONFIG_H
#define CHESS_CONFIG_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <map>

namespace Chess {

inline std::ofstream file_log("log.txt");

#define log(x) std::ofstream("log.txt") << x << std::endl
#define debug(x) file_log << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << (#x) << " = " << x << std::endl
}

#endif // CHESS_CONFIG_H