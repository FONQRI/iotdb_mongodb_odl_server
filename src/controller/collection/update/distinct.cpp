// header
#include "distinct.h"

// internal
#include "src/core/reply.h"
#include "src/database/collection_methods.h"
#include "src/database/security/password.h"

// boost
#include <boost/optional.hpp>

// mongocxx
#include <mongocxx/exception/exception.hpp>

// std
#include <iostream>
#include <string>
#include <vector>

void iotdb::controller::distinct(http::server::reply &rep,
				 http::server::request request)
{
	// add headers
	//	specifying content type as json
	http::server::header content_type;
	content_type.name = "Content-Type";
	content_type.value = "application/json;";

	//	specifying content charset as utf-8
	http::server::header charset;
	charset.name = "charset";
	charset.value = "utf-8";

	// putting headers into reply
	rep.headers.push_back(content_type);
	rep.headers.push_back(charset);

	try {
		// check if request is post
		if (request.method == "POST") {

			// get username of request
			std::string username{""};
			// get client key of request
			std::string client_key{""};
			for (auto &header : request.headers) {
				if (header.name == "username") {
					username = header.value;
				}
				else if (header.name == "client_key") {
					client_key = header.value;
				}
			}
			if (username.empty()) {
				std::string reply =
				core::reply::missing_item_error("username");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}
			else if (client_key.empty()) {
				std::string reply =
				core::reply::missing_item_error("client_key");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}

			// get database name and check client_key access
			std::string database_name{};
			std::string check_key_reply;
			if (!iotdb::database::security::password::check_key(
				username, client_key, check_key_reply)) {
				rep.content.append(check_key_reply.c_str(),
						   check_key_reply.size());
				return;
			}
			else {
				database_name = check_key_reply;
			}

			// convert content to json
			bsoncxx::document::value request_document =
			bsoncxx::from_json(request.content);

			// get query document from request document
			std::string name{""};

			try {

				name = request_document.view()["name"]
					   .get_utf8()
					   .value.to_string();
			}
			catch (std::exception &e) {

				// if username doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"replacement");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("name");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get filter document from request document
			bsoncxx::types::b_document filter;

			try {

				filter = request_document.view()["filter"]
					 .get_document();
			}
			catch (std::exception &e) {

				// if username doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"filter");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("filter");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get collation of request
			// https://docs.mongodb.com/manual/reference/collation/
			boost::optional<bsoncxx::types::b_document> collation;

			try {
				collation = request_document.view()["collation"]
						.get_document();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"collation");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get upsert from request document
			boost::optional<std::size_t> max_time;

			try {
				std::int32_t max_time_temp =
				request_document.view()["upsert"].get_bool();
				if (max_time_temp >= 0) {
					max_time = max_time_temp;
				}
				else {
					std::string reply =
					core::reply::wrong_item_type(
						"max_time is unsigned");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"max_time");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get reply from database
			auto reply =
			iotdb::database::distinct(username, database_name, name,
						  filter, collation, max_time);

			// write reply
			rep.content.append(reply.c_str(), reply.size());
		}
		else {
			// if request isn't post method
			std::string reply =
			core::reply::http_error("send post method");

			// write reply
			rep.content.append(reply.c_str(), reply.size());
		}
	}
	catch (std::exception &e) {

		// if execption happend in getting values or parsing json
		std::string reply =
		core::reply::wrong_request_content_type(e.what());

		// write reply
		rep.content.append(reply.c_str(), reply.size());
	}
}
