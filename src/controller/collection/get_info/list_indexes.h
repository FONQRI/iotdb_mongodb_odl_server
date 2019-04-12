#ifndef LIST_INDEXES_H
#define LIST_INDEXES_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace controller {
void list_indexes(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb

#endif // LIST_INDEXES_H
