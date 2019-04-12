#ifndef CONTROLLER_DROP_H
#define CONTROLLER_DROP_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>
namespace iotdb {
namespace controller {

void drop(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb

#endif // CONTROLLER_DROP_H
