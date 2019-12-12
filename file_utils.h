#pragma once

#include <vector>
#include <string>

void make_full_paths(std::vector<std::string>& files);
void remove_non_valid_paths(std::vector<std::string>& files);
void sort_names_and_remove_duplic(std::vector<std::string>& files);