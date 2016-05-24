#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <atltime.h>
#include <direct.h>

#include "..\Header\Common.h"
#include "..\Header\Node.h"
#include "..\Header\Expansion.h"
#include "..\Header\SPR.h"
#include "..\Header\Distance.h"

using namespace std;

void test1();
void calc_length_ave1();
void calc_length_ave2();
size_t get_neighbor(size_t s, int index);

class Neighbor {
private:
	size_t dim;
	size_t node_num;
	size_t diameter;
	Distance dist;

	int** pref_neighbor_count;
	int** spare_neighbor_count;
	int** equal_neighbor_count;

	string filename_bin[3];
	string filename_csv[3];

	// 配列達をセットアップ
	void init_arrays() {
		// 領域の確保
		pref_neighbor_count = new int*[node_num];
		spare_neighbor_count = new int*[node_num];
		equal_neighbor_count = new int*[node_num];
		for (size_t i = 0; i < node_num; i++) {
			pref_neighbor_count[i] = new int[node_num];
			spare_neighbor_count[i] = new int[node_num];
			equal_neighbor_count[i] = new int[node_num];
		}

		// 初期化
		if (PathFileExists(str_to_char(filename_bin[0]))) {
			// 既にファイル計算結果を保存済みなら読み込み
			cout << "n=" << dim << "の隣接情報をファイルから読み込みます...";
			load_file();
			cout << "ok" << endl;
		}
		else {
			// 無ければ計算
			cout << "n=" << dim << "の隣接情報を計算します\t" << getTime() << "...";
			calc_neighbors();
			cout << "ok" << endl;
			// ついでにファイルに保存しておく
			cout << "n=" << dim << "の隣接情報をファイル保存じます\t" << getTime() << "...";
			save_binary();
			cout << "ok" << endl;
		}
	}

	void calc_neighbors() {
		// 0で初期化
		for (size_t s = 0; s < node_num; s++) {
			for (size_t d = 0; d < node_num; d++) {
				pref_neighbor_count[s][d] = 0;
				spare_neighbor_count[s][d] = 0;
				equal_neighbor_count[s][d] = 0;
			}
		}

		// 計算
		for (size_t s = 0; s < node_num; s++) {
			Node* neighbor_set = Node(static_cast<ulong>(s)).getNeighbor(dim);

			for (size_t d = 0; d < node_num; d++) {
				for (size_t i = 0; i < dim; i++) {
					ulong neighbor = neighbor_set[i].GetAddr();

					if (dist.get_distance(s, d) > dist.get_distance(s, d)) {
						pref_neighbor_count[s][d]++;
					}
					else if (dist.get_distance(s, d) == dist.get_distance(s, d)) {
						spare_neighbor_count[s][d]++;
					}
					else {
						equal_neighbor_count[s][d]++;
					}
				}
			}
			delete[] neighbor_set;
		}
	}

	void load_file() {
		// TODO
	}

	void save_binary() {
		// TODO
	}



public:
	Neighbor(size_t _dim) {
		dim = _dim;
		dist = Distance(dim);
		node_num = static_cast<size_t>(pow(2, dim));
		diameter = static_cast<size_t>(ceil((double)dim / 3) + 3);

		// フォルダ作成
		string dir_bin = "NeighborBin\\";
		char *dir_w = str_to_char(dir_bin);
		if (!PathIsDirectoryEmpty(dir_w)) {
			CreateDirectory(dir_w, NULL);
		}
		string dir_csv = "NeighborCsv\\";
		dir_w = str_to_char(dir_csv);
		if (!PathIsDirectoryEmpty(dir_w)) {
			CreateDirectory(dir_w, NULL);
		}
		free(dir_w);

		filename_bin[0] = dir_bin + "pref" + to_string(dim) + ".bin";
		filename_bin[1] = dir_bin + "equal" + to_string(dim) + ".bin";
		filename_bin[2] = dir_bin + "spare" + to_string(dim) + ".bin";
		filename_csv[0] = dir_csv + "pref" + to_string(dim) + ".csv";
		filename_csv[1] = dir_csv + "equal" + to_string(dim) + ".csv";
		filename_csv[2] = dir_csv + "spare" + to_string(dim) + ".csv";

		void init_arrays();


	}

