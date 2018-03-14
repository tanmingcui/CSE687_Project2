/////////////////////////////////////////////////////////////////////
// Executive.cpp - Test all Repository and Nosql function          //
// ver 1.0                                                         //
// Tanming Cui, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DbCore/DbCore.h"
#include "../DBQuery/Conditions.h"
#include "../Filemgr/file.h"
#include "../CoreRepository/CoreRepository.h"
using namespace Repository;
auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	try {
		Utilities::title("Demonstrating Requirement #1");
		std::cout << "\n  " << typeid(std::function<bool()>).name()
			<< ", declared in this function, "
			<< "\n  is only valid for C++11 and later versions.";
		putLine();
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	try {
		Utilities::title("Demonstrating Requirement #2");
		std::cout << "\n  A visual examination of all the submitted code "
			<< "will show only\n  use of streams and operators new and delete.";
		putLine();
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
CoreRepo testrepo;
bool testR3()
{
	try {
		Utilities::title("Demonstrating Requirement #3");
		std::vector<std::string> currdirs = Directory::getDirectories("../");
		for (size_t i = 0; i < currdirs.size(); ++i)
			std::cout << "\n    " << currdirs[i].c_str();
		std::cout << "\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
bool testR4a() {
	try {
		Utilities::title("Demonstration Requirement #4");
		std::cout << "\n Open Check in by accepting a single package's files";
		std::string tocheckin = "../ClientFile/CSE686_Assignment2";
		std::cout << "\n  current directory is:\n    " << tocheckin;
		std::cout << "\n  It contains files:";
		std::vector<std::string> currfiles = Directory::getFiles(tocheckin);
		
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
			std::vector<std::string>child1;
			if(std::string(currfiles[i].c_str())=="JHTTP.java")child1.push_back("RequestProcessor.java");
			testrepo.Checkin(tocheckin+"/"+ std::string(currfiles[i].c_str()),"CSE686_Assignment2",true,"For Class CSE686 Internet Programming",child1);
		}
		std::cout << "\n  current directory is:\n    " << testrepo.RepoPath();
		std::cout << "\n  It contains files:";
		currfiles = Directory::getFiles(testrepo.RepoPath());
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
		}
		std::cout << "\n  Finished to check in one package.\n";
		testrepo.checkdependency("CSE686_Assignment2","JHTTP.java");
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}

bool testR4b() {
	try {
		Utilities::title("Demonstration Requirement #4");
		std::cout << "\n Close Check in by accepting a single package's files";
		std::string tocheckin = "../ClientFile/CSE687_Project1";
		std::cout << "\n  current directory is:\n    " << tocheckin;
		std::cout << "\n  It contains files:";
		std::vector<std::string> currfiles = Directory::getFiles(tocheckin);
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
			std::vector<std::string>child1;
			if (std::string(currfiles[i].c_str()) == "DateTime.cpp")child1.push_back("DateTime.h");
			if (std::string(currfiles[i].c_str()) == "DbCore.cpp") {
				child1.push_back("DbCore.h");
				child1.push_back("StringUtilities.h");
				child1.push_back("TestUtilities.h");
			}
			if (std::string(currfiles[i].c_str()) == "DbCore.h") child1.push_back("DateTime.h");
			if(std::string(currfiles[i].c_str())=="StringUtilities.cpp")child1.push_back("StringUtilities.h");
			if (std::string(currfiles[i].c_str()) == "TestUtilities.cpp")child1.push_back("TestUtilities.h");
			testrepo.Checkin(tocheckin + "/" + std::string(currfiles[i].c_str()), "CSE687_Project1", false, "For Class CSE687 Obeject-Oriented Design", child1);
		}
		std::cout << "\n  current directory is:\n    " << testrepo.RepoPath();
		std::cout << "\n  It contains files:";
		currfiles = Directory::getFiles(testrepo.RepoPath());
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
		}
		std::cout << "\n  Finished to check in one package.\n";
		std::cout << "\n";
		std::cout << "Check all the dependency files for DbCore.cpp in CSE687_Project1 package \n";
		testrepo.checkdependency("CSE687_Project1", "DbCore.cpp");
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
bool testR4c() {
	try {
		Utilities::title("Demonstration Requirement #4");
		std::cout << "\n Close Check in again by accepting the same single package's files";
		std::string tocheckin = "../ClientFile/CSE687_Project1";
		std::cout << "\n  current directory is:\n    " << tocheckin;
		std::cout << "\n  It contains files:";
		std::vector<std::string> currfiles = Directory::getFiles(tocheckin);
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
			std::vector<std::string>child1;
			if (std::string(currfiles[i].c_str()) == "DateTime.cpp")child1.push_back("DateTime.h");
			if (std::string(currfiles[i].c_str()) == "DbCore.cpp") {
				child1.push_back("DbCore.h");
				child1.push_back("StringUtilities.h");
				child1.push_back("TestUtilities.h");
			}
			if (std::string(currfiles[i].c_str()) == "DbCore.h") child1.push_back("DateTime.h");
			if (std::string(currfiles[i].c_str()) == "StringUtilities.cpp")child1.push_back("StringUtilities.h");
			if (std::string(currfiles[i].c_str())== "TestUtilities.cpp")child1.push_back("TestUtilities.h");
			testrepo.Checkin(tocheckin + "/" + currfiles[i].c_str(), "CSE687_Project1", false, "For Class CSE687 Obeject-Oriented Design", child1);
		}
		std::cout << "\n  current directory is:\n    " << testrepo.RepoPath();
		std::cout << "\n  It contains files:";
		currfiles = Directory::getFiles(testrepo.RepoPath());
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
		}
		std::cout << "\n  Finished to check in one package.\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
bool testR4d() {
	try {
		std::string tocheckin = "../ClientFile/CSE681_Project4";
		std::cout << "\n  current directory is:\n    " << tocheckin;
		std::cout << "\n  It contains files:";
		std::vector<std::string> currfiles = Directory::getFiles(tocheckin);
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
			std::vector<std::string>child1;
			if (std::string(currfiles[i].c_str()) == "Motherbuilder.cs") {
				child1.push_back("IMPCommService.cs");
				child1.push_back("MPCommService.cs");
				child1.push_back("BlockingQueue.cs");
			}
			if (std::string(currfiles[i].c_str()) == "Childbuilder.cs") {
				child1.push_back("IMPCommService.cs");
				child1.push_back("MPCommService.cs");
				child1.push_back("BlockingQueue.cs");
			}
			if (std::string(currfiles[i].c_str()) == "MPCommService.cs") child1.push_back("IMPCommService.cs");
			
			testrepo.Checkin(tocheckin + "/" + currfiles[i].c_str(), "CSE681_Project4", true, "For Class CSE681 Software Modeling Analysis", child1);
		}
		std::cout << "\n  current directory is:\n    " << testrepo.RepoPath();
		std::cout << "\n  It contains files:";
		currfiles = Directory::getFiles(testrepo.RepoPath());
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
		}
		std::cout << "\n  Finished to check in one package.\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;

}
bool testR5a() {
	try {
		Utilities::title("Demonstration Requirement #5");
		std::cout << "\n  Browsing package CSE686_Assignment2";
		Query<FileInfo> origin = testrepo.repoquery();
		Conditions<FileInfo> pack;
		pack.descrip("CSE686_Assignment2");
		NoSQLDB<FileInfo> res = origin.select(pack).query();
		for (auto item : res.nosqldb()) {
			std::string filename = item.second.name();
			std::cout << "\n----------------------------------------------------------------------------------------------------------";
			std::cout << "\n   Now display the latest version content of the file " << filename << " in "<< item.second.descrip()<<" package";
			std::cout << "\n----------------------------------------------------------------------------------------------------------";
			testrepo.Browse(filename);
			std::cout << "\n";
		}

	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
bool testR5b() {
	try {
		Utilities::title("Demonstration Check Out function");
		std::cout << "\n  Checkout the file DbCore.h in Repository ";
		std::string thelatest = testrepo.findlatest("DbCore.h");
		std::cout << "\n The latest version of DbCore.h in Repository is " << thelatest;
		testrepo.Checkout("DbCore.h");
		std::cout << "\n  current directory is:\n    " << testrepo.CheckoutPath();
		std::cout << "\n  It contains files:";
		std::vector<std::string>currfiles = Directory::getFiles(testrepo.CheckoutPath());
		for (size_t i = 0; i < currfiles.size(); ++i) {
			std::cout << "\n    " << currfiles[i].c_str();
		}
		std::cout << "\n  Finished to check in one package.\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		return false;
	}
	return true;
}
TestExecutive ex;
int main() {
	Utilities::Title("Testing CSE687 Project #2");
	putLine();
	TestExecutive::TestStr ts1{ testR1, "Use Visual Studio 2017 and the standard C++ libraries" };
	TestExecutive::TestStr ts2{ testR2, "Use C++ standard library's streams for all I/O and new and delete for all heap-based memory management" };
	TestExecutive::TestStr ts3{ testR3, "The project provides C++ packages" };
	TestExecutive::TestStr ts4a{ testR4a, "Open Check in single package" };
	TestExecutive::TestStr ts4b{ testR4b, "Close Check in single package" };
	TestExecutive::TestStr ts4c{ testR4c, "Close Check in agaian the same single package" };
	TestExecutive::TestStr ts4d{ testR4d, "Open Check in agaian the same single package" };
	TestExecutive::TestStr ts5a{ testR5a, "Browsing package CSE686_Assignment2" };
	TestExecutive::TestStr ts5b{ testR5b, "Checkout the file DbCore.h in Repository" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4a);
	ex.registerTest(ts4b);
	ex.registerTest(ts4c);
	ex.registerTest(ts4d);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	bool result = ex.doTests();
	if (result == true)std::cout << "\n  all tests passed";
	else std::cout << "\n  at least one test failed";
	putLine(2);
	return 0;
}