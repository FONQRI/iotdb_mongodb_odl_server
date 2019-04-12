/**
 * reply.h
 * reply namespace contain reply functions to make json reply
 * @author b.sabaghi
 * @version 0.9 12/5/2018
 */

#ifndef core_REPLY_H
#define core_REPLY_H

#include <string>

namespace iotdb {
namespace core {
namespace reply {

std::string error(size_t code, std::string what);

std::string missing_item_error(std::string item_name);

std::string wrong_item_type(std::string item_name);

std::string answer(std::string answer);

std::string answer_not_found(std::string answer);

std::string answer_done();

std::string access_denied(std::string why);

std::string client_key_not_found(std::string client_key);

std::string key_does_not_have_access_to_function(std::string function_name);

std::string key_does_not_have_access_to_request();

std::string user_creation_failed(std::string why);

std::string user_update_failed(std::string why);

std::string user_does_not_exist(std::string which);

std::string wrong_request_content_type(std::string what);

std::string database_error(std::string what);

std::string duplicate_index(std::string what);

std::string http_error(std::string what);

} // namespace reply
} // namespace core
} // namespace iotdb
#endif // core_REPLY_H
