#pragma once
/////////////////////////////////////////////////////////////////////
// Conditions.h - Implements NoSql database query function         //
// ver 1.0                                                         //
// Tanming Cui, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* Conditions class provide the query condtions that can be used for search 
* specific content in the nosql database
*
* Query class provide the select and from two primary query function for
* the nosql database
*
* This package primarily provides functions:
* - Query<T> select(value)      return the sets contain the specific value
* - Query<T> from(key)          return the sets contain the specific key
*
* The package also provides functions for displaying:
* - showres()            the result of query
* - showCond()           display the query condition
* Required Files:
* ---------------
* DbCore.h,DateTime.h,TestUtilities.h,StringUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 05 Feb 2018
* - primary nosql database query function
* - first release
*/

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include<unordered_set>
#include "../DateTime/DateTime.h"
#include"../DbCore/DbCore.h"

namespace NoSqlDb {
	/////////////////////////////////////////////////////////////////////
	// Conditions class
	// - provides the query conditions
	template<typename T>class Conditions
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;

		// methods to get and set Conditions fields
		Key& name() { return name_; }
		Key name() const { return name_; }
		void name(const Key& name) { name_ = name; }

		Key& descrip() { return descrip_; }
		Key descrip() const { return descrip_; }
		void descrip(const Key& name) { descrip_ = name; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		void children(const Children& children) { children_ = children; }
	private:
		Key name_;
		Key descrip_;
		DateTime dateTime_;
		Children children_;
	};

	/////////////////////////////////////////////////////////////////////
	// Query class
	// - provides the basic query functions
	template<typename T>class Query {
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using MultiConds = std::vector<Conditions<T>>;
		Query(const NoSQLDB<T>& nosqldbq) { nosqldbq_ = nosqldbq; }	
		NoSQLDB<T>& query() { return nosqldbq_; }
		NoSQLDB<T> query()const { return nosqldbq_; }

		//method to pick up the specific data sets according to the key
		Query<T> from(const Key& key)const;	
		Query<T>& from(const Key& key);	
		Query<T> from(const Keys& keys) const;
		Query<T>& from(const Keys& keys);

		//method to pick up the specific data sets according to the 
		//provided value
		Query<T> select(const Conditions<T>& cond)const;
		Query<T>& select(const Conditions<T>& cond);
		Query<T> select(const MultiConds& conds)const;
		Query<T>& select(const MultiConds& conds);
		template<typename CallObj>
		Query& select(CallObj callObj);
		//method to show the query result
		void showres();
	private:
		NoSQLDB<T> nosqldbq_;
	};
	template<typename T>
	template<typename CallObj>
	Query<T>& Query<T>::select(CallObj callObj)
	{
		NoSQLDB<T> newone;
		for (auto ele = nosqldbq_.nosqldb().begin(); ele != nosqldbq_.nosqldb().end(); ++ele) {
			if (callObj(ele->second))newone.add(ele->first, ele->second);
		}
		nosqldbq_ = newone;
		return *this;
	}
	
	//----<methond to show the query result>----------------------------------------------
	template<typename T>
	void Query<T>::showres() {
		std::ostream& out = std::cout;
		out << "\n";
		out << "\n The query result is: ";
		showHeader(out);
		for (auto item : nosqldbq_.nosqldb()) {
			DbElement<T> temp = item.second;
			showElem(temp,out);
		}
	}

	//----<methond to pick up the specific data sets according to the provided key>-------
	template<typename T> 
	Query<T> Query<T>::from(const Keys& keys) const {
		NoSQLDB<T> newone;
		for (auto item : keys) {
			if (nosqldbq_.nosqldb().contains(item)) {
				DbElement<T> val = nosqldbq_.nosqldb()[item];
				newone.add(key, val);
			}
		}
		nosqldbq_ = newone;
		return *this;
	}

