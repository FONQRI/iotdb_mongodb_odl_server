#ifndef CONTROLLER_FIND_ONE_CONTROLLER_H
#define CONTROLLER_FIND_ONE_CONTROLLER_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void find_one(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_FIND_ONE_CONTROLLER_H
