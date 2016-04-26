#pragma once

#include "Common.h"
#include "Node.h"
#include <iostream>

class Expansion {
private:
	Node Array[BIT_LENGTH];
	int Count;
public:
	// デフォルトコンストラクタ
	Expansion() {	// デフォルト
		Count = 0;
	}

	// コピーコンストラクタ
	Expansion(const Expansion &obj) {
		Count = obj.Count;
		for (int i = 0; i < Count; ++i)
			Array[i] = obj.Array[i];
	}

	// Nodeによるコンストラクタ
	Expansion(const Node &bin) {
		Count = 1;
		Array[0] = bin;
	}

	// Expansionがないとき用のコンストラクタ
	Expansion(int msg) {
		if (msg == 0) {
			Count = INF;
		}
		else {
			throw "NULL以外のint値を持つコンストラクタは存在しません";
		}
	}

	// 演算子のオーバーロード
	Expansion& operator=(const Expansion &obj);			// =
	 
	Expansion& operator+=(const Expansion &obj);		// += (Expansion + Expansionの場合)

	Expansion& operator+=(const Node &bin);				// += (Expansion + Nodeの場合)

	Expansion& operator+=(const ulong &bin);				// += (Expansion + ulongの場合)

	Expansion operator+(const Expansion &obj) const;	// + (Expansion + Expansionの場合)

	Expansion operator+(const Node &bin) const;			// + (Expansion + Nodeの場合)

	bool operator>(const Expansion &bin) const;			// + (Expansion + Nodeの場合)


	void Add(Node bin) {
		Array[Count++] = bin;
	}
	
	void Show() {
		if (Count == INF) {
			std::cout << "Expansionはありません" << std::endl;
			return;
		}
		for (int i = 0; i < Count; ++i) {
			std::cout << Array[i] << std::endl;
		}
	}

	bool Check(Node s, Node d) {
		ulong tmp = 0;
		for (int i = 0; i < Count; ++i) {
			tmp ^= Array[i].GetAddr();
		}
		return tmp != (s ^ d).GetAddr();
	}

	int GetCount() {
		return Count;
	}

	
};


// 代入演算子のオーバーロード
inline Expansion& Expansion::operator=(const Expansion &obj) {
	Count = obj.Count;
	for (int i = 0; i < BIT_LENGTH; ++i)
		Array[i] = obj.Array[i];
	return *this;
}

// += のオーバーロード(Expansion + Expansionの場合)
inline Expansion& Expansion::operator+=(const Expansion &obj) {
	for (int i = 0; i < obj.Count; ++i)
		Array[Count + i] = obj.Array[i];
	Count += obj.Count;
	return *this;
}

// += のオーバーロード(Expansion + Nodeの場合)
inline Expansion& Expansion::operator+=(const Node &bin) {
	Array[Count++] = bin;
	return *this;
}

// += のオーバーロード(Expansion + ulongの場合)
inline Expansion& Expansion::operator+=(const ulong &bin) {
	Array[Count++] = Node(bin);
	return *this;
}

// + のオーバーロード(Expansion + Expansionの場合)
inline Expansion Expansion::operator+(const Expansion &obj) const {
	Expansion exp = *this;
	for (int i = 0; i < obj.Count; ++i)
		exp.Array[Count + i] = obj.Array[i];
	exp.Count += obj.Count;
	return exp;
}

// + のオーバーロード(Expansion + Nodeの場合)
inline Expansion Expansion::operator+(const Node &bin) const {
	Expansion exp = *this;
	exp.Array[Count] = bin;
	exp.Count++;
	return exp;
}

// > のオーバーロード
inline bool Expansion::operator>(const Expansion &bin) const {
	return Count > bin.Count;
}