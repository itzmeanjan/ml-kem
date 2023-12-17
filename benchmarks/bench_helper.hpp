#pragma once
#include <algorithm>
#include <vector>

const auto compute_min = [](const std::vector<double>& v) -> double { return *std::min_element(v.begin(), v.end()); };
const auto compute_max = [](const std::vector<double>& v) -> double { return *std::max_element(v.begin(), v.end()); };
