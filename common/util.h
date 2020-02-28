/*
 *
 * util.h
 *
 * Common utilities
 *
 */ 
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <list>

/* Forward declarations */
template<class T>
bool contains(std::vector<T>,const T&);
template<class T>
void enumerate(std::list<T> vec, std::function<void(T&)> fn);
template<class T>
void enumerate(std::vector<T> vec, std::function<void(T&)> fn);

bool is_float(std::string str)
{
	try {
		std::stof(str);
	}
	catch(...) {
		return false;
	}
	return true;
}

bool is_int(std::string str)
{
	try {
		std::stoi(str);
	}
	catch(...){
		return false;
	}
	return true;
}

bool is_bool(std::string str, std::vector<std::string> valid_vals = { "true", "false", "TRUE", "FALSE", "yes", "no", "YES", "NO"})
{
	for(auto s : valid_vals)
		if(str == s) return true;
	return false;
}

bool to_bool(std::string str, std::vector<std::string> true_vals = {"true", "TRUE", "YES", "ON", "yes", "on" }, std::vector<std::string> false_vals = {
		"false", "FALSE", "OFF", "off", "NO", "no"})
{
	if(contains(true_vals, str)) return true;
	return false;
}

/* Simple enumeration function, which will apply some function to a range of values */
template<class T>
void enumerate(std::vector<T> vec, std::function<void(T&)> fn)
{
	for(auto& x : vec)
		fn(x);
}

/* Same as the previous, but it runs on a list */
template<class T>
void enumerate(std::list<T> vec, std::function<void(T&)> fn)
{
	for(auto& x : vec)
		fn(x);
}

/* Checks for a value inside of a list of some sort */
template<class T>
bool contains(std::vector<T> vec, const T& val)
{
	for(auto x : vec)
		if(x == val) return true;
	return false;
}
