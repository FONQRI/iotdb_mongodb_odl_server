#include "homepagecontroller.h"
#include "src/core/reply.h"
#include "src/database/collection_methods.h"

// mongocxx
#include <mongocxx/exception/exception.hpp>

#include <iostream>
#include <vector>

iotdb::core::home_page_controller::home_page_controller(
	http::server::reply &rep, http::server::request request)
{

	//	bsoncxx::document::value document =
	//	bsoncxx::from_json("{\"query\":{\"hello\":\"test\" }}");

	//	bsoncxx::types::b_document document_query =
	//	document.view()["query"].get_document();
	//	std::clog << bsoncxx::to_json(document_query.value);

	//	auto reply = iotdb::database::insert_many("userName", "dbname",
	//						  "[{\"hello\":\"test\" }]");

	// add headers
	//	specifying content type as json
	http::server::header content_type;
	content_type.name = "Content-Type";
	content_type.value = "text/html;";

	//	specifying content charset as utf-8
	http::server::header charset;
	charset.name = "charset";
	charset.value = "utf-8";

	std::string reply =
	"<!DOCTYPE html>"
	"		<html>"
	"	<body><head><style> *{    margin: 0;    padding: 0;}body{    "
	"color: #eee;   "

	"background: url('..images/page_bg.jpg') no-repeat center top #1b1b1c; "
	"   font: 15pxCalibri,Arial,sans-serif;    border-top: 5px solid "
	"#212121;}#page{    width:330px;    margin: 70px auto 100px;}#page h1{ "
	"   text-indent: -9999px;   overflow: hidden;    height: 105px;    "
	"background: "
	"url('http://dc703.4shared.com/imgyxxU3nVM/s7/0.7548750905549155/"
	"coming_soon.png') no-repeat;}</style> </head><body><divid=\"page\">   "
	"     <h1>    "
	"        Coming Soon</h1>    </div></body></html>";

	std::clog << reply << std::endl;
	rep.content.append(reply.c_str(), reply.size());
}
