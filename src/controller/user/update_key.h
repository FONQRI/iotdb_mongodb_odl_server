#ifndef CONTROLLER_UPDATE_KEY_H
#define CONTROLLER_UPDATE_KEY_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void update_key(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb

#endif // CONTROLLER_UPDATE_KEY_H
