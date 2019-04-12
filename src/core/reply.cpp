#include "reply.h"

/**
 * @brief error
 * @param code
 * @param what
 * @return
 */
std::string iotdb::core::reply::error(size_t code, std::string what)
{
	return "{\"isSuccessful\":false,\"code\":" + std::to_string(code) +
	   ",\"Message\":\"" + what + "\"}";
}

/**
 * @brief enter_item_error	: create error reply json for missing item in
 * query
 * @param item_name			: missing item name
 * @return					: json of reply
 */
std::string iotdb::core::reply::missing_item_error(std::string item_name)
{
	return "{\"isSuccessful\":false,\"code\":200,\"Message\":\"missing " +
	   item_name + "\"}";
}

/**
 * @brief wrong_item_type	: create error reply json for wrong item in
 * query
 * @param item_name			: missing item name
 * @return					: json of reply
 */
std::string iotdb::core::reply::wrong_item_type(std::string item_name)
{
	return "{\"isSuccessful\":false,\"code\":300,\"Message\":\"wrong type " +
	   item_name + "\"}";
}

/**
 * @brief answer	: create answer reply json
 * @param answer	: answer message
 * @return			: json of reply
 */
std::string iotdb::core::reply::answer(std::string answer)
{
	return "{\"isSuccessful\":true,\"code\":100,\"Response\":" + answer + "}";
}

/**
 * @brief answer_not_found
 * @param answer
 * @return
 */
std::string iotdb::core::reply::answer_not_found(std::string answer)
{
	return "{\"isSuccessful\":true,\"code\":101,\"Response\":\"" + answer +
	   "\"}";
}

/**
 * @brief answer_done	: write done answer
 * @return				: done reply sjon
 */
std::string iotdb::core::reply::answer_done()
{
	return "{\"isSuccessful\":true,\"code\":100}";
}

/**
 * @brief access_denied : client_key access denied
 * @param why			: reason of access denied
 * @return				: json for answer
 */
std::string iotdb::core::reply::access_denied(std::string why)
{
	return "{\"isSuccessful\":false,\"code\":400,\"Message\":\" access "
	   "denied " +
	   why + "\"}";
}

/**
 * @brief client_key_not_found
 * @param client_key
 * @return						: json error
 */
std::string iotdb::core::reply::client_key_not_found(std::string client_key)
{
	return "{\"isSuccessful\":false,\"code\":500,\"Message\":\" client key not "
	   "found : " +
	   client_key + "\"}";
}

/**
 * @brief user_creation_failed
 * @param why
 * @return : json error
 */
std::string iotdb::core::reply::key_does_not_have_access_to_function(
	std::string function_name)
{
	return "{\"isSuccessful\":false,\"code\":501,\"Message\":\" The key does "
	   "not have access to the function : " +
	   function_name + "\"}";
}

std::string iotdb::core::reply::key_does_not_have_access_to_request()
{
	return "{\"isSuccessful\":false,\"code\":502,\"Message\":\" The key does "
	   "not have access to request : \"}";
}

std::string iotdb::core::reply::user_creation_failed(std::string why)
{
	return "{\"isSuccessful\":false,\"code\":610,\"Message\":\" user creation "
	   "failed " +
	   why + "\"}";
}

std::string iotdb::core::reply::user_update_failed(std::string why)
{
	return "{\"isSuccessful\":false,\"code\":620,\"Message\":\" user update "
	   "failed " +
	   why + "\"}";
}

std::string iotdb::core::reply::user_does_not_exist(std::string which)
{
	return "{\"isSuccessful\":false,\"code\":601,\"Message\":\" User does not "
	   "exist : " +
	   which + "\"}";
}

/**
 * @brief wrong_request_content_type
 * @param what
 * @return : json error
 */
std::string iotdb::core::reply::wrong_request_content_type(std::string what)
{
	return "{\"isSuccessful\":false,\"code\":800,\"Message\":\"" + what + "\"}";
}

/**
 * @brief database_error
 * @param what
 * @return : json error
 */
std::string iotdb::core::reply::database_error(std::string what)
{
	return "{\"isSuccessful\":false,\"code\":900,\"Message\":\"" + what + "\"}";
}

std::string iotdb::core::reply::duplicate_index(std::string what)
{
	return "{\"isSuccessful\":false,\"code\":601,\"Message\":\"" + what + "\"}";
}

/**
 * @brief http_error
 * @param what
 * @return : json error
 */
std::string iotdb::core::reply::http_error(std::string what)
{
	return "{\"isSuccessful\":false,\"code\":700,\"Message\":\"" + what + "\"}";
}
