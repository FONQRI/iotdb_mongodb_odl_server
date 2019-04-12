// header
#include "create_key.h"

// internal
#include "src/core/reply.h"
#include "src/database/user_methods.h"
#include "src/database/security/password.h"

// boost
#include <boost/optional.hpp>

// mongocxx
#include <mongocxx/exception/exception.hpp>

// std
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

void iotdb::controller::create_key(http::server::reply &rep,
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
			std::string password{""};
			for (auto &header : request.headers) {
				if (header.name == "username") {
					username = header.value;
				}
				else if (header.name == "password") {
					password = header.value;
				}
			}
			if (username.empty()) {
				std::string reply =
				core::reply::missing_item_error("username");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}
			else if (password.empty()) {
				std::string reply =
				core::reply::missing_item_error("password");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}

			// convert content to json
			bsoncxx::document::value request_document =
			bsoncxx::from_json(request.content);

			// name
			std::string name;
			try {
				name = request_document.view()["name"]
					   .get_utf8()
					   .value.to_string();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error("name");
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

			// database_name
			std::string database_name;
			try {
				database_name =
				request_document.view()["database_name"]
					.get_utf8()
					.value.to_string();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"database_name");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"database_name");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// valid_requests_number
			int valid_requests_number;
			try {
				valid_requests_number =
				request_document.view()["valid_requests_number"]
					.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"valid_requests_number");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"valid_requests_number");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// valid_read_size
			int valid_read_size;
			try {
				valid_read_size =
				request_document.view()["valid_read_size"]
					.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"valid_read_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"valid_read_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// valid_read_size
			int valid_write_size;
			try {
				valid_write_size =
				request_document.view()["valid_write_size"]
					.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"valid_write_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"valid_write_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get access array of request
			bsoncxx::array::view array;
			std::vector<std::string> access;
			try {
				array = request_document.view()["access"]
					.get_array()
					.value;

				// document vector
				for (auto &doc : array) {
					access.push_back(
					doc.get_utf8().value.to_string());
				}
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"access");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("access");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get reply from database
			auto reply = iotdb::database::create_key(
			username, password, name, database_name,
			valid_requests_number, valid_read_size,
			valid_write_size, access);

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
