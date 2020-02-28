/*
 * argparse.h
 *
 * Simple argument parser
 *
 */ 
#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <list>

#include "util.h"

/* Argument parser class */
class ArgParser
{
public:
	struct Argument
	{
		Argument() : sval("") {};
		
		Argument(const Argument& other)
		{
			this->type = other.type;
			this->name = other.name;
			switch(this->type)
			{
				case BOOL: this->bval = other.bval; break;
				case INT: this->ival = other.ival; break;
				case FLOAT: this->fval = other.fval; break;
				default: this->sval = other.sval;
			}
		}

		~Argument() {};
		
		enum {
			STRING,
			BOOL,
			INT,
			FLOAT,
		} type;
		std::string name;
		union {
			std::string sval;
			bool bval;
			int ival;
			float fval;
		};
	};

	std::list<Argument> args;

public:
	ArgParser() : args(10) {};
	ArgParser(int argc, char** argv);

	~ArgParser();

	Argument* FindArgument(std::string arg);

	Argument* FindAny(std::initializer_list<std::string> arg);

	void RemoveArgument(std::string arg);

	Argument& operator[](std::string s);

	void Parse(int argc, char** argv);
};

ArgParser::ArgParser(int argc, char** argv)
{
	this->Parse(argc, argv);
}

ArgParser::~ArgParser()
{

}

ArgParser::Argument* ArgParser::FindAny(std::initializer_list<std::string> arg)
{
	for(auto x : arg)
	{
		Argument* argg = this->FindArgument(x);
		if(argg) return argg;
	}
	return nullptr;
}

void ArgParser::Parse(int argc, char** argv)
{
	for(int i = 0; i < argc; i++)
	{
		std::string str(argv[i]);
		if(str.length() > 2 && str[0] == '-' && str[1] == '-')
		{
			/* If we're a basic flag or have some value */
			if(size_t pos = str.find('=') != str.npos)
			{
				/* trailing = indicates maloformation, so we will not add the argument */
				if(pos == str.length()-1) continue;
				Argument arg;
				/* Detect the argument type */
				std::string value = str.substr(pos, str.length());
				arg.name = str.substr(2, pos);
				if(is_int(value)) { arg.type = Argument::INT; arg.ival = std::stoi(value); }
				else if(is_float(value)) { arg.type = Argument::FLOAT; arg.fval = std::stof(value); }
				else if(is_bool(value)) { arg.type = Argument::BOOL; arg.bval = to_bool(value); }
				else { arg.type =  Argument::STRING; arg.sval = str; }
				this->args.push_back(arg);
				continue;
			}
			else
			{
				Argument arg;
				arg.name = str.substr(2, str.length());
				arg.type = Argument::BOOL;
				arg.bval = true;
				this->args.push_back(arg);
				continue;
			}
		}
		/* Just a single flag */
		else if(str.length() > 1 && str[0] == '-')
		{
			Argument arg;
			arg.name = str.substr(1, str.length());
			arg.type = Argument::BOOL;
			arg.bval = true;
			this->args.push_back(arg);
			continue;
		}
		/* If we don't fit the bill, we will add it as an unnamed argument */
		Argument arg;
		arg.name = "";
		arg.type = Argument::STRING;
		arg.sval = str;
		this->args.push_back(arg);
	}
}

ArgParser::Argument* ArgParser::FindArgument(std::string arg)
{
	for(auto& x : this->args)
		if(x.name == arg) return &x;
	return nullptr;
}

void ArgParser::RemoveArgument(std::string arg)
{
	for(auto it = this->args.begin(); it != this->args.end(); it++)
		if(it->name == arg) args.erase(it);
}

ArgParser::Argument& ArgParser::operator[](std::string s)
{
	return *this->FindArgument(s);
}