	//----<methond to pick up the specific data sets according to the provided key>-------
	template<typename T> 
	Query<T>& Query<T>::from(const Keys& keys) {
		NoSQLDB<T> newone;
		for (auto item : keys) {
			if (nosqldbq_.nosqldb().contains(item)) {
				DbElement<T> val = nosqldbq_.nosqldb()[item];
				newone.add(key, val);
			}
		}
		nosqldbq_ = newone;
		return *this;
	}

	//----<methond to pick up the specific data sets according to the provided key>-------
	template<typename T>
	Query<T>& Query<T>::from(const Key& key) {
		if (key.size() > 0) {
			NoSQLDB<T> newone;
			if (nosqldbq_.nosqldb().contains(key)) {
				DbElement<T> val = nosqldbq_.nosqldb()[key];
				newone.add(key,val);
			}
			nosqldbq_ = newone;
		}		
		return *this;
	}

	//----<methond to pick up the specific data sets according to the provided key>-------
	template<typename T>
	Query<T> Query<T>::from(const Key& key)const {
		if (key.size() > 0) {
			NoSQLDB<T> newone;
			if (nosqldbq_.nosqldb().contains(key)) {
				DbElement<T> val = nosqldbq_.nosqldb()[key];
				newone.add(key, val);
			}
			nosqldbq_ = newone;
		}
		return *this;
	}
	
	//----<methond to pick up the specific data sets according to the provided value>-----
	template<typename T>Query<T> Query<T>::select(const MultiConds& conds)const {
		std::vector<NoSQLDB<T>> resseq;
		NoSQLDB<T> result;
		for (auto cond : conds) {
			NoSQLDB<T> copy = nosqldbq_;
			copy = namehelper(copy, cond);
			copy = descriphelper(copy, cond);
			copy = datetimehelper(copy, cond);
			copy = childrenhelper(copy, cond);
			copy = payloadhelper(copy, cond);
			resseq.push_back(copy);
		}
		for (NoSQLDB<T> item : resseq) {
			DbCore<T> resultcore = result.nosqldb();
			DbCore<T> itemcore = item.nosqldb();
			for (auto eachkey : itemcore.keys()) {
				DbElement<T> eachelem = itemcore.getelement(eachkey);
				if (!resultcore.contains(eachkey))result.add(eachkey, eachelem);
			}
		}
		nosqldbq_ = result;
		return *this;
	}

	//----<methond to pick up the specific data sets according to the provided value>-----
	template<typename T>Query<T>& Query<T>::select(const MultiConds& conds) {
		std::vector<NoSQLDB<T>> resseq;
		NoSQLDB<T> result;
		for (auto cond : conds) {
			NoSQLDB<T> copy = nosqldbq_;
			copy = namehelper(copy, cond);
			copy = descriphelper(copy, cond);
			copy = datetimehelper(copy, cond);
			copy = childrenhelper(copy, cond);
			copy = payloadhelper(copy, cond);
			resseq.push_back(copy);
		}
		for (NoSQLDB<T> item : resseq) {
			DbCore<T> resultcore = result.nosqldb();
			DbCore<T> itemcore = item.nosqldb();
			for (auto eachkey : itemcore.keys()) {
				DbElement<T> eachelem = itemcore.getelement(eachkey);
				if (!resultcore.contains(eachkey))result.add(eachkey, eachelem);
			}
		}
		nosqldbq_ = result;
		return *this;
	}
	template<typename T>
	NoSQLDB<T> namehelper(NoSQLDB<T> nosqldbq1,const Conditions<T>& condition) {
		NoSQLDB<T> res;
		if (condition.name().size() > 0) {
			for (auto ele = nosqldbq1.nosqldb().begin(); ele != nosqldbq1.nosqldb().end(); ++ele) {
				DbElement<T> temp = ele->second;
				if (temp.name() == condition.name())res.add(ele->first, ele->second);
			}
			return res;
		}
		return nosqldbq1;
	}

	template<typename T>
	NoSQLDB<T> descriphelper(NoSQLDB<T> nosqldbq1, const Conditions<T>& condition) {
		NoSQLDB<T> res;
		if (condition.descrip().size() > 0) {
			for (auto ele = nosqldbq1.nosqldb().begin(); ele != nosqldbq1.nosqldb().end(); ++ele) {
				DbElement<T> temp = ele->second;
				if (temp.descrip() == condition.descrip())res.add(ele->first, ele->second);
			}
			return res;
		}
		return nosqldbq1;
		
	}

