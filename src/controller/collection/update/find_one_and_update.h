#ifndef CONTROLLER_FIND_ONE_AND_UPDATE_H
#define CONTROLLER_FIND_ONE_AND_UPDATE_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>
namespace iotdb {
namespace controller {
/**
* @brief find_one_and_update	: gets request content json and prepare it for
* find_one_and_update function in database namespace
* @param rep			: reply of http
* @param request		: request of http
*/
void find_one_and_update(http::server::reply &rep,
			 http::server::request request);
} // controller
} // iotdb

#endif // CONTROLLER_FIND_ONE_AND_UPDATE_H
