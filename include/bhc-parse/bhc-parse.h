/*
 *
 * bhc-parse.h
 *
 * Common definitions for the XML device description parser
 *
 */ 
#pragma once

#include <string>
#include <vector>
#include <list>

/* An enum definition inside of a device description XML */
class BhcEnum
{
public:

};

class BhcPDO
{
public:
	
};

class BhcTerminal
{
public:
	int id; /* Numerical ID */
	std::string name; /* ASCII name */
	std::vector<BhcPDO> pdos; /* List of all PDOs for this terminal */
};
