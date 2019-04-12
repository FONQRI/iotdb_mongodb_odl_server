#ifndef CONTROLLER_COUNT_H
#define CONTROLLER_COUNT_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {
void count(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_COUNT_H
