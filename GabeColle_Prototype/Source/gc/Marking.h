#pragma once

#include<vector>

namespace gc {


/**
*マークアンドスイープ法によるガベージコレクションでは,<br>
*1.ルートから参照を辿れるオブジェクトにマークを付ける<br>
*2.マークの付いていないオブジェクトを破棄する<br>
*という順番で不要なオブジェクトを破棄する<br>
*Markingクラスは番地とその先のオブジェクトにマークが付いているかどうかの関係を管理する<br>
*@see GarbageCollection
*/
class Marking
{
public:
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
private:
	MarkingMap_t map_m;
public:
	/**
	*コンストラクタ
	*@param size メモリの最大のオブジェクト数
	*/
	Marking(int size);
	
	/**
	*addressのオブジェクトにマークを付ける
	*@param address
	*/
	void mark(Address_t address);
	
	/**
	*addressのオブジェクトにマークを外す
	*@param address
	*/
	void unmark(Address_t address);

	/**
	*addressのオブジェクトにマークが付いているか調べる
	*@param address
	*@return オブジェクトにマークが付いている時真
	*/
	bool isMarked(Address_t address)const;
};

}