	// デバッグ用。配列が正しいかチェック
	void check_neighbor_array() {
		for (size_t s = 0; s < node_num; s++) {
			for (size_t d = 0; d < node_num; d++) {
				int tmp =
					pref_neighbor_count[s][d] +
					spare_neighbor_count[s][d] +
					equal_neighbor_count[s][d];
				if (tmp != dim) {
					cout << "!";
				}
			}
		}
	}

	// 0でpref、1でequal、2でspareを指定できる
	int** choose_array(int flag) {
		int** ary;
		switch (flag) {
		case 0:
			ary = pref_neighbor_count;
			break;
		case 1:
			ary = equal_neighbor_count;
			break;
		case 2:
			ary = spare_neighbor_count;
			break;
		}
		return ary;
	}

	// 配列を標準出力
	void show_neibor_array(int flag) {
		int** ary = choose_array(flag);

		for (size_t s = 0; s < node_num; s++){
			for (size_t d = 0; d < node_num; d++){
				cout << ary[s][d] << " ";
			}
			cout << endl;
		}
	}

	// 配列をcsv形式で保存
	void save_csv(int flag) {
		string filename = filename_csv[flag];
		int** ary = choose_array(flag);

		ofstream fout(filename, ios::out);

		if (!fout) {
			printf_s("%sが開けません\n", filename.c_str());
			return;
		}
		for (size_t s = 0; s < node_num; ++s) {
			for (size_t d = 0; d < diameter + 1; ++d) {
				fout << ary[s][d] << ",";
			}
			fout << endl;
		}
		fout.close();
	}
};

int main() {

	Expansion exp = SPR::GetMinimalExpansion(0b01, 311, 13);
	exp.Show();
	int i;
	cin >> i;
	/*
	Distance d(13);
	size_t dis;
	size_t nei = get_neighbor(0, 8);
	size_t n_dis = d.get_distance(nei, 311);
	cout << Node(311) << ' ' << (size_t)d.get_distance(0, 311) << endl;
	cout << Node(nei) << ' ' << (size_t)d.get_distance(nei, 311) << endl;
	cin >> dis;*/


	// 実験の範囲を定める
	size_t min_dim = 13;
	size_t max_dim = 13;

	// 表の大きさを定める
	size_t ary_coulmn = 4;
	size_t ary_row = 14;

	for (size_t dim = min_dim; dim <= max_dim; dim++)
	{
		// もろもろ初期化
		Distance dist(dim);
		size_t node_num = 1 << dim;

		// 表の宣言・領域確保・初期化
		uchar **ary = new uchar*[ary_coulmn];
		for (size_t i = 0; i < ary_coulmn; i++)
		{
			ary[i] = new uchar[ary_row];
			for (size_t j = 0; j < ary_row; j++)
			{
				ary[i][j] = 0;
			}
		}

		// 計算とか
		for (size_t d = 0; d < node_num; d++)
		{
			size_t distance = dist.get_distance(0, d);
			size_t pref_count = 0;
			for (size_t i = 0; i < dim; i++)
			{
				size_t n_distance = dist.get_distance(get_neighbor(0, i), d);
				if (n_distance < distance) {
					pref_count++;
					
				}
			}
			if (pref_count == 0) {
				cout << d << "," << distance << endl;
			}
			ary[d & 0b11][pref_count]++;
		}

		// csvに保存
		string filename = "csv_new\\" + to_string(dim) + ".csv";
		ofstream fout(filename, ios::out | ios::trunc);
		if (!fout) {
			cout << filename << "が開けません" << endl;
			return -1;
		}
		for (size_t i = 0; i < ary_coulmn; i++)
		{
			for (size_t j = 0; j < ary_row; j++)
			{
				fout << (unsigned int)ary[i][j] << ',';
			}
			fout << endl;
		}
		fout.close();

		// 表の後始末
		for (size_t i = 0; i < ary_coulmn; i++)
		{
			delete[] ary[i];
		}
		delete[] ary;
	}
	



#ifdef TEST
	for (size_t dim = 2; dim <= max_dim; dim++)
	{
		Distance len(dim);
		size_t node_num = 1 << dim;
		
		// 領域を確保
		uchar** c_array = new uchar*[node_num];
		for (size_t i = 0; i < node_num; i++)
		{
			c_array[i] = new uchar[max_distance];
			for (size_t j = 0; j < max_distance; j++)
			{
				c_array[i][j] = 0;
			}
		}

		// 前方隣接頂点数を計算
		for (size_t s = 0; s < node_num; s++)
		{
			for (size_t d = 0; d < node_num; d++)
			{
				int count_pref = 0;
				size_t distance = len.get_distance(s, d);
				for (size_t i = 0; i < dim; i++)
				{
					size_t n_distance = len.get_distance(d, get_neighbor(s, i));
					if (n_distance < distance) {
						count_pref++;
					}
				}
				c_array[s][distance] += count_pref;
			}
		}

		// csvに保存
		string filename = "csv_new\\" + to_string(dim) + ".csv";
		ofstream fout(filename, ios::out | ios::trunc);
		if (!fout) {
			cout << filename << "が開けません" << endl;
			int i;
			cin >> i;
			return -1;
		}
		for (size_t s = 0; s < node_num; s++)
		{
			for (size_t d = 0; d < max_distance; d++)
			{
				fout << (unsigned int)c_array[s][d] << ',';
				//fout << (unsigned int)len.get_distance(s, d) << ',';
			}
			fout << endl;
		}
		fout.close();

		// 後処理
		for (size_t i = 0; i < node_num; i++)
		{
			delete[] c_array[i];
		}
		delete[] c_array;
	}
#endif
	cout << "fin" << endl;
	cin >> max_dim;
}

