#pragma once

#include <iostream>

#define LOG(TYPE, REASON) std::cout << "[" << TYPE << "] " << REASON << std::endl

#define ERROR(TYPE, REASON) std::cerr << "[" << TYPE << "] " << REASON << std::endl