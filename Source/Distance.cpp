#include "..\Header\Distance.h"


// dist_arrayのセットアップ本体
void Distance::init_Distinations() {
	// 領域の確保
	
	Distinations = new uchar*[node_num - 1];
	for (size_t s = 0; s < node_num - 1; s++){
		try {
			Distinations[s] = new uchar[node_num - 1 - s];
		}
		catch (const std::bad_alloc& e) {
			cout << (node_num - 1 - s) << e.what() << endl;
			int l;
			cin >> l;
		}
	}

	// 初期化
	if (PathFileExists(str_to_char(bin_filename))) {
		// 既にファイル計算結果を保存済みなら読み込み
		cout << "n=" << dim << "の距離をファイルから読み込みます...";
		load_file();
		cout << "ok" << endl;
	}
	else {
		// 無ければ計算
		cout << "n=" << dim << "の距離を計算します\t" << getTime() << "...";
		calc_distination();
		cout << "ok" << endl;
		// ついでにファイルに保存しておく
		cout << "n=" << dim << "の距離をファイル保存します\t" << getTime() << "...";
		save_binary();
		cout << "ok" << endl;
	}
}

// 距離をファイルから読み込み
void Distance::load_file() {
	ifstream fin(bin_filename, ios::in | ios::binary);

	if (!fin) {
		printf_s("%sが開けません\n", bin_filename.c_str());
		return;
	}
	
	for (size_t s = 0; s < node_num - 1; ++s) {
		fin.read((char *)Distinations[s], sizeof(uchar) * (node_num - s - 1));
	}

	fin.close();  //ファイルを閉じる
}

// 距離を計算
void Distance::calc_distination() {
	for (size_t s = 0; s < node_num - 1; ++s) {
		for (size_t d = s + 1; d < node_num; ++d) {
			int tmp = SPR::GetMinimalExpansion(s, d, static_cast<int>(dim)).GetCount();
			Distinations[s][d - s - 1] = tmp;
		}
	}
}

// ファイルに保存
void Distance::save_binary() {
	ofstream fout(bin_filename, ios::out | ios::binary | ios::trunc);

	if (!fout) {
		printf_s("%sが開けません\n", bin_filename.c_str());
		return;
	}

	for (size_t s = 0; s < node_num - 1; ++s) {
		fout.write((char *)Distinations[s], sizeof(uchar) * (node_num - s - 1));
	}

	fout.close();
}

// コンストラクタ
Distance::Distance(size_t _dim) {
	dim = _dim;
	node_num = static_cast<int>(pow(2, dim));
	diameter = static_cast<int>(ceil((double)dim / 3) + 3);

	// フォルダ作成
	string dir = "DistBin\\";
	char *dir_c = str_to_char(dir);
	if (!PathIsDirectoryEmpty(dir_c)) {
		CreateDirectory(dir_c, NULL);
	}

	bin_filename = dir + to_string(dim) + ".bin";

	// dist_arrayを使えるようにする
	init_Distinations();
}

// デストラクタ
Distance::~Distance() {
	// length_arrayの解放
	if (Distinations != NULL) {
		for (size_t s = 0; s < node_num - 1; s++) {
			delete[] Distinations[s];
		}
		delete[] Distinations;
	}
}

// dist_arrayを標準出力
void Distance::show_dist_array() {
	for (size_t s = 0; s < node_num; ++s) {
		for (size_t d = 0; d < node_num; ++d) {
			cout << get_distance(s, d) << ' ';
		}
		cout << endl;
	}
}

// 2頂点間の距離を表から返す
size_t Distance::get_distance(size_t s, size_t d) {
	int dist;
	if (s > d) {
		return Distinations[d][s - d - 1];
	}
	else if (s < d){
		return Distinations[s][d - s - 1];
	}
	else {
		return 0;
	}
}
