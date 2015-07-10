#pragma once

# include <Siv3D.hpp>


class Memory;

class Object
{
	friend Memory;
	bool exists_m;
private:
	bool exists()const;
	void construct();
	void destruct();
};

class CircleObject : public Object
{
	double x_m;
	double y_m;
public:
	Vec2 center()const;
	void center(Vec2 v);
	void x(double x);
	void y(double y);
};

class Error
{
public:
	int segmentationFault_m = 0;
	int nullptrAccess_m = 0;
	int outOfMemory_m = 0;
	int addressOutOfBounds_m = 0;
};

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
	bool isUsing(Address_t from, Address_t to)const;

};


class MarkingMap
{
public:
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
private:
	MarkingMap_t map_m;
public:
	MarkingMap(int size);
	void mark(Address_t address);
	void unmark(Address_t address);
	bool isMarked(Address_t address)const;
};

class GarbageCollection
{
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
private:
	static void mark(Address_t address, MarkingMap &map, Memory const &memory);
public:
	static MarkingMap mark(Memory const &memory);
	static void sweep(MarkingMap const &map, Memory &memory);
	static void gc(Memory &memory);
};

class Memory
{
public:
	typedef int Address_t;
	typedef std::vector<CircleObject> Memory_t;
private:
	Memory_t memory_m;
	Relation relation_m;
	Error error_m;
private:
	void check(Address_t address);
public:
	Memory(int size);
	Address_t alloc();
	void free(Address_t address);
	void link(Address_t from, Address_t to);
	void unlink(Address_t from, int to);
	MarkingMap mark()const;
	void gc();
	bool hasExpired(Address_t address);
	CircleObject &access(Address_t address);
	Relation const &getRelation()const;
	int size()const;
	Error error()const;
	CircleObject &root();
};
