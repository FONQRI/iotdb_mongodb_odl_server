#ifndef CONTROLLER_CREATE_PAYMENT_H
#define CONTROLLER_CREATE_PAYMENT_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {

void create_payment(http::server::reply &rep, http::server::request request);

} // namespace controller
} // namespace iotdb
#endif // CONTROLLER_CREATE_PAYMENT_H
