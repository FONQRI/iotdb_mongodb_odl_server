#ifndef TOOLS_H
#define TOOLS_H

// boost
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cassert>
#include <exception>

// std
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace iotdb {
namespace core {
namespace tools {
std::string add_date_time_to_query(std::string request);
std::string add_date_time_to_querys(std::string request);
}
}
}

#endif // TOOLS_H
