#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {
void aggregate(http::server::reply &rep, http::server::request request);
} // controller
} // iotdb
#endif // AGGREGATE_H
