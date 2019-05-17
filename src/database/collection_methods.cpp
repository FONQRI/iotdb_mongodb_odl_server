#include "collection_methods.h"
#include "core/reply.h"
#include "security/password.h"

#include <chrono>
#include <iostream>

// mongocxx
#include <mongocxx/exception/exception.hpp>

#include <iterator>

// TODO [3] use costume uri (real server and backups)

/**
 * @brief insert_many	: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.insertMany/">
 * insert_many mongodb</a> insert a vector of documents
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param insert_document_array	: a vector of documents
 * @param ordered				: bool of ordered
 * @return answer reply
 */
std::string iotdb::database::insert_many(
	std::string username, std::string database_name,
	std::vector<bsoncxx::document::value> insert_document_array,
	boost::optional<std::string> acknowledge_level, boost::optional<std::string> tag,
	boost::optional<bool> journal, boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<bool> ordered,
	boost::optional<bool> bypass_document_validation)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {
		mongocxx::options::insert options;

		if (ordered.is_initialized()) {
			options.ordered(ordered.get());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// add bypass_document_validation to options
		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}
		collection.insert_many(insert_document_array, options);
		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief insert_one			: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.insertOne/">
 * insert_one mongodb</a> document
 * @param username			: username of database
 * @param database_name		: database name of user
 * @param insert_document	: BSON document to insert
 * @return answer reply
 */
std::string iotdb::database::insert_one(std::string username, std::string database_name,
										bsoncxx::types::b_document insert_document,
										boost::optional<std::string> acknowledge_level,
										boost::optional<std::string> tag,
										boost::optional<bool> journal,
										boost::optional<int> majority, boost::optional<int> timeout,
										boost::optional<int> nodes, boost::optional<bool> ordered,
										boost::optional<bool> bypass_document_validation)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {
		mongocxx::options::insert options;

		if (ordered.is_initialized()) {
			options.ordered(ordered.get());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// add bypass_document_validation to options
		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}

		collection.insert_one(insert_document.view(), options);
		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief find					: <a href =
 * "https://docs.mongodb.com/manual/reference/method/db.collection.find/"> find
 * mongodb</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param projection_document	: BSON <a
 * href="https://docs.mongodb.com/manual/reference/glossary/#term-projection">
 * projection </a> document
 * @param sort_document			: BSON document to sort e.g.
 * {"value_name"= 1} or {"value_name"= -1}
 * @param min_document			: BSON document that specify maximum of
 * every value
 * @param max_document			: BSON document that specify minimum of
 * every value
 * @param limit_number_of_docs	: limitation number of returned document
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":" + answer json array of documents +"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::find(std::string username, std::string database_name,
								  bsoncxx::types::b_document query_document,
								  boost::optional<bsoncxx::types::b_document> projection_document,
								  boost::optional<bsoncxx::types::b_document> sort_document,
								  boost::optional<bsoncxx::types::b_document> min_document,
								  boost::optional<bsoncxx::types::b_document> max_document,
								  boost::optional<size_t> limit_number_of_docs)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::find options = mongocxx::options::find{};
		if (projection_document.is_initialized()) {
			options.projection(projection_document.get().view());
		}

		if (sort_document.is_initialized()) {
			options.sort(sort_document.get().view());
		}

		if (min_document.is_initialized()) {
			options.min(min_document.get().view());
		}

		if (max_document.is_initialized()) {
			options.max(max_document.get().view());
		}

		// limit number of docs
		if (limit_number_of_docs.is_initialized()) {
			options.limit(limit_number_of_docs.get());
		}

		auto cursor = collection.find({query_document}, options);
		std::string reply{};

		reply.append("[");
		for (auto &&doc : cursor) {
			reply.append(bsoncxx::to_json(doc) + ",");
		}

		// remove final "," character if ther is any element
		if (reply.size() > 2) {
			reply.erase(reply.size() - 1, 1);
		}

		reply.append("]");

		// make reply json
		return core::reply::answer(reply);
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief find_one				:<a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.findOne/">
 * find_one mongodb a vector of documents</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param projection_document	: BSON <a
 * href="https://docs.mongodb.com/manual/reference/glossary/#term-projection">
 * projection </a> document
 * @param sort_document			: BSON document to sort e.g.
 * {"value_name"= 1} or {"value_name"= -1}
 * @param min_document			: BSON document that specify maximum of
 * every value
 * @param max_document			: BSON document that specify minimum of
 * every value
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response\":"+ answer json document + "}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::find_one(
	std::string username, std::string database_name, bsoncxx::types::b_document query_document,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> min_document,
	boost::optional<bsoncxx::types::b_document> max_document)
{

	// create connection

	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::find options = mongocxx::options::find{};

		if (projection_document.is_initialized()) {
			options.projection(projection_document.get().view());
		}

		if (sort_document.is_initialized()) {
			options.sort(sort_document.get().view());
		}

		if (min_document.is_initialized()) {
			options.min(min_document.get().view());
		}

		if (max_document.is_initialized()) {
			options.max(max_document.get().view());
		}

		// create cursor bu qyery and options
		auto cursor = collection.find_one({query_document}, options);

		// create json string from response
		std::string reply = bsoncxx::to_json(cursor.value());

		// make reply json
		return core::reply::answer(reply);
	} catch (const mongocxx::exception &e) {
		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief update_many					:<a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.updateMany/">update_many
 * mongodb</a>
 * @param username						: username of
 * database
 * @param database_name					: database name of user
 * @param filter_document				: filter document
 * @param update_document				: BSON query document
 * @param collation						: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param upsert						: Sets the
 * upsert
 * option.
 * @param bypass_document_validation	: Sets the bypass_document_validation
 * option.
 * If true, allows the write to opt-out of document level validation.
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */

std::string iotdb::database::update_many(
	std::string username, std::string database_name, bsoncxx::types::b_document filter_document,
	bsoncxx::types::b_document update_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level, boost::optional<std::string> tag,
	boost::optional<bool> journal, boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<bool> upsert,
	boost::optional<bool> bypass_document_validation)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::update options = mongocxx::options::update{};

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (upsert.is_initialized()) {
			options.upsert(upsert.get());
		}

		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}

				// add created write_concern to options
				options.write_concern(write_concern);
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// create cursor bu qyery and options
		collection.update_many({filter_document}, {update_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief update_many					:<a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.updateMany/">update_many
 * mongodb</a>
 * @param username						: username of
 * database
 * @param database_name					: database name of user
 * @param filter_document				: filter document
 * @param update_document				: BSON query document
 * @param collation						: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param upsert						: Sets the
 * upsert
 * option.
 * @param bypass_document_validation	: Sets the bypass_document_validation
 * option.
 * If true, allows the write to opt-out of document level validation.
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::update_one(std::string username, std::string database_name,
										bsoncxx::types::b_document filter_document,
										bsoncxx::types::b_document update_document,
										boost::optional<bsoncxx::types::b_document> collation,
										boost::optional<std::string> acknowledge_level,
										boost::optional<std::string> tag,
										boost::optional<bool> journal,
										boost::optional<int> majority, boost::optional<int> timeout,
										boost::optional<int> nodes, boost::optional<bool> upsert,
										boost::optional<bool> bypass_document_validation)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::update options = mongocxx::options::update{};

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (upsert.is_initialized()) {
			options.upsert(upsert.get());
		}

		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {

				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// create cursor bu qyery and options
		collection.update_one({filter_document}, {update_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief find_one_and_update	: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.findOneAndUpdate/">
 * find_one_and_update mongodb</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param filter_document		: filter document
 * @param update_document		: BSON query document
 * @param projection_document	: BSON <a
 * href="https://docs.mongodb.com/manual/reference/glossary/#term-projection">
 * projection </a> document
 * @param sort_document			: BSON document to sort e.g.
 * {"value_name"= 1} or {"value_name"= -1}
 * @param collation						: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout						: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes							: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param max_time						: The max amount
 * of
 * running
 * time
 * @param upsert						: Sets the
 * upsert
 * option.
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::find_one_and_update(
	std::string username, std::string database_name, bsoncxx::types::b_document filter_document,
	bsoncxx::types::b_document update_document,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level, boost::optional<std::string> tag,
	boost::optional<bool> journal, boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time, boost::optional<bool> upsert)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::find_one_and_update options = mongocxx::options::find_one_and_update{};
		if (projection_document.is_initialized()) {
			options.projection(projection_document.get().view());
		}

		if (sort_document.is_initialized()) {
			options.sort(sort_document.get().view());
		}

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (upsert.is_initialized()) {
			options.upsert(upsert.get());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// add max_time to options
		if (max_time.is_initialized()) {
			options.max_time(std::chrono::milliseconds(max_time.get()));
		}

		// create cursor bu qyery and options
		collection.find_one_and_update({update_document}, {filter_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief find_one_and_replace	: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.findOneAndReplace/">
 * find_one_and_replace mongodb</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param filter_document		: filter document
 * @param replacement			: replacement document
 * @param projection_document	: BSON <a
 * href="https://docs.mongodb.com/manual/reference/glossary/#term-projection">
 * projection </a> document
 * @param sort_document			: BSON document to sort e.g.
 * {"value_name"= 1} or {"value_name"= -1}
 * @param collation				: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level		: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param max_time				: The max amount of running time
 * @param upsert				: Sets the upsert option.
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::find_one_and_replace(
	std::string username, std::string database_name, bsoncxx::types::b_document filter_document,
	bsoncxx::types::b_document replacement,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level, boost::optional<std::string> tag,
	boost::optional<bool> journal, boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time, boost::optional<bool> upsert,
	boost::optional<bool> bypass_document_validation)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::find_one_and_replace options = mongocxx::options::find_one_and_replace{};

		if (projection_document.is_initialized()) {
			options.projection(projection_document.get().view());
		}

		if (sort_document.is_initialized()) {
			options.sort(sort_document.get().view());
		}

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (upsert.is_initialized()) {
			options.upsert(upsert.get());
		}

		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}
		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// add max_time to options
		options.max_time(std::chrono::milliseconds(max_time.get()));

		// create cursor bu qyery and options
		collection.find_one_and_replace({filter_document}, {replacement}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief delete_many		: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.deleteMany/">delete_many
 * mongodb</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param collation				: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level		: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */

std::string iotdb::database::delete_many(std::string username, std::string database_name,
										 bsoncxx::types::b_document query_document,
										 boost::optional<bsoncxx::types::b_document> collation,
										 boost::optional<std::string> acknowledge_level,
										 boost::optional<std::string> tag,
										 boost::optional<bool> journal,
										 boost::optional<int> majority,
										 boost::optional<int> timeout, boost::optional<int> nodes)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	mongocxx::database database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::delete_options options = mongocxx::options::delete_options{};
		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// create cursor bu qyery and options
		collection.delete_many({query_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief delete_one		: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.deleteOne/">delete_one
 * mongodb</a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param collation				: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level		: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */

std::string iotdb::database::delete_one(std::string username, std::string database_name,
										bsoncxx::types::b_document query_document,
										boost::optional<bsoncxx::types::b_document> collation,
										boost::optional<std::string> acknowledge_level,
										boost::optional<std::string> tag,
										boost::optional<bool> journal,
										boost::optional<int> majority, boost::optional<int> timeout,
										boost::optional<int> nodes)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::delete_options options = mongocxx::options::delete_options{};

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}
			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// create cursor bu qyery and options
		collection.delete_one({query_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief find_one_and_delete	: <a href =
 * "https://docs.mongodb.com/manual/reference/method/db.collection.findOneAndDelete/">
 * find_one_and_delete mongodb </a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param projection_document	: BSON <a
 * href="https://docs.mongodb.com/manual/reference/glossary/#term-projection">
 * projection </a> document
 * @param sort_document			: BSON document to sort e.g.
 * {"value_name"= 1} or {"value_name"= -1}
 * @param collation				: <a
 * href="https://docs.mongodb.com/manual/reference/collation/index.html" >
 * collation mongodb </a>
 * @param acknowledge_level		: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param tag					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param journal				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param majority				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param timeout				: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param nodes					: a value for <a
 * href="https://docs.mongodb.com/manual/reference/write-concern/index.html">
 * write-concern mongodb</a>
 * @param max_time				: The max amount of running time
 * (in
 * milliseconds).
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":"success"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::find_one_and_delete(
	std::string username, std::string database_name, bsoncxx::types::b_document query_document,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level, boost::optional<std::string> tag,
	boost::optional<bool> journal, boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::find_one_and_delete options = mongocxx::options::find_one_and_delete{};

		if (projection_document.is_initialized()) {
			options.projection(projection_document.get().view());
		}

		if (sort_document.is_initialized()) {

			options.sort(sort_document.get().view());
		}

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {
				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		// add max_time to options
		if (max_time.is_initialized()) {
			options.max_time(std::chrono::milliseconds(max_time.get()));
		}

		// create cursor bu qyery and options
		collection.find_one_and_delete({query_document}, options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief count					: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.count/">
 * count mongodb </a>
 * @param username				: username of database
 * @param database_name			: database name of user
 * @param query_document		: BSON query document
 * @param limit_number_of_docs	: limitation number of returned document
 * @param skip_number_of_docs	: how many document to skip before start
 * @return reply document as json at this format
 * : "{"isSuccessful":true,"Response":" + number +"}"
 * or this one for errors
 * :"{"isSuccessful":false,"Message":" + what happened + "}"
 */
std::string iotdb::database::count(std::string username, std::string database_name,
								   bsoncxx::types::b_document query_document,
								   boost::optional<size_t> limit_number_of_docs,
								   boost::optional<size_t> skip_number_of_docs)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// create option
		mongocxx::options::count options = mongocxx::options::count{};

		// limit number of docs
		if (limit_number_of_docs.is_initialized()) {
			options.limit(limit_number_of_docs.get());
		}

		// skip number of docs
		if (skip_number_of_docs.is_initialized()) {
			options.skip(skip_number_of_docs.get());
		}

		auto cursor = collection.count({query_document}, options);
		std::string reply{};

		// make reply json
		return core::reply::answer(std::to_string(cursor));
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief drop			: drop a collection <a href=
 * "https://docs.mongodb.com/manual/reference/method/db.collection.drop/#db.collection.drop">
 * drop mongodb </a>
 * @param username		: username
 * @param database_name	: collection name
 * @return				: success or failure message
 */

std::string iotdb::database::drop(std::string username, std::string database_name)
{

	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	auto collection = database[database_name];

	try {

		// drop collection
		collection.drop();

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief create_index				: <a
 * href="https://docs.mongodb.com/manual/reference/method/db.collection.createIndex/">
 * create_index mongodb </a>
 * @param username				   	: username of database
 * @param database_name			   	: database name of user
 * @param index_document		   	: index_document
 * @param options_is_set		   	: true if options document is
 * set
 * from
 * user side in request
 * @param background			   	: background of options
 * @param unique				   	: unique of options
 * @param sparse				   	: sparse of options
 * @param version				   	: version of options
 * @param twod_sphere_version	   	: twod_sphere_version of options
 * @param twod_bits_precision	   	: twod_bits_precision of options
 * @param twod_location_max		   	: twod_location_max of options
 * @param twod_location_min		   	: twod_location_min of options
 * @param haystack_bucket_size	   	: haystack_bucket_size of options
 * @param expire_after			   	: expire_after of options
 * @param name					   	: name of options
 * @param default_language		   	: default_language of options
 * @param language_override		   	: language_override of options
 * @param collation				   	: collation of options
 * @param weights				   	: weights of options
 * @param partial_filter_expression	: partial_filter_expression of options
 * @param max_time				   	: max_time of
 * operation_options
 * @param acknowledge_level		   	: acknowledge_level of
 * write_concern
 * @param tag					   	: tag of write_concern
 * @param journal				   	: journal of
 * write_concern
 * @param majority				   	: majority of
 * write_concern
 * @param timeout				   	: timeout of
 * write_concern
 * @param nodes					   	: nodes of write_concern
 * @return							: success or
 * failure
 * message
 */

std::string iotdb::database::create_index(
	std::string username, std::string database_name, bsoncxx::types::b_document index_document,
	bool options_is_set, boost::optional<bool> background, boost::optional<bool> unique,
	boost::optional<bool> sparse, boost::optional<std::int32_t> version,
	boost::optional<std::uint8_t> twod_sphere_version,
	boost::optional<std::uint8_t> twod_bits_precision, boost::optional<double> twod_location_max,
	boost::optional<double> twod_location_min, boost::optional<double> haystack_bucket_size,
	boost::optional<size_t> expire_after, boost::optional<std::string> name,
	boost::optional<std::string> default_language, boost::optional<std::string> language_override,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<bsoncxx::types::b_document> weights,
	boost::optional<bsoncxx::types::b_document> partial_filter_expression,
	boost::optional<size_t> max_time, boost::optional<std::string> acknowledge_level,
	boost::optional<std::string> tag, boost::optional<bool> journal, boost::optional<int> majority,
	boost::optional<int> timeout, boost::optional<int> nodes)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	mongocxx::collection collection = database[database_name];

	try {

		mongocxx::options::index options;

		if (options_is_set) {

			if (background.is_initialized()) {
				options.background(background.get());
			}

			if (unique.is_initialized()) {
				options.unique(unique.get());
			}

			if (sparse.is_initialized()) {
				options.sparse(sparse.get());
			}

			if (version.is_initialized()) {
				options.version(version.get());
			}

			if (twod_sphere_version.is_initialized()) {
				options.twod_sphere_version(twod_sphere_version.get());
			}

			if (twod_bits_precision.is_initialized()) {
				options.twod_bits_precision(twod_bits_precision.get());
			}

			if (twod_location_max.is_initialized()) {
				options.twod_location_max(twod_location_max.get());
			}

			if (twod_location_min.is_initialized()) {
				options.twod_location_min(twod_location_min.get());
			}

			if (haystack_bucket_size.is_initialized()) {
				options.haystack_bucket_size(haystack_bucket_size.get());
			}

			if (expire_after.is_initialized()) {
				options.expire_after(std::chrono::seconds(expire_after.get()));
			}

			if (name.is_initialized()) {
				bsoncxx::string::view_or_value temp_name{name.get()};
				options.name(temp_name);
			}

			if (default_language.is_initialized()) {

				bsoncxx::string::view_or_value temp_default_language{default_language.get()};
				options.default_language(temp_default_language);
			}

			if (language_override.is_initialized()) {
				bsoncxx::string::view_or_value temp_language_override{language_override.get()};
				options.language_override(temp_language_override);
			}

			if (collation.is_initialized()) {
				options.collation(collation.get());
			}

			if (weights.is_initialized()) {
				options.weights(weights.get());
			}

			if (partial_filter_expression.is_initialized()) {
				options.partial_filter_expression(partial_filter_expression.get());
			}
		}

		mongocxx::options::index_view operation_options = mongocxx::options::index_view{};

		if (max_time.is_initialized()) {
			operation_options.max_time(std::chrono::milliseconds(max_time.get()));
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {

				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			operation_options.write_concern(write_concern);
		}

		// create index
		collection.create_index(index_document.view(), options, operation_options);

		return core::reply::answer_done();
	} catch (const mongocxx::exception &e) {

		// create json from error
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief name			: return name of collection
 * @param username		: username of database
 * @param database_name	: database name of user
 * @return				: name of collection in json
 */

std::string iotdb::database::name(std::string username, std::string database_name)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	mongocxx::collection collection = database[database_name];

	try {

		// create option
		mongocxx::options::count options = mongocxx::options::count{};

		mongocxx::stdx::string_view cursor = collection.name();

		// make reply json
		return core::reply::answer(cursor.to_string());
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

/**
 * @brief list_indexes	: return an array of indexes of specific collection
 * @param username		: username of database
 * @param database_name	: database name of user
 * @return 				: an array of indexes of collection
 */

std::string iotdb::database::list_indexes(std::string username, std::string database_name)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// create xollection
	auto collection = connection[username][database_name];

	try {
		auto cursor = collection.list_indexes();
		std::string reply{};

		reply.append("[");
		for (auto &&doc : cursor) {
			reply.append(bsoncxx::to_json(doc) + ",");
		}

		// remove final "," character if ther is any element
		if (reply.size() > 2) {
			reply.erase(reply.size() - 1, 1);
		}

		reply.append("]");

		// make reply json
		return core::reply::answer(reply);
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

std::string iotdb::database::distinct(std::string username, std::string database_name,
									  std::string name_string,
									  bsoncxx::types::b_document filter_document,
									  boost::optional<bsoncxx::types::b_document> collation,
									  boost::optional<size_t> max_time)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};

	// database
	auto database = connection[username];

	// create collection
	mongocxx::collection collection = database[database_name];

	try {

		// create option
		mongocxx::options::distinct options = mongocxx::options::distinct();
		if (collation.is_initialized()) {

			options.collation(collation.get().view());
		}

		if (max_time.is_initialized()) {

			options.max_time(std::chrono::milliseconds(max_time.get()));
		}

		bsoncxx::string::view_or_value name_view{name_string};

		auto cursor = collection.distinct(name_view, {filter_document}, options);

		// make reply json
		return core::reply::answer("cursor.to_string()");
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}

void iotdb::database::aggregate_oprate(mongocxx::pipeline &pipeline, bsoncxx::types::b_document doc)
{

	bool successful{false};
	try {
		auto op_doc = doc.view()["match"].get_document();
		pipeline.match(op_doc.view());
		successful = true;
	} catch (...) {
		successful = false;
	}

	if (!successful) {
		try {
			auto op_doc = doc.view()["project"].get_document();
			pipeline.project(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			int limit = doc.view()["limit"].get_int32();
			pipeline.limit(limit);
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			int sample = doc.view()["sample"].get_int32();
			pipeline.sample(sample);
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			int skip = doc.view()["skip"].get_int32();
			pipeline.skip(skip);
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			std::string out = doc.view()["out"].get_utf8().value.to_string();
			pipeline.out(out);
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			std::string count = doc.view()["count"].get_utf8().value.to_string();
			pipeline.count(count);
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["add_fields"].get_document();
			pipeline.add_fields(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["bucket"].get_document();
			pipeline.bucket(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["bucket_auto"].get_document();
			pipeline.bucket_auto(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["coll_stats"].get_document();
			pipeline.coll_stats(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["facet"].get_document();
			pipeline.facet(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["geo_near"].get_document();
			pipeline.geo_near(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["graph_lookup"].get_document();
			pipeline.graph_lookup(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["group"].get_document();
			pipeline.group(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["lookup"].get_document();
			pipeline.lookup(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			auto op_doc = doc.view()["redact"].get_document();
			pipeline.redact(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			auto op_doc = doc.view()["replace_root"].get_document();
			pipeline.replace_root(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}
	if (!successful) {
		try {
			auto op_doc = doc.view()["sort"].get_document();
			pipeline.sort(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			auto op_doc = doc.view()["sort_by_count"].get_document();
			pipeline.sort_by_count(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			auto op_doc = doc.view()["unwind"].get_document();
			pipeline.unwind(op_doc.view());
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		try {
			bool op_doc = doc.view()["index_stats"].get_bool();
			if (op_doc) {
				pipeline.index_stats();
			}
			successful = true;
		} catch (...) {
			successful = false;
		}
	}

	if (!successful) {
		throw std::invalid_argument("invalid pipeline opration");
	}
}

std::string iotdb::database::aggregate(
	std::string username, std::string database_name, optional_bool allow_disk_use,
	optional_bool use_cursor, optional_bool bypass_document_validation, optional_size max_time,
	optional_int batch_size, optional_ducument collation, optional_string acknowledge_level,
	optional_string tag, optional_bool journal, optional_int majority, optional_int timeout,
	optional_int nodes, optional_ducument hint, optional_string hint_str,
	optional_array pipeline_array)
{
	// create connection
	mongocxx::client connection{mongocxx::uri{}};
	// database
	auto database = connection[username];

	// create collection
	mongocxx::collection collection = database[database_name];

	try {
		mongocxx::options::aggregate options = mongocxx::options::aggregate();

		if (allow_disk_use.is_initialized()) {
			options.allow_disk_use(allow_disk_use.get());
		}

		if (use_cursor.is_initialized()) {
			options.use_cursor(use_cursor.get());
		}

		if (bypass_document_validation.is_initialized()) {
			options.bypass_document_validation(bypass_document_validation.get());
		}

		if (max_time.is_initialized()) {
			options.max_time(std::chrono::milliseconds(max_time.get()));
		}

		if (batch_size.is_initialized()) {
			options.batch_size(batch_size.get());
		}

		if (collation.is_initialized()) {
			options.collation(collation.get().view());
		}

		if (journal.is_initialized() || majority.is_initialized() || timeout.is_initialized()
			|| nodes.is_initialized() || tag.is_initialized()
			|| acknowledge_level.is_initialized()) {

			// create write_concern
			// https://docs.mongodb.com/manual/reference/glossary/#term-write-concern
			mongocxx::write_concern write_concern = mongocxx::write_concern();

			if (journal.is_initialized()) {
				write_concern.journal(journal.get());
			}
			if (majority.is_initialized()) {

				write_concern.majority(std::chrono::milliseconds(majority.get()));
			}

			if (timeout.is_initialized()) {
				write_concern.timeout(std::chrono::milliseconds(timeout.get()));
			}

			if (nodes.is_initialized()) {
				write_concern.nodes(nodes.get());
			}

			if (tag.is_initialized()) {

				write_concern.tag(mongocxx::stdx::string_view(tag.get()));
			}

			if (acknowledge_level.is_initialized()) {
				if (acknowledge_level.get() == "k_acknowledged") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_default") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_acknowledged);
				} else if (acknowledge_level.get() == "k_majority") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_majority);
				} else if (acknowledge_level.get() == "k_tag") {
					write_concern.acknowledge_level(mongocxx::write_concern::level::k_tag);
				} else if (acknowledge_level.get() == "k_unacknowledged") {
					write_concern.acknowledge_level(
						mongocxx::write_concern::level::k_unacknowledged);
				}
			}

			// add created write_concern to options
			options.write_concern(write_concern);
		}

		if (hint.is_initialized()) {
			options.hint(mongocxx::hint(hint.get().view()));
		} else if (hint_str.is_initialized()) {
			options.hint(mongocxx::hint(hint_str.get()));
		}

		// TODO implement this ***************************
		mongocxx::read_preference read_ref;
		read_ref = mongocxx::read_preference();
		options.read_preference(read_ref);
		//************************************************

		mongocxx::pipeline pipeline = mongocxx::pipeline();

		if (pipeline_array.is_initialized()) {
			for (const auto &element : pipeline_array.get().value) {
				bsoncxx::types::b_document doc = element.get_value().get_document();
				aggregate_oprate(pipeline, doc);
			}
		}
		auto cursor = collection.aggregate(pipeline, options);

		std::string reply{};
		//		reply.append("{");
		for (auto &&doc : cursor) {
			reply.append(bsoncxx::to_json(doc) + ",");
		}

		// remove final "," character if ther is any element
		if (reply.size() > 2) {
			reply.erase(reply.size() - 1, 1);
		}

		//		reply.append("}");

		// make reply json
		return core::reply::answer(reply);
	} catch (const mongocxx::exception &e) {
		// make error json
		return core::reply::database_error(e.what());
	}
}
