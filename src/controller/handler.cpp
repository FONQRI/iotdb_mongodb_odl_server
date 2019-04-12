#include "handler.h"

#include "homepagecontroller.h"

#include "collection/delete/delete_many.h"
#include "collection/delete/delete_one.h"
#include "collection/delete/find_one_and_delete.h"
#include "collection/find/find.h"
#include "collection/find/find_one.h"
#include "collection/get_info/count.h"
#include "collection/get_info/list_indexes.h"
#include "collection/get_info/name.h"
#include "collection/insert/insert_many.h"
#include "collection/insert/insert_one.h"
#include "collection/update/create_index.h"
#include "collection/update/find_one_and_update.h"
#include "collection/update/update_many.h"
#include "collection/update/update_one.h"

#include "user/update_key.h"

#include <iostream>

/**
 * @brief controller	: a function for controller mapping
 * @param rep			: reply of http
 * @param request		: request of http
 */
typedef void controller(http::server::reply &rep,
			http::server::request request);

/**
 * @brief function_mapper : controller mapper type
 */
typedef std::map<std::string, controller *> function_mapper;

/**
 * @brief controller_mapper
 */
extern function_mapper controller_mapper;

// handler
iotdb::core::handler::handler(http::server::request request,
				  http::server::reply &rep)
{

	// split url
	std::istringstream string_stream(request.uri);
	std::string temp_str;
	std::vector<std::string> url_list;

	while (getline(string_stream, temp_str, '/')) {
		url_list.push_back(temp_str);
	}

	// map controllers
	if (url_list.size() < 2) {
		home_page_controller controller(rep, request);
	}
	else if (controller_mapper[url_list.at(1)] != 0) {
		controller_mapper[url_list.at(1)](rep, request);
	}
}
