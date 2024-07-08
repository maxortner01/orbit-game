#pragma once

#include <exception>
#include <filesystem>
#include <iostream>

#define ASSERT(expr, msg) if (!(expr)) { std::cout << "Assertion hit: " << msg << "\n"; std::terminate(); }