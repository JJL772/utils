/*
 * bhc-parse
 *
 * Simple parsing application for beckhoff terminals
 *
 */ 
#include <iostream>
#include <cstdio>
#include <fstream>
#include <tinyxml2.h>

/* Common includes */
#include "bhc-parse.h"
#include "../include/common/argparse.h"

/* Parses the specified file */
std::vector<BhcTerminal> parse_terminals(std::string file);

int main(int argc, char** argv)
{
	ArgParser parser(argc, argv);

	const char* file;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);

	if(parser.FindArgument("help"))
		printf("Cheese\n");

	auto terms = parse_terminals("/home/jeremy/Desktop/Projects/Utils/common/resources/Beckhoff EL1xxx.xml");

	for(auto x : terms)
	{
		printf("Terminal: %s\n", x.name.c_str());
	}

	return 0;
}

std::string GetValueIfValid(tinyxml2::XMLElement* elem)
{
	if(elem && elem->Value()) return std::string(elem->Value());
	return std::string("");
}

int GetIntValueIfValid(tinyxml2::XMLElement* elem)
{
	if(elem) return elem->IntText();
	return -1;
}

template<class...T>
void ForEachElement(tinyxml2::XMLElement* elem, const char* name, std::function<void(tinyxml2::XMLElement*,T...)> fn, T...args)
{
	if(!elem) return;
	for(auto _elem = elem->FirstChildElement(name); _elem; _elem = elem->NextSiblingElement(name))
		fn(_elem,args...);
}

std::vector<BhcTerminal> parse_terminals(std::string file)
{
	using namespace tinyxml2;

	std::vector<BhcTerminal> terminals;
	tinyxml2::XMLDocument doc;

	/* Load the file */
	doc.LoadFile(file.c_str());

	/* Try to grab the root element */
	XMLElement* root = doc.RootElement();
	if(!root) return terminals;

	/* Next, there's the descriptions */
	XMLElement* descriptions = root->FirstChildElement("Descriptions");
	if(!descriptions) return terminals;

	/* Next in the tree, we've got Devices */
	XMLElement* devices = descriptions->FirstChildElement("Devices");
	if(!devices) return terminals;

	/* Loop through all of the devices */
	for(XMLElement* device = devices->FirstChildElement("Device"); device; device = devices->NextSiblingElement("Device"))
	{
		BhcTerminal terminal;
		/* Try to grab the name */
		terminal.name = GetValueIfValid(device->FirstChildElement("Name"));
		/* Grab the type */
		terminal.type = GetValueIfValid(device->FirstChildElement("Type"));
		/* Go into the info tag */
		XMLElement* info = device->FirstChildElement("Info");
		XMLElement* twincat = nullptr, *vendorspecific = nullptr;
		if(info && (vendorspecific = info->FirstChildElement("VendorSpecific")) &&
				(twincat = vendorspecific->FirstChildElement("TwinCAT")))
		{
			/* Loop through the AlternativeSmMapping objects */
			for(auto mapping = twincat->FirstChildElement("AlternativeSmMapping"); mapping; mapping = twincat->NextSiblingElement("AlternativeSmMapping"))
			{
				/* This is a terrible mess */
				BhcPDOMapping _mapping;
				_mapping.bdefault = mapping->BoolAttribute("Default");
				_mapping.name = GetValueIfValid(device->FirstChildElement("Name"));
				/* Read all PDOs for this mapping */
				std::function<void(XMLElement*,std::vector<int>&)> fn = [](XMLElement* elem, std::vector<int>& pdos) {
					if(!elem) return;
					pdos.push_back(elem->IntText());
				};
				ForEachElement<std::vector<int>&>(mapping->FirstChildElement("Sm"), "Pdo", fn, _mapping.indicies);
				terminal.pdos.push_back(_mapping);
			}
		}

		/* Get the group type */
		std::string type = GetValueIfValid(device->FirstChildElement("GroupType"));
		if(type == "AnaIn") terminal.etype = BhcTerminal::AI;
		else if(type == "AnaOut") terminal.etype = BhcTerminal::AO;
		else if(type == "DigIn") terminal.etype = BhcTerminal::BI;
		else terminal.etype = BhcTerminal::BO;

		/* Read the profile */
		XMLElement* profile = device->FirstChildElement("Profile");
		if(!profile) continue; //return terminals;

		XMLElement* dictionary = profile->FirstChildElement("Dictionary");
		XMLElement* types = dictionary->FirstChildElement("DataTypes");
		
		/* Run this lambda on all of the types */
		std::function<void(XMLElement*,std::vector<BhcType>&)> fn = [](XMLElement* elem, std::vector<BhcType>&) {
			BhcType type;
			//type.basetype = GetValueIfValid(elem->FirstChildElement("BaseType"));
			type.bitsize = GetIntValueIfValid(elem->FirstChildElement("BitSize"));
			type.name = GetValueIfValid(elem->FirstChildElement("Name"));
			type.basetypename = GetValueIfValid(elem->FirstChildElement("BaseType"));
			/* What a friggin mess.... */
			std::function<void(XMLElement*,std::vector<BhcEnum>&)> fn2 = [](XMLElement* elem2, std::vector<BhcEnum>&) {
				BhcEnum _enum;
				_enum.text = GetValueIfValid(elem2->FirstChildElement("Text"));
				_enum.value = GetValueIfValid(elem2->FirstChildElement("Value"));
			};
			ForEachElement<std::vector<BhcEnum>&>(elem, "EnumInfo", fn2, type.enums);
			/* Another mess... */
			std::function<void(XMLElement*,std::vector<BhcSubItem>&)> fn3 = [](XMLElement* elem2, std::vector<BhcSubItem>&) {
				BhcSubItem item;
				item.name = GetValueIfValid(elem2->FirstChildElement("Name"));
				item.stype = GetValueIfValid(elem2->FirstChildElement("Type"));
				item.bitsize = GetIntValueIfValid(elem2->FirstChildElement("BitSize"));
				item.bitoff = GetIntValueIfValid(elem2->FirstChildElement("BitOffs"));
				/* Read flags and category */
				if(XMLElement* flags = elem2->FirstChildElement("Flags"))
				{
					std::string access = GetValueIfValid(flags->FirstChildElement("Access"));
					std::string category = GetValueIfValid(flags->FirstChildElement("Category"));
					if(access == "ro") item.flags = BhcSubItem::RO;
					else item.flags = BhcSubItem::RW;
					if(category == "T") item.category = BhcSubItem::IN;
					else if(category == "R") item.category = BhcSubItem::OUT;
				}
			};
			ForEachElement<std::vector<BhcSubItem>&>(elem, "EnumInfo", fn3, type.subitems);

		};
		terminals.push_back(terminal);
	}
}
