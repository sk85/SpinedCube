#pragma once
#include "Expansion.h"
#include "Node.h"
#include "Common.h"

class SPR {
private:
	static Expansion SingleType(Node s, Node d, int n);
	static Expansion DoubleType_0001(Node s, Node d, int n);
	static Expansion DoubleType_0010(Node s, Node d, int n);
	static Expansion DoubleType_0111(Node s, Node d, int n);
	static Expansion DoubleType_1011(Node s, Node d, int n);
	static Expansion TripleType_000110(Node s, Node d, int n);
	static Expansion TripleType_000111(Node s, Node d, int n);
	static Expansion TripleType_001011(Node s, Node d, int n);
	static Expansion TripleType_011011(Node s, Node d, int n);
	static Expansion AllType(Node s, Node d, int n);

	static Node DoubleType_0111_Sub1
		(Node c, int index, Expansion expA, Expansion expB, Expansion *exp);
	static Node DoubleType_0111_Sub2
		(Node c, int index, Expansion expA, Expansion expB, Expansion expC, Expansion *exp);

	static Node DoubleType_1011_Sub1
		(Node c, int index, Expansion expA, Expansion expB, Expansion *exp);
	static Node DoubleType_1011_Sub2
		(Node c, int index, Expansion expA, Expansion expB, Expansion *exp);

	static Node TripleType_000111_Sub
		(Node c, int index, Expansion expA, Expansion expB, Expansion *exp);

	static Node TripleType_011011_Sub
		(Node c, int index, Expansion expA, Expansion expB, Expansion *exp);

	static Node GetBin(ulong bin, int index);

public:
	static Expansion GetMinimalExpansion(Node s, Node d, int n);
};

// ó◊ê⁄åàíËÉoÉCÉiÉäÇçÏÇÈ
inline Node SPR::GetBin(ulong type, int index) {
	return (0b100 + type) << (index - 2);
}