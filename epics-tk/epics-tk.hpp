/*
 *
 * epics-tk.h
 *
 * Common definitions for the toolkit, header-only library too
 *
 */ 
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

namespace epicstk {

/* Simple record class which will hold a list of PVs */
class Record
{
public:
	Record(std::string base, std::string name);

	struct PV
	{
		PV(std::string name, std::string value) { this->name = name; this->value = value; };
		std::string name;
		std::string value;
	};

	std::string record;
	std::string name;
	std::vector<PV> pvlist;

	/* Sets a field to some value */
	void SetField(std::string name, std::string value);

	/* Clears out a previously set field */
	void ClearField(std::string name);

	/* Write a simple DB file which will include any overridden values */
	void WriteDB(std::ofstream stream);

	/* Write a substitutions file for the fields overridden in this record */
	void WriteSubs(std::ofstream stream);
};

Record::Record(std::string _base, std::string _name) :
	name(_name),
	record(_base)
{
}

void Record::SetField(std::string name, std::string value)
{
	for(auto& x : this->pvlist)
	{
		if(x.name == name)
		{
			x.value = value;
			return;
		}
	}
	this->pvlist.push_back(Record::PV(name, value));
}

void Record::ClearField(std::string name)
{
	for(auto it = this->pvlist.begin(); it != this->pvlist.end(); it++)
	{
		if(it->name == name) 
		{
			this->pvlist.erase(it);
			return;
		}
	}
}

void Record::WriteDB(std::ofstream stream)
{
	/* Writes a DB file */
	stream << "\nrecord(" << this->record << ",";
	stream << "\"" << this->name << "\")\n{\n";
	/* write out our PVs */
	for(auto x : this->pvlist)
		stream << "\tfield(" << x.name << ",\"" << x.value << "\")\n";
	/* close it off */
	stream << "}" << std::endl; /* & flush it */
}

void Record::WriteSubs(std::ofstream stream)
{

}

}
