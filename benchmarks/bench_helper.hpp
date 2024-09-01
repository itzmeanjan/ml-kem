#pragma once
#include <algorithm>
#include <vector>

constexpr auto compute_min = [](const std::vector<double>& v) -> double { return *std::min_element(v.begin(), v.end()); };
constexpr auto compute_max = [](const std::vector<double>& v) -> double { return *std::max_element(v.begin(), v.end()); };
