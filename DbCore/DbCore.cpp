/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Tanming Cui, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};


///////////////////////////////////////////////////////////////////////
// test functions

//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	NoSQLDB<std::string> nosql1; DbCore<std::string> db;
	DbElement<std::string> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	nosql1.add("Salman", demoElem);
	if (!db.contains("Salman"))return false;
	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;
	nosql1.add("Sun", demoElem);
	demoElem.payLoad("Tanming Cui");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	nosql1.add("Parashar", demoElem);
	demoElem.payLoad("TanmingCui");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	demoElem.payLoad("Tanming Cui");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	if (db.size() != 6)return false;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();
	std::cout << "\n  showing all the database elements:";
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);
	putLine();
	return true;
}
//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities;

int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };

	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts3b);

	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}
#endif
