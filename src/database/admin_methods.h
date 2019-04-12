#ifndef DATABASE_ADMIN_METHODS_H
#define DATABASE_ADMIN_METHODS_H

// mongo includes
#include <bsoncxx/array/element.hpp>
#include <bsoncxx/array/value.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/array/view_or_value.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/model/insert_one.hpp>
#include <mongocxx/options/count.hpp>
#include <mongocxx/result/insert_one.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

// boost
#include <boost/optional.hpp>

// typedefs
typedef boost::optional<bool> optional_bool;
typedef boost::optional<bsoncxx::types::b_document> optional_ducument;
typedef boost::optional<std::string> optional_string;
typedef boost::optional<std::vector<std::string>> optional_string_array;
typedef boost::optional<int> optional_int;
typedef boost::optional<size_t> optional_size;

namespace iotdb {
namespace database {
namespace admin {
// std::string get_user(std::string username);

// std::string increase_request(std::string username, std::string name,
//				 int request_per_day);

bool m_check_key_database(std::string username, std::string key,
			  std::string &reply, std::string function_name);

bool m_check_key(std::string username, std::string request_key_name,
		 std::string &reply, std::string function_name);
}
} // iotdb
} // database

#endif // DATABASE_ADMIN_METHODS_H
