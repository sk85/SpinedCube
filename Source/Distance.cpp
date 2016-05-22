#include "..\Header\Distance.h"


// 全体の初期化
void Distance::init_Distance_array() {
	// 領域の確保
	Distance_array = new uchar*[node_num - 1];
	for (size_t s = 0; s < node_num - 1; s++) {
		Distance_array[s] = new uchar[node_num - 1 - s];
	}

	// 初期化
	if (PathFileExists(str_to_char(file_path))) {
		// 既にファイル計算結果を保存済みなら読み込み
		cout << "n=" << dimension << "の距離をファイルから読み込みます...";
		read_bin();
		cout << "ok" << endl;
	}
	else {
		// 無ければ計算
		cout << "n=" << dimension << "の距離を計算します\t" << getTime() << "...";
		calc_distance();
		cout << "ok" << endl;
		// ついでにファイルに保存しておく
		cout << "n=" << dimension << "の距離をファイル保存します\t" << getTime() << "...";
		write_bin();
		cout << "ok" << endl;
	}
}

// 距離をファイルから読み込み
void Distance::read_bin() {
	ifstream fin(file_path, ios::in | ios::binary);

	if (!fin) {
		cout << file_path << "が開けません" << endl;
		return;
	}
	
	for (size_t s = 0; s < node_num - 1; ++s) {
		fin.read((char *)Distance_array[s], sizeof(uchar) * (node_num - s - 1));
	}

	fin.close();  //ファイルを閉じる
}

// 距離を計算
void Distance::calc_distance() {
	for (size_t s = 0; s < node_num - 1; ++s) {
		size_t row_size = node_num - s - 1;
		for (size_t d = 0; d < row_size; ++d) {
			uchar tmp = SPR::GetMinimalExpansion(s, d + s + 1, dimension).GetCount();
			Distance_array[s][d] = tmp;
		}
	}
}

// ファイルに保存
void Distance::write_bin() {
	ofstream fout(file_path, ios::out | ios::binary | ios::trunc);

	if (!fout) {
		cout << file_path << "が開けません" << endl;
		return;
	}

	for (size_t s = 0; s < node_num - 1; ++s) {
		fout.write((char *)Distance_array[s], sizeof(uchar) * (node_num - s - 1));
	}

	fout.close();
}

// コンストラクタ
Distance::Distance(size_t _dim) {
	dimension = _dim;
	node_num = 1 << dimension;
	diameter = static_cast<int>(ceil((double)dimension / 3) + 3);

	// フォルダ作成(無ければ)
	string dir = "DistBin\\";
	char *dir_c = str_to_char(dir);
	if (!PathIsDirectoryEmpty(dir_c)) {
		CreateDirectory(dir_c, NULL);
	}

	file_path = dir + to_string(dimension) + ".bin";

	// dist_arrayを使えるようにする
	init_Distance_array();
}

// デストラクタ
Distance::~Distance() {
	// length_arrayの解放
	if (Distance_array != NULL) {
		for (size_t s = 0; s < node_num - 1; s++)
		{
			delete[] Distance_array[s];
		}
		delete[] Distance_array;
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
uchar Distance::get_distance(size_t s, size_t d) {
	if (s > d) {
		return Distance_array[d][s - d - 1];
	}
	else if (s < d){
		return Distance_array[s][d - s - 1];
	}
	else {
		return 0;
	}
}