// SQの隣接頂点を返す
size_t get_neighbor(size_t s, int index) {
	size_t d;
	if (index > 3) {
		size_t type = s & 0b11;
		d =  (0b100 | type) << (index - 2);
	}
	else if (index > 1){
		size_t type = s & 1;
		d = (0b10 | type) << (index - 1);
	}
	else {
		d = 1 << index;
	}
	return d ^ s;
}



// string をLPWSTR ＝ WCHAR*に変換
wchar_t* str_to_wchar(string str) {
	size_t length = str.length() + 1;
	wchar_t* wch = (wchar_t *)malloc(length * sizeof(wchar_t));
	for (size_t i = 0; i < length; i++)
		wch[i] = str[i];

	return wch;
}

// string をLPTSTR ＝ TCHAR*に変換
char* str_to_char(string str) {
	size_t length = str.length() + 1;
	char* ch = (char *)malloc(length * sizeof(char));
	for (size_t i = 0; i < length; i++)
		ch[i] = str[i];
	return ch;
}

//現在日時を取得
string getTime() {
	SYSTEMTIME t;
	GetLocalTime(&t);

	return to_string(t.wMonth) + "/" + to_string(t.wDay) + " " + to_string(t.wHour) + ":" + to_string(t.wMinute) + ":" + to_string(t.wSecond);
}



// 提案手法を用いて平均距離を計算
///// 同じ頂点はやらないので2^n(2^n - 1)通り
void test0(size_t max_dim) {
	std::ofstream ofs("test.txt");

	for (size_t dim = 2; dim <= max_dim; dim++)
	{
		cout << "n = " << dim << endl;
		ofs << "n = " << dim << endl;
		cout << getTime() << endl;

		double node_num = pow(2, dim);
		double combination_count = node_num * (node_num - 1) / 2;
		double distance = 0;
		size_t diam = 0;
		for (size_t s = 0; s < node_num; s++)
		{
			for (size_t d = s + 1; d < node_num; d++)
			{
				int tmp = SPR::GetMinimalExpansion(s, d, static_cast<int>(dim)).GetCount();
				if (tmp > diam) diam = tmp;
				distance += tmp / combination_count;
			}
		}
		cout << "直径：" << diam << endl;
		cout << "経路：" << distance << endl;
		cout << endl;

		ofs << "直径：" << diam << endl;
		ofs << "経路：" << distance << endl;
		ofs << endl;
	}

	ofs.close();
}



// ある頂点xから他の頂点までの距離の和をD(x)とすると、
// xとタイプの等しい任意の頂点yについてD(x)=D(y)である
//// n = 13まで実験済
void test1() {
	for (int dim = 2; dim < 30; ++dim) {
		double node_num = pow(2, dim);
		double ary[4] = { 0, 0, 0, 0 };
		cout << "n=" << dim << endl;
		for (int s = 0; s < node_num; ++s) {
			double length_sum = 0;
			for (int d = 0; d < node_num; ++d) {
				if (s == d) continue;
				int tmp = SPR::GetMinimalExpansion(s, d, dim).GetCount();
				length_sum += tmp;
			}
			if (ary[s % 4] != length_sum) {
				if (ary[s % 4] == 0) {
					ary[s % 4] = length_sum;
				}
				else {
					cout << s << endl;
				}
			}
		}
	}
	cout << "finish";
}



