// header
#include "update_user.h"

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

void iotdb::controller::update_user(http::server::reply &rep,
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

			// update_username
			optional_string update_username;
			try {
				update_username =
				request_document.view()["update_username"]
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
						"update_username");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// update_password
			optional_string update_password;
			try {
				update_password =
				request_document.view()["update_password"]
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
						"update_password");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// primary_phone_number
			optional_string primary_phone_number;
			try {
				primary_phone_number =
				request_document.view()["primary_phone_number"]
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
						"primary_phone_number");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// primary_email
			optional_string primary_email;
			try {
				primary_email =
				request_document.view()["primary_email"]
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
						"primary_email");
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

			// get phone_numbers array of request
			bsoncxx::array::view array;
			optional_string_array phone_numbers;
			try {
				array = request_document.view()["phone_numbers"]
					.get_array()
					.value;
				phone_numbers = std::vector<std::string>();

				// document vector
				for (auto &doc : array) {
					phone_numbers.get().push_back(
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

			// get reply from database
			auto reply = iotdb::database::update_user(
			username, password, update_username, update_password,
			primary_phone_number, primary_email, phone_numbers,
			emails_array);

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
