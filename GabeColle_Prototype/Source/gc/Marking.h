#pragma once

#include<vector>


class Marking
{
public:
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
private:
	MarkingMap_t map_m;
public:
	Marking(int size);
	void mark(Address_t address);
	void unmark(Address_t address);
	bool isMarked(Address_t address)const;
};
