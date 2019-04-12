#ifndef CONTROLLER_INSERT_ONE_CONTROLLER_H
#define CONTROLLER_INSERT_ONE_CONTROLLER_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>
namespace iotdb {
namespace controller {
/**
* @brief insert_one	: gets request content json and prepare it for
* insert_one function in database namespace
* @param rep			: reply of http
* @param request		: request of http
*/
void insert_one(http::server::reply &rep, http::server::request request);
} // controller
} // iotdb

#endif // CONTROLLER_INSERT_ONE_CONTROLLER_H
