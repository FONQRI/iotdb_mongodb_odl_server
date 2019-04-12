#ifndef CONTROLLER_HOMEPAGECONTROLLER_H
#define CONTROLLER_HOMEPAGECONTROLLER_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"
#include <string>

namespace iotdb {
namespace core {

class home_page_controller {
  public:
	home_page_controller(http::server::reply &rep,
			 http::server::request request);
};
}
}
#endif // CONTROLLER_HOMEPAGECONTROLLER_H
