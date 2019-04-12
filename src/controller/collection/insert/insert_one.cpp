// header
#include "insert_one.h"

// internal
#include "src/core/reply.h"
#include "src/core/tools.h"
#include "src/database/admin_methods.h"
#include "src/database/collection_methods.h"
#include "src/database/security/password.h"

// boost
#include <boost/optional.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// std
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

void iotdb::controller::insert_one(http::server::reply &rep,
				   http::server::request request)
{
	// add headers
	//	specifying content type as json
	http::server::header content_type;
	content_type.name = "Content-Type";
	content_type.value = "application/json;";

	//	std::clog << "request.content" << std::endl;

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

			// get add_date_time
			bool add_date_time{false};

			for (auto &header : request.headers) {
				if (header.name == "username") {
					username = header.value;
				}
				else if (header.name == "client_key") {
					client_key = header.value;
				}
				else if (header.name == "add_date_time") {

					if (header.value == "true") {
						add_date_time = true;
					}
					else if (header.value == "false") {
						add_date_time = false;
					}
					else {
						std::string reply =
						core::reply::wrong_item_type(
							"add_date_time");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
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
			//			std::string m_reply;
			//			bool rs =
			// iotdb::database::admin::m_check_key(
			//			username, client_key, m_reply,
			//__FUNCTION__);

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

			// NOTE test
			//			std::string database_name{};
			//			std::string check_key_reply;
			//			if
			//(!iotdb::database::admin::m_check_key(
			//				username, client_key,
			// check_key_reply,
			//				__FUNCTION__)) {
			//				rep.content.append(check_key_reply.c_str(),
			//						   check_key_reply.size());
			//				return;
			//			}
			//			else {
			//				database_name = check_key_reply;
			//			}

			// add date and time
			std::string edited_content;
			if (add_date_time) {
				edited_content =
				iotdb::core::tools::add_date_time_to_query(
					request.content);

				if (edited_content == "missing query") {
					std::string reply =
					core::reply::missing_item_error(
						"query");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
				else if (edited_content == "wrong query") {
					std::string reply =
					core::reply::wrong_item_type("query");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}
			else {
				edited_content = request.content;
			}

			// convert content to json
			bsoncxx::document::value request_document =
			bsoncxx::from_json(edited_content);

			// get query document of request
			bsoncxx::types::b_document query;

			try {

				query =
				request_document.view()["query"].get_document();
			}
			catch (std::exception &e) {

				// if element doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"query");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("query");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get write_concern document of request
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			boost::optional<bsoncxx::types::b_document> write_concern;

			try {
				write_concern =
				request_document.view()["write_concern"]
					.get_document();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"write_concern");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get acknowledge_level from write_concern doc
			boost::optional<std::string> acknowledge_level;

			// get tag from write_concern doc
			boost::optional<std::string> tag;

			// get journal from write_concern doc
			boost::optional<bool> journal;

			// get majority from write_concern doc
			boost::optional<int> majority;

			// get timeout from write_concern doc
			boost::optional<int> timeout;

			// get nodes from write_concern doc
			boost::optional<int> nodes;

			if (write_concern.is_initialized()) {
				try {
					acknowledge_level =
					write_concern.get()
						.view()["acknowledge_level"]
						.get_utf8()
						.value.to_string();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"acknowledge_level");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get tag from write_concern doc
				try {
					tag = write_concern.get()
						  .view()["tag"]
						  .get_utf8()
						  .value.to_string();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"tag");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get journal from write_concern doc
				try {
					journal = write_concern.get()
						  .view()["journal"]
						  .get_bool();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"journal");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get majority from write_concern doc
				try {
					majority = write_concern.get()
						   .view()["majority"]
						   .get_int32();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"majority");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get timeout from write_concern doc
				try {
					timeout = write_concern.get()
						  .view()["timeout"]
						  .get_int32();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"timeout");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get nodes from write_concern doc
				try {
					nodes = write_concern.get()
						.view()["nodes"]
						.get_int32();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"nodes");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
			}
			// get ordered bool from request document
			boost::optional<bool> ordered;

			try {
				ordered =
				request_document.view()["ordered"].get_bool();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("ordered");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get ordered bool from request document
			boost::optional<bool> bypass_document_validation;

			try {
				bypass_document_validation =
				request_document
					.view()["bypass_document_validation"]
					.get_bool();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"bypass_document_validation");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get reply from database
			auto reply = iotdb::database::insert_one(
			username, database_name, query, acknowledge_level, tag,
			journal, majority, timeout, nodes, ordered,
			bypass_document_validation);

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
		//		if (first) {
		//			std::clog << "reply" << std::endl;
		//			first = false;
		//		}
		// if execption happend in getting values or parsing json
		std::string reply =
		core::reply::wrong_request_content_type(e.what());
		// write reply
		rep.content.append(reply.c_str(), reply.size());
	}
}
