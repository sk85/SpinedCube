#include <Distance.h>

// dist_arrayのセットアップ本体
void Distance::init_dist_array() {
	// 領域の確保
	dist_array = new uchar*[node_num];
	for (size_t i = 0; i < node_num; i++)
	{
		dist_array[i] = new uchar[node_num];
	}

	// 初期化
	if (PathFileExists(str_to_wchar(bin_filename))) {
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
		cout << "n=" << dim << "の距離をファイル保存じます\t" << getTime() << "...";
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
	for (int s = 0; s < node_num; ++s) {
		for (int d = 0; d < node_num; ++d) {
			fin.read((char *)&dist_array[s][d], sizeof(uchar));
		}
	}
	fin.close();  //ファイルを閉じる
}

// 距離を計算
void Distance::calc_distination() {
	for (int s = 0; s < node_num; ++s) {
		for (int d = 0; d < node_num; ++d) {
			// 同じノード同士・同じノードの組は計算しない
			if (s <= d) {
				if (s == d) dist_array[s][d] = 0;
				continue;
			}

			int tmp = SPR::GetMinimalExpansion(s, d, static_cast<int>(dim)).GetCount();
			dist_array[s][d] = tmp;
			dist_array[d][s] = tmp;
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
	for (int s = 0; s < node_num; ++s) {
		for (int d = 0; d < node_num; ++d) {
			fout.write((char *)&dist_array[s][d], sizeof(uchar));
		}
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
	wchar_t *dir_w = str_to_wchar(dir);
	if (!PathIsDirectoryEmpty(dir_w)) {
		CreateDirectory(dir_w, NULL);
	}

	bin_filename = dir + to_string(dim) + ".bin";

	// dist_arrayを使えるようにする
	init_dist_array();
}

// デストラクタ
Distance::~Distance() {
	// length_arrayの解放
	if (dist_array != NULL) {
		for (size_t i = 0; i < node_num; i++)
		{
			delete[] dist_array[i];
		}
		delete[] dist_array;
	}
}

// dist_arrayを標準出力
void Distance::show_dist_array() {
	for (int s = 0; s < node_num; ++s) {
		for (int d = 0; d < node_num; ++d) {

			cout << static_cast<int>(dist_array[s][d]) << ' ';
		}
		cout << endl;
	}
}

// 2頂点間の距離を表から返す
uchar Distance::get_distance(size_t s, size_t d) {
	return dist_array[s][d];
}
