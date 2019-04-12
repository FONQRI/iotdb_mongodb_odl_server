#ifndef CONTROLLER_CREATE_INDEX_H
#define CONTROLLER_CREATE_INDEX_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>

namespace iotdb {
namespace controller {
/**
* @brief create_index	: create index <a href =
* "https://docs.mongodb.com/manual/reference/method/db.collection.createIndex/">
* create index mongodb <a>
* @param rep			: reply of http
* @param request		: request of http
*/
void create_index(http::server::reply &rep, http::server::request request);

} // controller
} // iotdb
#endif // CONTROLLER_CREATE_INDEX_H
