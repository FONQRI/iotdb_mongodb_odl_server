// header
#include "create_user.h"

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

void iotdb::controller::create_user(http::server::reply &rep,
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

			// primary_phone_number
			std::string primary_phone_number;
			try {
				primary_phone_number =
				request_document.view()["primary_phone_number"]
					.get_utf8()
					.value.to_string();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"primary_phone_number");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"primary_phone_number");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// primary_email
			std::string primary_email;
			try {
				primary_email =
				request_document.view()["primary_email"]
					.get_utf8()
					.value.to_string();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"primary_email");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"primary_email");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get phone_numbers array of request
			bsoncxx::array::view array;
			optional_string_array phone_numbers_array;
			try {
				array = request_document.view()["phone_numbers"]
					.get_array()
					.value;
				phone_numbers_array = std::vector<std::string>();
				// document vector
				for (auto &doc : array) {
					phone_numbers_array.get().push_back(
					doc.get_utf8().value.to_string());
				}
			}
			catch (std::exception &e) {

				// optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"phone_numbers");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get emails array of request
			bsoncxx::array::view emails;
			optional_string_array emails_array;
			try {
				emails = request_document.view()["emails"]
					 .get_array()
					 .value;
				emails_array = std::vector<std::string>();

				// document vector
				for (auto &doc : emails) {
					emails_array.get().push_back(
					doc.get_utf8().value.to_string());
				}
			}
			catch (std::exception &e) {

				// optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("emails");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// check if primary_phone_number is in array
			if (std::find(phone_numbers_array.get().begin(),
					  phone_numbers_array.get().end(),
					  primary_phone_number) ==
				phone_numbers_array.get().end()) {
				phone_numbers_array.get().push_back(
				primary_phone_number);
			}

			// check if primary_email is in array
			if (std::find(emails_array.get().begin(),
					  emails_array.get().end(),
					  primary_email) ==
				emails_array.get().end()) {
				emails_array.get().push_back(primary_email);
			}

			// get reply from database
			auto reply = iotdb::database::create_user(
			username, password, primary_phone_number, primary_email,
			phone_numbers_array, emails_array);

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