// 前方隣接頂点数の計算と保存
void pref_neighbor(size_t max_dim) {
	for (size_t dim = 2; dim <= max_dim; dim++)
	{
		Distance len(dim);
		size_t node_num = 1 << dim;
		size_t diameter = ceil(static_cast<double>(dim) / 3) + 3;

		// 前方隣接頂点数を保存する領域を確保
		uchar** pref = new uchar*[node_num];
		for (size_t i = 0; i < node_num; i++)
		{
			pref[i] = new uchar[node_num];
		}

		// 前方隣接頂点数を計算
		for (size_t s = 0; s < node_num; s++)
		{
			for (size_t d = 0; d < node_num; d++)
			{
				pref[s][d] = 0;
				size_t distance = len.get_distance(s, d);
				for (size_t i = 0; i < dim; i++)
				{
					size_t n = len.get_distance(d, get_neighbor(s, i));
					if (n < distance) {
						pref[s][d]++;
					}
				}
			}
		}

		// csvに保存
		string filename = "csv_new\\" + to_string(dim) + ".csv";
		ofstream fout(filename, ios::out | ios::trunc);
		if (!fout) {
			cout << filename << "が開けません" << endl;
			int i;
			cin >> i;
			return;
		}
		for (size_t s = 0; s < node_num; s++)
		{
			for (size_t d = 0; d < node_num; d++)
			{
				fout << (unsigned int)pref[s][d] << ',';
				//fout << (unsigned int)len.get_distance(s, d) << ',';
			}
			fout << endl;
		}
		fout.close();

		// 後処理
		for (size_t i = 0; i < node_num; i++)
		{
			delete[] pref[i];
		}
		delete[] pref;
	}
}

// 平均距離の計算（全探索版）
void calc_length_ave1() {
	ofstream ofs("result.csv");
	ofs << "次元,直径,平均距離,00→00,00→01,00→10,00→11,01→00,01→01,01→10,01→11,10→00,10→01,10→10,10→11,11→00,11→01,11→10,11→11" << endl;
	for (int dim = 1; dim < 31; ++dim) {
		double averageLength = 0;	// 平均経路長
		double lengthArray[4][4] = {	// 個別の平均経路長
			{ 0, 0, 0, 0, },
			{ 0, 0, 0, 0, }
		};

		int maxLength = 0;			// 直径
		double nodeNumber = pow(2, dim);	// ノード数
		double combinationCount = (nodeNumber * (nodeNumber - 1)) / 2;	// 組み合わせの数

																		// 開始時刻を表示
		cout << getTime();
		// 計測スタート時刻を保存
		auto start = chrono::system_clock::now();

		for (int s = 0; s < nodeNumber; ++s) {
			for (int d = s; d < nodeNumber; ++d) {
				// 同じノード同士は考えない
				if (s == d) continue;

				// 距離を計算
				int tmp = SPR::GetMinimalExpansion(s, d, dim).GetCount();
				double delta = tmp / combinationCount;

				// 平均経路長を加算
				averageLength += delta;
				lengthArray[s & 0b11][d & 0b11] += delta;

				// 距離が大きければ直径を更新
				if (maxLength < tmp) maxLength = tmp;
			}
		}

		// 計測終了時刻を保存
		auto end = chrono::system_clock::now();
		// 要した時間を計算
		auto dur = end - start;
		// 要した時間をミリ秒（1/1000秒）に変換
		auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();

		cout << " n=" << dim;
		cout << "\t平均距離:" << setprecision(5) << averageLength;
		cout << "\t直径:" << maxLength;
		cout << "\t実行時間:" << msec << "ms" << endl;

		ofs << dim << "," << maxLength << "," << averageLength;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				ofs << "," << lengthArray[i][j];
			}
		}
		ofs << endl;
	}
}

// 平均距離の計算（効率化版）
void calc_length_ave2() {
	for (int dim = 2; dim < 20; ++dim) {
		double node_num = pow(2, dim);
		double length = 0;

		for (int d = 0; d < node_num; ++d) {
			for (int i = 0; i < 4; ++i) {
				length += SPR::GetMinimalExpansion(i, d, dim).GetCount();
			}
		}

		length = length / (pow(2, dim + 2) - 4);

		cout << "n=" << dim << "\t平均距離:" << length << endl;
	}
}