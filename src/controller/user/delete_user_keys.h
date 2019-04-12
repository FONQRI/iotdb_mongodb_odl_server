#ifndef CONTROLLER_DELETE_USER_KEYS_H
#define CONTROLLER_DELETE_USER_KEYS_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void delete_user_keys(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_DELETE_USER_KEYS_H
