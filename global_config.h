#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>

#define debug(x) std::cout << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << (#x) << " = " << x << std::endl
#define log(x) std::cout << std::filesystem::path(__FILE__).filename() << ' ' << __LINE__ << ": " << x << std::endl
