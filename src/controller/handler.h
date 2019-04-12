#ifndef CONTROLLER_REQUEST_HANDLER_H
#define CONTROLLER_REQUEST_HANDLER_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>

namespace iotdb {
namespace core {
		/**
 * @brief The handler class : handle http request and map them into its controler
 */
class handler {
  public:
	handler(http::server::request request, http::server::reply &rep);

  private:
};
}
}
#endif // CONTROLLER_REQUEST_HANDLER_H
