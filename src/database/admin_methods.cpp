// header
#include "admin_methods.h"

// std
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// internal
#include "core/reply.h"

// mongocxx
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/v_noabi/mongocxx/client.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::sub_array;
using bsoncxx::builder::basic::make_document;

// std::string iotdb::database::admin::get_user(std::string username)
//{

//	// reply message
//	std::string reply;

//	// create connection
//	mongocxx::client connection{mongocxx::uri{}};

//	// database
//	mongocxx::database database = connection["server_admin"];
//	// create collection
//	auto collection = database["users"];

//	// check if user dose not exist
//	try {
//		bsoncxx::builder::basic::document filter_document{};

//		filter_document.append(kvp("username", username));

//		auto cursor = collection.find_one(filter_document.view());
//		// return seccess message
//		return core::reply::answer(bsoncxx::to_json(cursor.value()));
//	}
//	catch (const mongocxx::exception &e) {
//		// create json from error
//		return core::reply::wrong_request_content_type(e.what());
//	}

//	return reply;
//}

// std::string iotdb::database::admin::increase_request(std::string username,
//							 std::string name,
//							 int request_per_day)
//{
//	// reply message
//	std::string reply;

//	// create connection
//	mongocxx::client connection{mongocxx::uri{}};

//	// database
//	auto database = connection["server_admin"];

//	// create collection
//	auto collection = database["users"];

//	try {

//		bsoncxx::builder::basic::document find_document{};

//		find_document.append(kvp("username", username));
//		find_document.append(kvp("keys.name", name));

//		bsoncxx::builder::basic::document key_document{};

//		bsoncxx::builder::basic::document update_document{};

//		update_document.append(kvp(
//		"$set",
//		make_document(kvp("keys.$.request_per_day", request_per_day))));

//		// create cursor bu qyery and options
//		collection.update_one({find_document}, {update_document});

//		// return seccess message
//		return core::reply::answer_done();
//	}
//	catch (const mongocxx::exception &e) {
//		// create json from error
//		return core::reply::user_creation_failed(e.what());
//	}

//	return reply;
//}

bool iotdb::database::admin::m_check_key_database(std::string username,
						  std::string key,
						  std::string &reply,
						  std::string function_name)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	mongocxx::database database = connection["server_admin"];
	// create collection
	auto collection = database["users"];

	try {
		bsoncxx::builder::basic::document command_document{};
		command_document.append(kvp(
		"eval",
		"function(username, key, function_name, data_size) {"
		"var doc = db.users.findOne( { username : username } );"

		// check if user exists
		"if(doc === null){"
		"return {isSuccessful:false,code:601,Message:\" User does not "
		"exist :\" + username }"
		"}"

		// find key
		"var key_found = doc.keys.find(function(element) {"
		"	return element.name === key;"
		"	});"

		// check if key exists
		"if(key_found === undefined){"
		"return {isSuccessful:false,code:500,Message:\" "
		"client key not "
		"found :  \"+ key}"
		"}"

		// check access of key
		"var has_access = "
		"key_found.access_level.find(function(element) {"
		"	return element === function_name;"
		"	});"

		// check if has access
		"if(has_access === undefined){"
		"return {isSuccessful:false,code:501,Message:\" The key does "
		"not have access to the function : \" + function_name}"
		"}"

		// check limitations and increase request numbers
		"if(key_found.valid_requests_number > "
		"key_found.request_per_day){"
		"key_found.request_per_day = key_found.request_per_day + 1;"
		"} else {"
		"return {isSuccessful:false,code:502,Message:\" The key does "
		"not have access to request : \"}"
		"}"
		"db.users.save(doc);"

		"return {isSuccessful:true,Response:key_found};"
		"}"));
		bsoncxx::array::view array;

		command_document.append(kvp("args", [&](sub_array child) {
		child.append(username);
		child.append(key);
		child.append(function_name);
		}));
		auto doc = database.run_command({command_document});
		if (doc.view()["retval"]
			.get_document()
			.view()["isSuccessful"]
			.get_bool()) {
			reply = doc.view()["retval"]
				.get_document()
				.view()["Response"]
				.get_document()
				.view()["database_name"]
				.get_utf8()
				.value.to_string();

			return true;
		}
		else {
			reply = bsoncxx::to_json(
			doc.view()["retval"].get_document().view());
			return false;
		}
		std::clog << bsoncxx::to_json(doc.view());
	}
	catch (std::exception &e) {
		std::clog << "run command " << e.what() << std::endl;
	}
	return "";
}

