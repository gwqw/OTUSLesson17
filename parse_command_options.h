#pragma once

#include <tuple>
#include <vector>
#include <string>

#include <boost/optional.hpp>

#include "hasher.h"

boost::optional<std::tuple<std::vector<std::string>, int, HashType>>
        parse_command_oprions(int argc, char* argv[]);