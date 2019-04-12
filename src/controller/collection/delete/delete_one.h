/**
  @link
  https://docs.mongodb.com/manual/reference/method/db.collection.deleteOne/#db.collection.deleteOne
  */

#ifndef CONTROLLER_DELETE_ONE_H
#define CONTROLLER_DELETE_ONE_H

#include "src/server/reply.hpp"
#include "src/server/request.hpp"

#include <string>

namespace iotdb {
namespace controller {

void delete_one(http::server::reply &rep, http::server::request request);
} // namespace controller
} // namespace iotdb

#endif // CONTROLLER_DELETE_ONE_H
