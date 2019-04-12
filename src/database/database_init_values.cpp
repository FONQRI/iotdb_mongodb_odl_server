#include "database_init_values.h"

// TODO remove test code
#include "security/password.h"

// mongo includes
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// TODO add indexes
bool iotdb::database::initialize()
{

	//	iotdb::database::database_urls_and_ports = database_urls_and_ports;

	create_indexes();
	return true;
}

bool iotdb::database::create_indexes()
{
	auto index_document = bsoncxx::builder::basic::document{};
	auto index_document_phone_number = bsoncxx::builder::basic::document{};
	auto index_document_email = bsoncxx::builder::basic::document{};
	auto index_document_key = bsoncxx::builder::basic::document{};
	index_document.append(kvp("username", 1));
	index_document_phone_number.append(kvp("primary_phone_number", 1));
	index_document_email.append(kvp("primary_email", 1));
	index_document_key.append(kvp("keys.name", 1));

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection["server_admin"];

	// create collection
	mongocxx::collection collection = database["users"];

	// option
	mongocxx::options::index options{};
	options.unique(true);

	// create indexes
	collection.create_index(index_document.view(), options);
	collection.create_index(index_document_phone_number.view(), options);
	collection.create_index(index_document_email.view(), options);
	collection.create_index(index_document_key.view(), options);

	return true;
}