bool iotdb::database::admin::m_check_key(std::string username,
					 std::string request_key_name,
					 std::string &reply,
					 std::string function_name)
{
	//	// reply message
	//	std::string reply;

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	mongocxx::database database = connection["server_admin"];
	// create collection
	auto collection = database["users"];

	// check if user dose not exist
	try {
		bsoncxx::builder::basic::document filter_document{};

		filter_document.append(kvp("username", username));

		auto cursor = collection.find_one(filter_document.view());
		// return seccess message
		reply = core::reply::answer(bsoncxx::to_json(cursor.value()));
	}
	catch (const mongocxx::exception &e) {
		// create json from error
		reply = core::reply::wrong_request_content_type(e.what());
	}

	// get user from database
	bsoncxx::document::value user_reply_document = bsoncxx::from_json(reply);

	if (!user_reply_document.view()["isSuccessful"].get_bool()) {
		// user not found
		reply = core::reply::user_does_not_exist(username);
		return false;
	}

	//	std::clog << bsoncxx::to_json(user_reply_document) << std::endl;
	bsoncxx::types::b_document user_document =
	user_reply_document.view()["Response"].get_document();

	// get query document of request
	try {
		bsoncxx::array::view keys_array;

		keys_array = user_document.view()["keys"].get_array().value;

		auto key = std::find_if(keys_array.begin(), keys_array.end(),
					[&request_key_name](auto &key) {
					return key.get_document()
						   .view()["name"]
						   .get_utf8()
						   .value.to_string() ==
						   request_key_name;
					});

		// check if key found
		if (key == keys_array.end()) {

			// key not found
			reply = core::reply::client_key_not_found(request_key_name);
			return false;
		}

		// check if user has access to this function
		auto key_document = key->get_document().view();

		bsoncxx::array::view access_array;

		access_array = key_document["access_level"].get_array().value;

		auto has_access = std::find_if(
		access_array.begin(), access_array.end(),
		[&function_name](auto &access) {
			return access.get_utf8().value.to_string() == function_name;
		});

		// check if key has access
		if (has_access == access_array.end()) {

			// key does not have access to function
			reply = core::reply::key_does_not_have_access_to_function(
			function_name);
			return false;
		}

		// TODO P[0] check limitation and update request_per day
		if (key_document["request_per_day"].get_int32() >=
			key_document["valid_requests_number"].get_int32()) {

			// limit request number per day
			reply = core::reply::key_does_not_have_access_to_request();
			return false;
		}
		else {
			// update request_per_day + 1

			try {

				bsoncxx::builder::basic::document find_document{};

				find_document.append(kvp("username", username));
				find_document.append(
				kvp("keys.name", key_document["name"]
							 .get_utf8()
							 .value.to_string()));

				bsoncxx::builder::basic::document key_document{};

				bsoncxx::builder::basic::document update_document{};

				update_document.append(
				kvp("$inc", make_document(kvp(
						"keys.$.request_per_day", 1))));

				// create cursor bu qyery and options
				collection.update_one({find_document},
						  {update_document});

				// return seccess message
			}
			catch (const mongocxx::exception &e) {
				// create json from error
			}
		}

		reply = key_document["database_name"].get_utf8().value.to_string();
	}
	catch (std::exception &e) {
		reply = core::reply::client_key_not_found(request_key_name);
		return false;
	}

	if (reply != "") {
		return true;
	}
	else {
		reply = core::reply::client_key_not_found(request_key_name);
		return false;
	}

	return false;
}
