#ifndef CONTROLLER_FIND_H
#define CONTROLLER_FIND_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void find(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_FIND_H
