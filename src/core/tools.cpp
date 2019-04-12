#include "tools.h"
#include <utility>

#include <boost/property_tree/ptree.hpp>

// mongocxx
#include <mongocxx/exception/exception.hpp>

using boost::property_tree::ptree;

std::string iotdb::core::tools::add_date_time_to_query(std::string request)
{
	// TODO comment
	std::stringstream ss;
	ss << request;

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(ss, pt);

	// get now time and date
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	std::string signup_date_time = oss.str();

	try {
		auto query_test = pt.get_child("query");
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;

		if (strcmp(e.what(), "No such node (query)") == 0) {
			return "missing query";
		}
	}

	pt.put("query.date_time", signup_date_time);

	std::ostringstream oss1;

	try {
		write_json(oss1, pt);
	}
	catch (std::exception &e) {
		if (strcmp(e.what(), "<unspecified file>: ptree contains data "
					 "that cannot be represented in JSON "
					 "format") == 0) {
			return "wrong query";
		}
	}

	return oss1.str();
}

std::string iotdb::core::tools::add_date_time_to_querys(std::string request)
{
	// TODO comment
	std::stringstream ss;
	ss << request;

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(ss, pt);

	// get now time and date
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	std::string signup_date_time = oss.str();

	try {
		auto query_test = pt.get_child("query");
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;

		if (strcmp(e.what(), "No such node (query)") == 0) {
			return "missing query";
		}
	}

	BOOST_FOREACH (boost::property_tree::ptree::value_type &framePair2,
			   pt.get_child("query")) {
		//	   if
		//(!framePair2.second.get<std::string>("id").compare("stackoverflow")){
		//			cout << framePair2.second.get<std::string>("id")
		//<<
		// endl;
		//			cout <<
		// framePair2.second.get<std::string>("visible")
		//<<
		// endl;
		//			framePair2.second.put<string>("visible",
		//"false");
		//			cout <<
		// framePair2.second.get<std::string>("visible")
		//<<
		// endl;
		//	   }
		framePair2.second.put("date_time", signup_date_time);
	}

	// pt.put("query.date_time", signup_date_time);

	std::ostringstream oss1;

	try {
		write_json(oss1, pt);
	}
	catch (std::exception &e) {
		if (strcmp(e.what(), "<unspecified file>: ptree contains data "
					 "that cannot be represented in JSON "
					 "format") == 0) {
			return "wrong query";
		}
	}

	return oss1.str();
}
