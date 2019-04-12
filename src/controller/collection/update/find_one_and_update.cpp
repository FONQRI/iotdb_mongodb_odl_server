// header
#include "find_one_and_update.h"

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

void iotdb::controller::find_one_and_update(http::server::reply &rep,
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

			// get query document from request document
			bsoncxx::types::b_document query;

			try {

				query =
				request_document.view()["query"].get_document();
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
					core::reply::wrong_item_type("query");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get projection document of request
			boost::optional<bsoncxx::types::b_document> projection;
			try {
				projection = request_document.view()["projection"]
						 .get_document();
			}
			catch (std::exception &e) {
				// ckeck wrong type
				if (strcmp(
					e.what(),
					"expected element type k_document") ==
					0) {
					// document or its type is false
					std::string reply =
					core::reply::wrong_item_type(
						"projection");

					// write reply
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
				// element is optional
			}

			// get sort document of request
			boost::optional<bsoncxx::types::b_document> sort;
			try {
				sort =
				request_document.view()["sort"].get_document();
			}
			catch (std::exception &e) {
				// ckeck wrong type
				if (strcmp(
					e.what(),
					"expected element type k_document") ==
					0) {
					// document or its type is false
					std::string reply =
					core::reply::wrong_item_type("sort");

					// write reply
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
				// element is optional
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
			std::string tag;

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

			// get upsert from request document
			boost::optional<bool> upsert;

			try {
				upsert =
				request_document.view()["upsert"].get_bool();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("upsert");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get max_time from request document
			boost::optional<int> max_time;

			try {
				max_time =
				request_document.view()["max_time"].get_int32();
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
			auto reply = iotdb::database::find_one_and_update(
			username, database_name, filter, query, projection,
			sort, collation, acknowledge_level, tag, journal,
			majority, timeout, nodes, max_time, upsert);

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
