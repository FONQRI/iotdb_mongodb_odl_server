#ifndef DATABASE_USER_METHODS_H
#define DATABASE_USER_METHODS_H

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

std::string create_user(std::string username, std::string password,
			std::string primary_phone_number,
			std::string primary_email,
			optional_string_array phone_numbers,
			optional_string_array emails);

std::string get_user(std::string username, std::string password);

std::string
update_user(std::string username, std::string password,
		optional_string update_username, optional_string update_password,
		optional_string primary_phone_number, optional_string primary_email,
		optional_string_array phone_numbers, optional_string_array emails);

std::string delete_user(std::string username, std::string password);

std::string create_key(std::string username, std::string password,
			   std::string name, std::string database_name,
			   int valid_requests_number, int valid_read_size,
			   int valid_write_size, std::vector<std::string> access);

std::string
update_key(std::string username, std::string password, std::string name,
	   optional_string update_name, optional_string database_name,
	   optional_int valid_requests_number, optional_int request_per_day,
	   optional_int valid_read_size, optional_int valid_write_size,
	   optional_string_array access);

std::string get_user_key(std::string username, std::string name);

std::string get_user_keys(std::string username, std::string password);

std::string delete_user_key(std::string username, std::string password,
				std::string name);

std::string delete_user_keys(std::string username, std::string password);

std::string create_payment(std::string username, std::string password,
			   int main_value, optional_int discount_value,
			   optional_string discount_type, int expiration_time,
			   int request_numbers_per_day, int read_size,
			   int write_size);

std::string get_payments(std::string username, std::string password);

} // namespace database
} // namespace iotdb

#endif // DATABASE_USER_METHODS_H
