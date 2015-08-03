#pragma once


template<class DerivedObject>
class Memory;

class Object
{
	template<class DerivedObject>
	friend class Memory;

	bool exists_m;
private:
	bool exists()const;
	void construct();
	void destruct();
};
