#pragma once

#include<vector>

namespace gc {

class Relation
{
public:
	typedef std::vector<std::vector<bool>> RelationMatrix_t;
	typedef int Address_t;
private:
	RelationMatrix_t relation_m;
public:
	Relation(Address_t size);
	void link(Address_t from, Address_t to);
	void unlink(Address_t from, Address_t to);
	bool areLinked(Address_t from, Address_t to)const;
};

}