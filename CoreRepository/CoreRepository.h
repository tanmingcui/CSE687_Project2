/////////////////////////////////////////////////////////////////////
// CoreRepository.h - Implements Core Repository function          //
// ver 1.0                                                         //
// Tanming Cui, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* FileInfo class:
* The FileInfo plays for the extension of payload of the nosql database,
* it records the fullfilepath, version number, open/clsoe for check in,
* and catagory information
* 
* CoreRepo class:
* The CoreRepo class has the main function of a repository, Checkin, 
* Check out, version, and browse the content of specific file, itself
* set its repository path and check out file path.
*
* This package primarily provides functions:
* CoreRepo::Checkin(file)             Check in file from client
* CoreRepo::isExist(file)             Whether the file exisit in repository
* CoreRepo::findlatest(file)          Get the latest version of  the file in repository
* CoreRepo::Checkout(file)            Check out file to a target position
* CoreRepo::checkdependency(file)     Check all the dependency for the file
* Required Files:
* ---------------
* Conditions.h file.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 05 Mar 2018
* - primary nosql database query function
* - first release
*/
#pragma once
#include"../DBQuery/Conditions.h"
#include"../Filemgr/file.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include<unordered_set>
using namespace NoSqlDb;
namespace Repository {
	class FileInfo {
	public:
		FileInfo() {_FilePath = ""; _isOpen = true; _version = 0;}
		std::string& getFilePath() { return _FilePath; }
		void putFilePath(const std::string& thepath) { _FilePath = thepath; }
		bool isOpen() { return _isOpen; }
		void putOpen(const bool open) { _isOpen = open; }
		int verNum() { return _version; }
		void putvsrNum(const int num) { _version = num; }
		std::string& getCata() { return _Catagory; }
		void putCata(const std::string& catagory) { _Catagory = catagory; }
	private:
		std::string _FilePath;
		bool _isOpen;
		int _version;
		std::string _Catagory;
	};
	class CoreRepo {
	public:
		CoreRepo() { _RepoPath = "../Repository/"; _CheckoutPath = "../CheckoutFile/"; }
		void Checkin(const std::string& filepath, const std::string& _descrip, bool open,const std::string& CatagoryInfo, const std::vector<std::string>& childs);
		Query<FileInfo> repoquery() { return _Repoquery; }
		bool isExisit(const std::string& thename);
		std::string findlatest(const std::string& filename);
		void Checkout(const std::string& filename);
		std::string tearoff(const std::string& fullname);
		void Browse(const std::string& filename);
		std::string& RepoPath() { return _RepoPath; }
		std::string& CheckoutPath() { return _CheckoutPath; }
		bool checkdependency(const std::string& packagename, const std::string& filename);
	private:
		NoSQLDB<FileInfo> _coreRepo;
		Query<FileInfo> _Repoquery = Query<FileInfo>(_coreRepo);
		std::string _RepoPath;
		std::string _CheckoutPath;
		void depcheckhelper(const std::string& filename, const std::unordered_map<std::string, std::vector<std::string>>, std::vector<std::string >& output);
		void firstcheckin(const std::string& filepath, const std::string& _descrip, bool open, const std::string& CatagoryInfo, const std::vector<std::string>& childs, int versions);
	};
	bool CoreRepo::isExisit(const std::string& thename) {
		Conditions<FileInfo> forSearch;
		forSearch.name(thename);
		_Repoquery = Query<FileInfo>(_coreRepo);
		Query<FileInfo> theres = _Repoquery.select(forSearch);
		NoSQLDB<FileInfo> resdb = theres.query();
		for (auto item : resdb.nosqldb()) {
			DbElement<FileInfo> ele = item.second;
			if (ele.name() == thename)return true;
		}
		return false;
	}
	void CoreRepo::Browse(const std::string& filename) {
		std::string path = _RepoPath + findlatest(filename);
		path = Path::getFullFileSpec(path);
		File bufferIn(path);
		bufferIn.open(File::in, File::binary);
		if (!bufferIn.isGood())std::cout << "\n  could not open \"" << filename << "\" for reading";
		else std::cout << "\n  opening: \"" << filename << "\" for reading";
		const size_t bufLen = 124;
		File::byte buffer[bufLen];
		while (true){
			size_t resultSize = bufferIn.getBuffer(bufLen, buffer);
			std::string temp1(buffer, resultSize);
			std::cout << temp1;
			if (resultSize < bufLen || !bufferIn.isGood())break;
		}
	}
	void showcheckouterror(const std::string& filename) {
		std::cout << "\n";
		std::cout << "==================================";
		std::cout << "Cannot find the file " + filename;
		std::cout << "==================================";
		std::cout << "\n";
	}
	