	template<typename T>
	NoSQLDB<T> childrenhelper(NoSQLDB<T> nosqldbq1, const Conditions<T>& condition) {
		NoSQLDB<T> res;
		if (condition.children().size()>0) {
			for (auto ele = nosqldbq1.nosqldb().begin(); ele != nosqldbq1.nosqldb().end(); ++ele) {
				int issame = 0;
				DbElement<T> temp = ele->second;
				for (auto childcond : condition.children()) {
					for (auto tempele : temp.children()) if (childcond == tempele)++issame;
				}
				if (issame == condition.children().size())res.add(ele->first, ele->second);
			}
			return res;
		}
		return nosqldbq1;
	}

	template<typename T>
	NoSQLDB<T> datetimehelper(NoSQLDB<T> nosqldbq1, const Conditions<T>& condition) {
		NoSQLDB<T> res;
		//error range
		DateTime cond = condition.dateTime();
		cond += DateTime::makeDuration(0, 10);
		if (cond<DateTime().now()) {
			for (auto ele = nosqldbq1.nosqldb().begin(); ele != nosqldbq1.nosqldb().end(); ++ele) {
				DbElement<T> temp = ele->second;
				DateTime tempdaytime = temp.dateTime();
				DateTime conddaytime = condition.dateTime();
				if (tempdaytime>conddaytime)res.add(ele->first, ele->second);
			}
			return res;
		}
		return nosqldbq1;
	}
	/*
	template<typename T>
	NoSQLDB<T> payloadhelper(NoSQLDB<T> nosqldbq1, const Conditions<T>& condition) {
		NoSQLDB<T>res;
		for (auto ele = nosqldbq1.nosqldb().begin(); ele != nosqldbq1.nosqldb().end(); ++ele) {
			DbElement<T> temp = ele->second;
			 callobj = condition.payLoad();
			if (callobj(ele->second))res.add(ele->first, ele->second);
		}
		DbCore<callobj> fortest = res.nosqldb;
		if(fortest.begin()!=fortest.end())return res;
		return nosqldbq1;
	}*/
	//----<methond to pick up the specific data sets according to the provided value>-----
	template<typename T>Query<T> Query<T>::select(const Conditions<T>& cond)const {
		nosqldbq_ = namehelper(nosqldbq_, cond);
		nosqldbq_ = descriphelper(nosqldbq_, cond);
		nosqldbq_ = datetimehelper(nosqldbq_,cond);
		nosqldbq_ = childrenhelper(nosqldbq_, cond);
		//nosqldbq_ = payloadhelper(nosqldbq_, cond);
		return *this;
	}

	//----<methond to pick up the specific data sets according to the provided value>-----
	template<typename T>Query<T>& Query<T>::select(const Conditions<T>& cond) {
		nosqldbq_ = namehelper(nosqldbq_, cond);
		nosqldbq_ = descriphelper(nosqldbq_, cond);
		nosqldbq_ = datetimehelper(nosqldbq_, cond);
		nosqldbq_ = childrenhelper(nosqldbq_, cond);
		//nosqldbq_ = payloadhelper(nosqldbq_, cond);
		return *this;
	}
	template<typename T>
	void showCond(const Conditions<T>& cond, std::ostream& out = std::cout)
	{
		out << "\n The query condition is: ";
		showHeader();
		out << "\n";
		cond.dateTime() += DateTime::makeDuration(0,5);
		if(cond.dateTime() < DateTime().now())out << std::setw(26) << std::left << std::string(cond.dateTime());
		if (cond.dateTime() > DateTime().now())out << std::setw(26) << std::left << "";
		out << std::setw(10) << std::left << cond.name();
		out << std::setw(25) << std::left << cond.descrip();
		out << std::setw(25) << std::left << cond.payLoad();
		typename Conditions<T>::Children children = cond.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}
}
