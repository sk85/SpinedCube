#pragma once

#include "Common.h"
#include <iostream>

class Node {
private:
	ulong Addr;
public:
	// コンストラクタ
	Node();	// デフォルト

	Node(const Node &obj);	// コピー

	Node(ulong nodeAddr);	// addrを引数に取る場合

	// 演算子のオーバーロード
	Node &operator=(const Node &obj);	// =

	int operator[](int i);	// []

	Node operator^(Node &bin);	// ^ 

	// ^= 演算子のオーバーロード
	//// 右のオペランドがulongの場合
	Node &operator^=(Node obj);

	// == 演算子のオーバーロード
	//// 右のオペランドがulongの場合
	bool operator==(Node obj);

	// != 演算子のオーバーロード
	//// 右のオペランドがulongの場合
	bool operator!=(Node obj);

	// cout用<<演算子のオーバーロードのfriend宣言
	friend std::ostream& operator<<(std::ostream& os, const Node& obj);

	ulong Subsequence(int index, int length);

	// cの更新
	void Pop(ulong bin, int index);
	void Pop(Node &bin);

	
	// addrを取得
	ulong GetAddr() {
		return Addr;
	}

	// タイプを取得
	ulong GetType() {
		return Addr & 0b11;
	}

	// タイプ以外を取得
	ulong GetHead() {
		return Addr & (~0b11);
	}

	// 隣接頂点集合を取得
	Node* getNeighbor(size_t dim);

	Node* test(size_t dim);
};


inline Node Node::operator^(Node &bin) {
	return Addr ^ bin.Addr;
}

// 部分列を取得
inline ulong Node::Subsequence(int index, int length) {
	return (Addr >> (index - length + 1)) & ((1 << length) - 1);
}

// cの更新
inline void Node::Pop(ulong bin, int index) {
	ulong tmp = bin;
	int count = 0;
	while (tmp > 0) {
		tmp >>= 0b1;
		++count;
	}
	Addr ^= (bin << (index - count + 1));
}
inline void Node::Pop(Node &bin) {
	Addr ^= bin.Addr;
}