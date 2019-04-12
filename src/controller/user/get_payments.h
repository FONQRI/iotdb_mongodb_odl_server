#ifndef CONTROLLER_GET_PAYMENTS_H
#define CONTROLLER_GET_PAYMENTS_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void get_payments(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb

#endif // CONTROLLER_GET_PAYMENTS_H
