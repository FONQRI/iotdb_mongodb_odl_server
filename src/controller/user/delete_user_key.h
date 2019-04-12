#ifndef CONTROLLER_DELETE_USER_KEY_H
#define CONTROLLER_DELETE_USER_KEY_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void delete_user_key(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_DELETE_USER_KEY_H