	void CoreRepo::Checkout(const std::string& filename) {
		if (isExisit(filename)) {
			std::string tocheckout =_RepoPath + findlatest(filename);
			File me(tocheckout);
			me.open(File::in, File::binary);
			if (!me.isGood()) {
				std::cout << "\n  can't open executable\n";
				std::cout << "\n  looking for:\n  ";
				std::cout << Path::getFullFileSpec(me.name()) << "\n";
			}
			else {
				std::string des = _CheckoutPath + filename;
				File you(des);
				you.open(File::out, File::binary);
				if (you.isGood()) {
					while (me.isGood()) {
						static size_t count = 0;
						Block b = me.getBlock(1024);
						you.putBlock(b);
						if (++count < 10) {
							std::cout << "\n    reading block of " << b.size() << " bytes";
							std::cout << "\n    writing block of " << b.size() << " bytes";
						}
						if (b.size() < 1024) {
							std::cout << "\n\n    omitted " << count - 10 << " blocks from display\n\n";
							std::cout << "\n    reading block of " << b.size() << " bytes";
							std::cout << "\n    writing block of " << b.size() << " bytes";
						}
					}
					std::cout << "\n";
				}
				}
			}
		else showcheckouterror(filename);
	}
	std::string CoreRepo::tearoff(const std::string& fullname) {
		std::size_t length = fullname.length() - 1;
		std::size_t pos = length - 1;
		for (; pos > 0 && fullname[pos] != 'v'; pos--);
		std::size_t start = 0;
		std::string res (fullname.substr(start, pos - 1));
		return res;
	}
	std::string CoreRepo::findlatest(const std::string& filename) {
		int largest = 0;
		std::string largestfile("");
		Conditions<FileInfo> forSearch;
		forSearch.name(filename);
		Query<FileInfo> origin = _Repoquery;
		Query<FileInfo> theres = _Repoquery.select(forSearch);
		_Repoquery = origin;
		NoSQLDB<FileInfo> resdb = theres.query();
		for (auto item : resdb.nosqldb()) {
			DbElement<FileInfo> ele = item.second;
			if (ele.name() == filename) {
				std::size_t length = item.first.length();
				std::size_t pos = length - 1;
				std::string _name(item.first);
				for (; pos > 0 && _name[pos] != 'v'; pos--);
				std::size_t startpos = 0;
				std::string version(_name.erase(startpos, pos + 1));
				int thever = std::stoi(version);
				if (thever > largest) {
					largest = thever;
					largestfile = item.first;
				}
			}
		}
		return largestfile;
	}
	void CoreRepo::firstcheckin(const std::string& filepath, const std::string& _descrip, bool open, const std::string& CatagoryInfo, const std::vector<std::string>& childs,int versions) {
		File me(filepath);
		me.open(File::in, File::binary);
		if (!me.isGood()){
			std::cout << "\n  can't open executable\n";
			std::cout << "\n  looking for:\n  ";
			std::cout << Path::getFullFileSpec(me.name()) << "\n";
		}
		else {
			DbElement<FileInfo> newFile;
			newFile.name(Path::getName(filepath));
			newFile.dateTime(DateTime().now());
			newFile.descrip(_descrip);
			newFile.children(childs);
			FileInfo newInfo;
			newInfo.putvsrNum(versions);
			std::string filename = Path::getName(filepath);
			filename = filename + +"." + "v" + std::to_string(newInfo.verNum());
			std::string des = _RepoPath + filename;
			newInfo.putFilePath(des);
			newInfo.putCata(CatagoryInfo);
			newInfo.putOpen(open);
			newFile.payLoad(newInfo);
			_coreRepo=_coreRepo.add(filename, newFile);
			_Repoquery = Query<FileInfo>(_coreRepo);
			File you(des);
			you.open(File::out, File::binary);
			if (you.isGood()) {
				while (me.isGood()) {
					static size_t count = 0;
					Block b = me.getBlock(1024);
					you.putBlock(b);
					if (++count < 10) {
						std::cout << "\n    reading block of " << b.size() << " bytes";
						std::cout << "\n    writing block of " << b.size() << " bytes";
					}
					if (b.size() < 1024) {
						std::cout << "\n\n    omitted " << count - 10 << " blocks from display\n\n";
						std::cout << "\n    reading block of " << b.size() << " bytes";
						std::cout << "\n    writing block of " << b.size() << " bytes";
					}
				}
				std::cout << "\n";
			}
		}
	}
	void CoreRepo::Checkin(const std::string& filepath, const std::string& _descrip, bool open,const std::string& CatagoryInfo, const std::vector<std::string>& childs) {
		if (!Directory::exists(_RepoPath)) Directory::create(_RepoPath);
		std::string filename = Path::getName(filepath);
		if (isExisit(filename)) {
			std::string key;
			key = findlatest(filename);
			DbElement<FileInfo> theinfo = _coreRepo.nosqldb()[key];
			if (theinfo.payLoad().isOpen()) {
				int version = theinfo.payLoad().verNum();
				firstcheckin(filepath, _descrip, open, CatagoryInfo, childs, version);
			}
			else {
				int version = theinfo.payLoad().verNum();
				firstcheckin(filepath, _descrip, open, CatagoryInfo, childs, version+1);
			}
		}
		if (!isExisit(filename)) firstcheckin(filepath, _descrip, open, CatagoryInfo, childs,1);			
	}
	bool CoreRepo::checkdependency(const std::string& packagename, const std::string& filename) {
		Conditions<FileInfo> search;
		search.descrip(packagename);
		std::unordered_map<std::string,std::vector<std::string>> deplist;
		Query<FileInfo> Origin = _Repoquery;
		NoSQLDB<FileInfo> result= _Repoquery.select(search).query();
		_Repoquery = Origin;
		for (auto item : result.nosqldb()) {
			DbElement<FileInfo>ele = item.second;
			if ((ele.children().size() > 0)&&(deplist.find(ele.name())==deplist.end())) {
				std::vector<std::string> childs = ele.children();
				deplist[ele.name()] = childs;
			}
		}
		std::vector<std::string>output;
		if (deplist.size() > 0)depcheckhelper(filename,deplist,output);
		for (auto item : output){
			if (!isExisit(item)) {
				std::cout << "\n   Does not have the dependency file: " << item;
				std::cout << "\n";
				return false;
			}
		}
		
		for (auto item : output)std::cout << "\n" << item;
		std::cout << "\n";
		std::cout << "All dependecy file for "<< filename<<" exisit!";
		std::cout << "\n";
		return true;
	}
	void CoreRepo::depcheckhelper(const std::string& filename, std::unordered_map<std::string, std::vector<std::string>> deplist, std::vector<std::string >& output) {
		if (deplist.find(filename) == deplist.end()) return;
		std::vector<std::string> childlist = deplist[filename];
		for (std::string file : childlist) {
			depcheckhelper(file, deplist, output);
			std::vector<std::string> list = output;
			list.push_back(file);
			output.push_back(file);
		}
	}
}

