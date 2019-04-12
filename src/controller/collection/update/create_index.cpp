// header
#include "create_index.h"

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

void iotdb::controller::create_index(http::server::reply &rep,
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

			std::string username;

			// get client key of request

			std::string client_key;

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

			// get index document from request document
			bsoncxx::types::b_document index_document;

			try {

				index_document =
				request_document.view()["index_document"]
					.get_document();
			}
			catch (std::exception &e) {

				// if username doesn't exist in request document
				if (strcmp(e.what(),
					   "unset document::element") == 0) {
					std::string reply =
					core::reply::missing_item_error(
						"index_document");
					rep.content.append(reply.c_str(),
							   reply.size());
				} // check if element type is wrong
				else if (strcmp(e.what(),
						"expected element "
						"type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"index_document");
					rep.content.append(reply.c_str(),
							   reply.size());
				}
				return;
			}

			// get options of request
			boost::optional<bsoncxx::types::b_document> options;

			try {
				options = request_document.view()["options"]
					  .get_document();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type("options");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			boost::optional<bool> background;
			boost::optional<bool> unique;
			boost::optional<bool> sparse;
			boost::optional<std::int32_t> version;
			boost::optional<std::uint8_t> twod_sphere_version;
			boost::optional<std::uint8_t> twod_bits_precision;
			boost::optional<double> twod_location_max;
			boost::optional<double> twod_location_min;
			boost::optional<double> haystack_bucket_size;
			boost::optional<size_t> expire_after;
			boost::optional<std::string> name;
			boost::optional<std::string> default_language;
			boost::optional<std::string> language_override;
			boost::optional<bsoncxx::types::b_document> collation;
			boost::optional<bsoncxx::types::b_document> weights;
			boost::optional<bsoncxx::types::b_document>
			partial_filter_expression;

			if (options.is_initialized()) {

				// get background from option
				try {
					background = options.get()
							 .view()["background"]
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
							"background");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get unique from option
				try {
					unique = options.get()
						 .view()["unique"]
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
							"unique");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get sparse from option
				try {
					sparse = options.get()
						 .view()["sparse"]
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
							"sparse");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get version from option
				try {
					version = options.get()
						  .view()["version"]
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
							"version");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get twod_sphere_version from option
				try {
					std::int32_t twod_sphere_version_temp =
					options.get()
						.view()["twod_sphere_version"]
						.get_int32();

					if (twod_sphere_version_temp >= 0) {
						twod_sphere_version =
						twod_sphere_version_temp;
					}
					else {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_sphere_version");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_sphere_version");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get twod_bits_precision from option
				try {
					std::int32_t twod_bits_precision_temp =
					options.get()
						.view()["twod_bits_precision"]
						.get_int32();

					if (twod_bits_precision_temp >= 0) {
						twod_bits_precision =
						twod_bits_precision_temp;
					}
					else {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_bits_precision");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_bits_precision");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get expire_after from option
				try {
					std::int32_t expire_after_temp =
					options.get()
						.view()["expire_after"]
						.get_int32();

					if (expire_after_temp >= 0) {
						expire_after = expire_after_temp;
					}
					else {
						std::string reply =
						core::reply::wrong_item_type(
							"expire_after");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"expire_after");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get twod_location_max from option
				try {
					twod_location_max =
					options.get()
						.view()["twod_location_max"]
						.get_double();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_location_max");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get twod_location_min from option
				try {
					twod_location_min =
					options.get()
						.view()["twod_location_min"]
						.get_double();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"twod_location_min");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get haystack_bucket_size from option
				try {
					haystack_bucket_size =
					options.get()
						.view()["haystack_bucket_size"]
						.get_double();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"haystack_bucket_size");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get name from option
				try {
					name = options.get()
						   .view()["name"]
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
							"name");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get default_language from option
				try {
					default_language =
					options.get()
						.view()["default_language"]
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
							"default_language");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get language_override from option
				try {
					language_override =
					options.get()
						.view()["language_override"]
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
							"language_override");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get collation from option
				try {
					collation = options.get()
							.view()["collation"]
							.get_document();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"collation");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get weights from option
				try {
					weights = options.get()
						  .view()["weights"]
						  .get_document();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"weights");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get partial_filter_expression from option
				try {
					partial_filter_expression =
					options.get()
						.view()["partial_filter_expression"]
						.get_document();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"partial_filter_"
							"expression");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
			}

			// get options of request
			boost::optional<bsoncxx::types::b_document>
			operation_options;

			try {
				operation_options =
				request_document.view()["operation_options"]
					.get_document();
			}
			catch (std::exception &e) {

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
							 "type k_document") == 0) {
					std::string reply =
					core::reply::wrong_item_type(
						"operation_options");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get max_time from operation_options
			boost::optional<size_t> max_time;

			// get acknowledge_level from write_concern doc
			boost::optional<std::string> acknowledge_level;

			// get journal from write_concern doc
			boost::optional<bool> journal;

			// get tag from write_concern doc
			boost::optional<std::string> tag;

			// get majority from write_concern doc
			boost::optional<int> majority;

			// get timeout from write_concern doc
			boost::optional<int> timeout;

			// get nodes from write_concern doc
			boost::optional<int> nodes;

			if (operation_options.is_initialized()) {
				// get max_time from option
				try {
					std::int32_t max_time_temp =
					operation_options.get()
						.view()["max_time"]
						.get_int32();

					if (max_time_temp >= 0) {
						max_time = max_time_temp;
					}
					else {
						std::string reply =
						core::reply::wrong_item_type(
							"max_time");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"max_time");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get write_concern document of operation_options
				// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
				boost::optional<bsoncxx::types::b_document>
				write_concern;

				try {
					write_concern = operation_options.get()
							.view()["write_concern"]
							.get_document();
				}
				catch (std::exception &e) {

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0) {
						std::string reply =
						core::reply::wrong_item_type(
							"write_concern");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"acknowledge_"
								"level");
							rep.content.append(
							reply.c_str(),
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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"tag");
							rep.content.append(
							reply.c_str(),
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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"journal");
							rep.content.append(
							reply.c_str(),
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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"majority");
							rep.content.append(
							reply.c_str(),
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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"timeout");
							rep.content.append(
							reply.c_str(),
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
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") ==
							0) {
							std::string reply = core::
							reply::wrong_item_type(
								"nodes");
							rep.content.append(
							reply.c_str(),
							reply.size());
							return;
						}
					}
				}
			}

			// get reply from database
			auto reply = iotdb::database::create_index(
			username, database_name, index_document,
			options.is_initialized(), background, unique, sparse,
			version, twod_sphere_version, twod_bits_precision,
			twod_location_max, twod_location_min,
			haystack_bucket_size, expire_after, name,
			default_language, language_override, collation, weights,
			partial_filter_expression, max_time, acknowledge_level,
			tag, journal, majority, timeout, nodes);

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
