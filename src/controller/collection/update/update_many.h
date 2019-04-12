#ifndef CONTROLLER_UPDATE_MANY_H
#define CONTROLLER_UPDATE_MANY_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>
namespace iotdb {
namespace controller {
/**
* @brief update_many	: gets request content json and prepare it for
* update_many function in database namespace
* @param rep			: reply of http
* @param request		: request of http
*/
void update_many(http::server::reply &rep, http::server::request request);
} // controller
} // iotdb

#endif // CONTROLLER_UPDATE_MANY_H
