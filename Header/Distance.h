#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <direct.h>
#include <atltime.h>

#include "Common.h"
#include "SPR.h"

using namespace std;




class Distance {
private:
	size_t dim;
	size_t diameter;
	size_t node_num;

	uchar **dist_array;
	string bin_filename;

	/*******************************
		dist_arrayのセットアップ
	 *******************************/
	void init_dist_array();	// 本体

	void load_file();	// ファイルから読み込み

	void calc_distination();	// 距離を計算

	void save_binary();	// ファイルに保存


public:
	/***********************************
		コンストラクタ・デストラクタ
	************************************/
	Distance() {};

	Distance(size_t _dim);

	~Distance();


	/*************
		その他
	**************/
	// dist_arrayを標準出力
	void show_dist_array();

	// 2頂点間の距離を表から返す
	uchar get_distance(size_t s, size_t d);
};