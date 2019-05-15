#ifndef DATABASE_COLLECTIONS_METHODS_H
#define DATABASE_COLLECTIONS_METHODS_H

// mongo includes
#include <bsoncxx/array/element.hpp>
#include <bsoncxx/array/value.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/array/view_or_value.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/model/insert_one.hpp>
#include <mongocxx/options/count.hpp>
#include <mongocxx/result/insert_one.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

// boost
#include <boost/optional.hpp>

typedef boost::optional<bool> optional_bool;
typedef boost::optional<bsoncxx::types::b_document> optional_ducument;
typedef boost::optional<std::string> optional_string;
typedef boost::optional<int> optional_int;
typedef boost::optional<size_t> optional_size;
typedef boost::optional<bsoncxx::types::b_array> optional_array;

namespace iotdb {
namespace database {

// TODO [0] add writeConcern and update comment
std::string
insert_many(std::string username, std::string database_name,
		std::vector<bsoncxx::document::value> insert_document_array,
		boost::optional<std::string> acknowledge_level,
		boost::optional<std::string> tag, boost::optional<bool> journal,
		boost::optional<int> majority, boost::optional<int> timeout,
		boost::optional<int> nodes, boost::optional<bool> ordered,
		boost::optional<bool> bypass_document_validation);

std::string insert_one(std::string username, std::string database_name,
			   bsoncxx::types::b_document insert_document,
			   boost::optional<std::string> acknowledge_level,
			   boost::optional<std::string> tag,
			   boost::optional<bool> journal,
			   boost::optional<int> majority,
			   boost::optional<int> timeout, boost::optional<int> nodes,
			   boost::optional<bool> ordered,
			   boost::optional<bool> bypass_document_validation);

std::string
find(std::string username, std::string database_name,
	 bsoncxx::types::b_document query_document,
	 boost::optional<bsoncxx::types::b_document> projection_document,
	 boost::optional<bsoncxx::types::b_document> sort_document,
	 boost::optional<bsoncxx::types::b_document> min_document,
	 boost::optional<bsoncxx::types::b_document> max_document,
	 boost::optional<size_t> limit_number_of_docs = 0);

std::string
find_one(std::string username, std::string database_name,
	 bsoncxx::types::b_document query_document,
	 boost::optional<bsoncxx::types::b_document> projection_document,
	 boost::optional<bsoncxx::types::b_document> sort_document,
	 boost::optional<bsoncxx::types::b_document> min_document,
	 boost::optional<bsoncxx::types::b_document> max_document);

std::string
update_many(std::string username, std::string database_name,
		bsoncxx::types::b_document filter_document,
		bsoncxx::types::b_document update_document,
		boost::optional<bsoncxx::types::b_document> collation,
		boost::optional<std::string> acknowledge_level,
		boost::optional<std::string> tag, boost::optional<bool> journal,
		boost::optional<int> majority, boost::optional<int> timeout,
		boost::optional<int> nodes, boost::optional<bool> upsert,
		boost::optional<bool> bypass_document_validation);

std::string update_one(std::string username, std::string database_name,
			   bsoncxx::types::b_document filter_document,
			   bsoncxx::types::b_document update_document,
			   boost::optional<bsoncxx::types::b_document> collation,
			   boost::optional<std::string> acknowledge_level,
			   boost::optional<std::string> tag,
			   boost::optional<bool> journal,
			   boost::optional<int> majority,
			   boost::optional<int> timeout, boost::optional<int> nodes,
			   boost::optional<bool> upsert,
			   boost::optional<bool> bypass_document_validation);

std::string find_one_and_update(
	std::string username, std::string database_name,
	bsoncxx::types::b_document filter_document,
	bsoncxx::types::b_document update_document,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level,
	boost::optional<std::string> tag, boost::optional<bool> journal,
	boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time,
	boost::optional<bool> upsert);

std::string find_one_and_replace(
	std::string username, std::string database_name,
	bsoncxx::types::b_document filter_document,
	bsoncxx::types::b_document replacement,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level,
	boost::optional<std::string> tag, boost::optional<bool> journal,
	boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time,
	boost::optional<bool> upsert,
	boost::optional<bool> bypass_document_validation);

std::string delete_many(std::string username, std::string database_name,
			bsoncxx::types::b_document query_document,
			boost::optional<bsoncxx::types::b_document> collation,
			boost::optional<std::string> acknowledge_level,
			boost::optional<std::string> tag,
			boost::optional<bool> journal,
			boost::optional<int> majority,
			boost::optional<int> timeout,
			boost::optional<int> nodes);

std::string delete_one(std::string username, std::string database_name,
			   bsoncxx::types::b_document query_document,
			   boost::optional<bsoncxx::types::b_document> collation,
			   boost::optional<std::string> acknowledge_level,
			   boost::optional<std::string> tag,
			   boost::optional<bool> journal,
			   boost::optional<int> majority,
			   boost::optional<int> timeout,
			   boost::optional<int> nodes);

std::string find_one_and_delete(
	std::string username, std::string database_name,
	bsoncxx::types::b_document query_document,
	boost::optional<bsoncxx::types::b_document> projection_document,
	boost::optional<bsoncxx::types::b_document> sort_document,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<std::string> acknowledge_level,
	boost::optional<std::string> tag, boost::optional<bool> journal,
	boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes, boost::optional<int> max_time);

std::string count(std::string username, std::string database_name,
		  bsoncxx::types::b_document query_document,
		  boost::optional<size_t> limit_number_of_docs = 0,
		  boost::optional<size_t> skip_number_of_docs = 0);

std::string drop(std::string username, std::string database_name);

std::string create_index(
	std::string username, std::string database_name,
	bsoncxx::types::b_document index_document, bool options_is_set,
	boost::optional<bool> background, boost::optional<bool> unique,
	boost::optional<bool> sparse, boost::optional<std::int32_t> version,
	boost::optional<std::uint8_t> twod_sphere_version,
	boost::optional<std::uint8_t> twod_bits_precision,
	boost::optional<double> twod_location_max,
	boost::optional<double> twod_location_min,
	boost::optional<double> haystack_bucket_size,
	boost::optional<size_t> expire_after, boost::optional<std::string> name,
	boost::optional<std::string> default_language,
	boost::optional<std::string> language_override,
	boost::optional<bsoncxx::types::b_document> collation,
	boost::optional<bsoncxx::types::b_document> weights,
	boost::optional<bsoncxx::types::b_document> partial_filter_expression,
	boost::optional<size_t> max_time,
	boost::optional<std::string> acknowledge_level,
	boost::optional<std::string> tag, boost::optional<bool> journal,
	boost::optional<int> majority, boost::optional<int> timeout,
	boost::optional<int> nodes);

std::string name(std::string username, std::string database_name);

std::string list_indexes(std::string username, std::string database_name);

std::string distinct(std::string username, std::string database_name,
			 std::string name_string,
			 bsoncxx::types::b_document filter_document,
			 boost::optional<bsoncxx::types::b_document> collation,
			 boost::optional<size_t> max_time);

void aggregate_oprate(mongocxx::pipeline &pipeline,
			  bsoncxx::types::b_document doc);
std::string aggregate(std::string username, std::string database_name,
			  optional_bool allow_disk_use, optional_bool use_cursor,
			  optional_bool bypass_document_validation,
			  optional_size max_time, optional_int batch_size,
			  optional_ducument collation,
			  optional_string acknowledge_level, optional_string tag,
			  optional_bool journal, optional_int majority,
			  optional_int timeout, optional_int nodes,
			  optional_ducument hint, optional_string hint_str,
			  optional_array pipeline_array);
} // namespace database
} // namespace iotdb

#endif // DATABASE_COLLECTIONS_METHODS_H
