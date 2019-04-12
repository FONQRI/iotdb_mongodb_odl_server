#ifndef CONTROLLER_NAME_H
#define CONTROLLER_NAME_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {
void name(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_NAME_H
