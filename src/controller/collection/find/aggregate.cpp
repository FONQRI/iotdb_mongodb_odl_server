// header
#include "aggregate.h"

// internal
#include "src/core/reply.h"
#include "src/database/collection_methods.h"
#include "src/database/security/password.h"

// mongocxx
#include <mongocxx/exception/exception.hpp>

// boost
#include <boost/optional.hpp>

// std
#include <iostream>
#include <vector>

void iotdb::controller::aggregate(http::server::reply &rep,
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

	try
	{
		// check if request is post
		if (request.method == "POST")
		{

			// get username of request
			std::string username{""};
			// get client key of request
			std::string client_key{""};
			for (auto &header : request.headers)
			{
				if (header.name == "username")
				{
					username = header.value;
				}
				else if (header.name == "client_key")
				{
					client_key = header.value;
				}
			}
			if (username.empty())
			{
				std::string reply =
				core::reply::missing_item_error("username");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}
			else if (client_key.empty())
			{
				std::string reply =
				core::reply::missing_item_error("client_key");
				rep.content.append(reply.c_str(), reply.size());
				return;
			}

			// get database name and check client_key access
			std::string database_name{};
			std::string check_key_reply;
			if (!iotdb::database::security::password::check_key(
				username, client_key, check_key_reply))
			{
				rep.content.append(check_key_reply.c_str(),
						   check_key_reply.size());
				return;
			}
			else
			{
				database_name = check_key_reply;
			}

			// convert content to json
			bsoncxx::document::value request_document =
			bsoncxx::from_json(request.content);

			// get options document of request
			optional_ducument options;

			try
			{
				options = request_document.view()["options"]
					  .get_document();
			}
			catch (std::exception &e)
			{

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
						 "type k_document") == 0)
				{
					std::string reply =
					core::reply::wrong_item_type("options");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get allow_disk_use from options document
			optional_bool allow_disk_use;

			// get use_cursor from options document
			optional_bool use_cursor;

			// get bypass_document_validation from options
			// document
			optional_bool bypass_document_validation;

			// get max_time from options document
			optional_size max_time;

			// get batch_size from options document
			optional_int batch_size;

			// get collation options of request
			optional_ducument collation;

			// get acknowledge_level from  write_concern document
			optional_string acknowledge_level;

			// get tag from  write_concern document
			optional_string tag;

			// get journal from  write_concern document
			optional_bool journal;

			// get majority from  write_concern document
			optional_int majority;

			// get timeout from  write_concern document
			optional_int timeout;

			// get nodes from  write_concern document
			optional_int nodes;

			// get hint document of options document
			optional_ducument hint;

			// get hint_str string of  options document
			optional_string hint_str;

			if (options.is_initialized())
			{

				// get allow_disk_use from options document
				try
				{
					allow_disk_use =
					options.get()
						.view()["allow_disk_use"]
						.get_bool();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"allow_disk_use");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// controll ram usage
				allow_disk_use = false;

				// get use_cursor from options document
				try
				{
					use_cursor = options.get()
							 .view()["use_cursor"]
							 .get_bool();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"use_cursor");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get bypass_document_validation from options
				// document
				try
				{
					bypass_document_validation =
					options.get()
						.view()
						["bypass_document_validation"]
						.get_bool();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"bypass_document_"
							"validation");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get max_time from options document
				try
				{
					std::int32_t max_time_temp =
					options.get()
						.view()["max_time"]
						.get_int32();

					if (max_time_temp >= 0)
					{
						max_time = max_time_temp;
					}
					else
					{
						std::string reply =
						core::reply::wrong_item_type(
							"max_time is unsigned");
						rep.content.append(reply.c_str(),
								   reply.size());
					}
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"max_time");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get batch_size from options document
				try
				{
					batch_size = options.get()
							 .view()["batch_size"]
							 .get_int32();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"batch_size");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get collation options of request
				try
				{
					collation = options.get()
							.view()["collation"]
							.get_document();
				}
				catch (std::exception &e)
				{

					// optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"collation");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get write_concern document of request
				optional_ducument write_concern;

				try
				{
					write_concern = options.get()
							.view()["write_concern"]
							.get_document();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"write_concern");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
				if (write_concern.is_initialized())
				{

					// get acknowledge_level from
					// write_concern document
					try
					{
						acknowledge_level =
						write_concern.get()
							.view()["acknowledge_level"]
							.get_utf8()
							.value.to_string();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
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

					// get tag from  write_concern document
					try
					{
						tag = write_concern.get()
							  .view()["tag"]
							  .get_utf8()
							  .value.to_string();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
							std::string reply = core::
							reply::wrong_item_type(
								"tag");
							rep.content.append(
							reply.c_str(),
							reply.size());
							return;
						}
					}

					// get journal from  write_concern
					// document
					try
					{
						journal = write_concern.get()
							  .view()["journal"]
							  .get_bool();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
							std::string reply = core::
							reply::wrong_item_type(
								"journal");
							rep.content.append(
							reply.c_str(),
							reply.size());
							return;
						}
					}

					// get majority from  write_concern
					// document
					try
					{
						majority = write_concern.get()
							   .view()["majority"]
							   .get_int32();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
							std::string reply = core::
							reply::wrong_item_type(
								"majority");
							rep.content.append(
							reply.c_str(),
							reply.size());
							return;
						}
					}

					// get timeout from  write_concern
					// document
					try
					{
						timeout = write_concern.get()
							  .view()["timeout"]
							  .get_int32();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
							std::string reply = core::
							reply::wrong_item_type(
								"timeout");
							rep.content.append(
							reply.c_str(),
							reply.size());
							return;
						}
					}

					// get nodes from  write_concern
					// document
					try
					{
						nodes = write_concern.get()
							.view()["nodes"]
							.get_int32();
					}
					catch (std::exception &e)
					{

						// element is optional
						// check if element type is
						// wrong
						if (strcmp(e.what(),
							   "expected element "
							   "type k_document") == 0)
						{
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

				// get hint document of options document
				try
				{
					hint = options.get()
						   .view()["hint"]
						   .get_document();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"hint");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}

				// get hint_str document of request

				try
				{
					hint_str = options.get()
						   .view()["hint_str"]
						   .get_utf8()
						   .value.to_string();
				}
				catch (std::exception &e)
				{

					// element is optional
					// check if element type is wrong
					if (strcmp(e.what(),
						   "expected element "
						   "type k_document") == 0)
					{
						std::string reply =
						core::reply::wrong_item_type(
							"hint_str");
						rep.content.append(reply.c_str(),
								   reply.size());
						return;
					}
				}
			}

			// get pipeline document of request
			optional_array pipeline;

			try
			{
				pipeline =
				request_document.view()["pipeline"].get_array();
			}
			catch (std::exception &e)
			{

				// element is optional
				// check if element type is wrong
				if (strcmp(e.what(), "expected element "
						 "type k_document") == 0)
				{
					std::string reply =
					core::reply::wrong_item_type(
						"pipeline");
					rep.content.append(reply.c_str(),
							   reply.size());
					return;
				}
			}

			// get reply from database
			auto reply = iotdb::database::aggregate(
			username, database_name, allow_disk_use, use_cursor,
			bypass_document_validation, max_time, batch_size,
			collation, acknowledge_level, tag, journal, majority,
			timeout, nodes, hint, hint_str, pipeline);

			// write reply
			rep.content.append(reply.c_str(), reply.size());
		}
		else
		{
			// if request isn't post method
			std::string reply =
			core::reply::http_error("send post method");

			// write reply
			rep.content.append(reply.c_str(), reply.size());
		}
	}
	catch (std::exception &e)
	{

		// if execption happend in getting values or parsing json
		std::string reply =
		core::reply::wrong_request_content_type(e.what());

		// write reply
		rep.content.append(reply.c_str(), reply.size());
	}
}
