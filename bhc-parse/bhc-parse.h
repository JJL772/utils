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

struct BhcEnum
{
	std::string text;
	std::string value;
};

struct BhcArrayInfo
{
	int lbound, hbound;
	int elements;
};

struct BhcSubItem
{
	struct BhcType* type;
	int subindex;
	std::string name;
	std::string pdo_mapping;
	std::string stype;
	int bitsize, bitoff;
	/* FLags for this subitem */
	enum {
		FINVAL = 0,
		RO,
		RW,
		NA,
	} flags;
	/* The category of the thing */
	enum {
		CINVAL = 0,
		IN,
		OUT,
	} category;
};

struct BhcType
{
	std::string name;
	/* Bitsize of the type */
	int bitsize;
	/* Ptr to basetype, if any */
	BhcType* basetype;
	/* Name of the actual basetype */
	std::string basetypename;
	/* List of all enums */
	std::vector<BhcEnum> enums;
	/* List of all subitems */
	std::vector<BhcSubItem> subitems;
};

struct BhcObject 
{
	int index, subindex;
	std::string name;
	BhcType* type;
	int bitsize;
	enum {
		FINVAL = 0,
		RO,
		RW,
		NA,
	} flags;
};

struct BhcPDOEntry
{
	int index, subindex;
	int bitlen;
	std::string name;
	std::string comment;
	BhcType* type;
};

struct BhcPDO
{
	enum {
		TXPDO,
		RXPDO,
	} pdo_type;
	std::string name;
	int index;
	std::vector<int> excluded;
	std::vector<BhcPDOEntry> entries;
};

struct BhcPDOMapping
{
	bool bdefault;
	std::string name;
	std::vector<int> indicies;
	BhcPDO* pdo; /* This will initially be NULL, needs to be resolved on a second-pass */
};

class BhcTerminal
{
public:
	int id; /* Numerical ID */
	std::string name; /* ASCII name, usually with extra crap on it */
	std::string type; /* ASCII type, just the terminal name */
	std::vector<BhcPDOMapping> pdos;
	std::vector<BhcType> types;

	enum {
		AI = 0,
		AO,
		BI,
		BO,
		MOTOR,
		ENC,
	} etype;
};
