#ifndef FIND_AND_DELETE_ONE_H
#define FIND_AND_DELETE_ONE_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>

namespace iotdb {
namespace controller {

void find_one_and_delete(http::server::reply &rep,
			 http::server::request request);
} // namespace controller
} // namespace iotdb

#endif // CONTROLLER_FIND_AND_DELETE_ONE_H
