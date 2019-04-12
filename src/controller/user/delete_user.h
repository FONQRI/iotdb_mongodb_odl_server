#ifndef CONTROLLER_DELETE_USER_H
#define CONTROLLER_DELETE_USER_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void delete_user(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_DELETE_USER_H
