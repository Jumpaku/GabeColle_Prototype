#pragma once

#include<vector>

namespace gc {

/**
*内部に関係行列を持っていてオブジェクの参照関係を管理する
*/
class Relation
{
public:
	typedef std::vector<std::vector<bool>> RelationMatrix_t;
	typedef int Address_t;
private:
	RelationMatrix_t relation_m;
public:
	/**
	*kコンストラクタ
	*@param size メモリの最大のオブジェクト数
	*/
	Relation(Address_t size);

	/**
	*番地fromのオブジェクトから番地toのオブジェクトへの参照をつくる
	*@param from
	*@param to
	*/
	void link(Address_t from, Address_t to);

	/**
	*番地fromのオブジェクトから番地toのオブジェクトへの参照をなくす
	*@param from
	*@param to
	*/
	void unlink(Address_t from, Address_t to);

	/**
	*番地fromのオブジェクトから番地toのオブジェクトへの参照があるか調べる
	*@param from
	*@param to
	*return fromからtoへの参照があれば真
	*/
	bool areLinked(Address_t from, Address_t to)const;
};

}