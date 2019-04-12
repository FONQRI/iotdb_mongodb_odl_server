// header
#include "create_payment.h"

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

void iotdb::controller::create_payment(http::server::reply &rep,
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

			// main_value
			size_t main_value;
			try {
				main_value = request_document.view()["main_value"]
						 .get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"main_value");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"main_value");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// discount_value
			optional_int discount_value;
			try {
				discount_value =
				request_document.view()["discount_value"]
					.get_int32();
			}
			catch (std::exception &e) {

				// optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"discount_value");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// discount_type
			optional_string discount_type;
			try {
				discount_type =
				request_document.view()["discount_type"]
					.get_utf8()
					.value.to_string();
			}
			catch (std::exception &e) {

				// optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"discount_type");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// expiration_time
			size_t expiration_time;
			try {
				expiration_time =
				request_document.view()["expiration_time"]
					.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"expiration_time");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"expiration_time");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// request_numbers_per_day
			size_t request_numbers_per_day;
			try {
				request_numbers_per_day =
				request_document
					.view()["request_numbers_per_day"]
					.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"request_numbers_per_day");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"request_numbers_per_day");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// read_size
			size_t read_size;
			try {
				read_size = request_document.view()["read_size"]
						.get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"read_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"read_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// read_size
			size_t write_size;
			try {
				write_size = request_document.view()["write_size"]
						 .get_int32();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"write_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"write_size");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get reply from database
			auto reply = iotdb::database::create_payment(
			username, password, main_value, discount_value,
			discount_type, expiration_time, request_numbers_per_day,
			read_size, write_size);

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
