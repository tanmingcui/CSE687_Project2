/////////////////////////////////////////////////////////////////////
// testcorepo.cpp - Test repository core functions                 //
// ver 1.0                                                         //
// Tanming Cui, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
#include "CoreRepository.h"
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>
using namespace Repository;
#ifdef TEST_COREREPO

int main() {
	CoreRepo repo;
	std::vector<std::string> included;
	included.push_back("DataTime.h");
	included.push_back("DbCore.h");
	repo.Checkin("../ClientFile/Conditions.h","CSE687_Project1",true,"header file for DBQuery.cpp",included);
	repo.Checkout("Conditions.h");
	repo.Browse("../Repository/Conditions.h.v1");
	repo.Browse("../CheckoutFile/Conditions.h");
}

#endif 
