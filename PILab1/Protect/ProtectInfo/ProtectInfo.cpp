#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <string>
using namespace std;


int getFileSize(const char* fileName)
{
	struct stat file_stat;
	stat(fileName, &file_stat);
	return file_stat.st_size;
}

string keyparser() {
	string keytxt;
	ifstream keyfile("key.txt", ios::in);
	if (!keyfile) {
		cout << "Cannot open file.\n"; return NULL;
	}
	getline(keyfile, keytxt);

	keyfile.close();
	return keytxt;
}

int coder(char* filename) {
	char** orig_content;
	struct stat stat_buff;
	stat(filename, &stat_buff);

	ifstream in(filename, ios::in | ios::binary);
	if (!in) {
		cout << "Cannot open file.\n"; return -1;
	}


	ofstream outres("test.doc", ios::out | ios::binary);
	if (!outres) {
		cout << "Cannot open file.\n";   return 1;
	}

	int file_size = stat_buff.st_size;
	int mod = file_size % 5;
	int size = file_size / 5;
	if (mod > 0)
		size++;
	orig_content = new char* [size];
	orig_content[0] = new char[5];

	char ch;
	int j = 0;
	int i = 0;

	in.seekg(ios::beg);
	while (in) {
		in.read(&ch, 1);
		if (in.fail())
		{
			if (!in.eof()) {
				cout << "Input error\n";
				break;
			}
			else break;

		}

		if (j == 5) {
			j = 0;
			i++;
			orig_content[i] = new char[5];
		}
		orig_content[i][j] = ch;
		j++;
	}

	if (mod > 0)
		for (int j = mod; j < 5; j++) {
			orig_content[size - 1][j] = 'z';
		}

	string key = keyparser(); //Читаем код

	int count = 1;
	while (count <= 5) {

		j = key.find(to_string(count));
		count++;
		for (i = 0; i < size; i++) {
			outres.put(orig_content[i][j]);
			//cout << (char)orig_content[i][j];
		}
	}
	in.close();
	outres.close();
}

int decoder(char* filename) {

	char** orig_content;
	struct stat stat_buff;
	stat(filename, &stat_buff);

	ifstream in(filename, ios::in | ios::binary);
	if (!in) {
		cout << "Cannot open file.\n"; return -1;
	}

	ofstream outres("test_dec.doc", ios::out | ios::binary);
	if (!outres) {
		cout << "Cannot open file.\n";   return 1;
	}

	int file_size = stat_buff.st_size;
	orig_content = new char* [file_size / 5];

	for (int i = 0; i < file_size / 5; i++)
		orig_content[i] = new char[5];

	char ch;
	int count = 1;
	string key = keyparser();
	in.seekg(ios::beg);
	while (count <= 5) {

		int j = key.find(to_string(count));
		count++;

		for (int i = 0; i < file_size / 5; i++) {
			in.read(&ch, 1);
			if (in.fail())
			{
				if (!in.eof()) {
					cout << "Input error\n";
					break;
				}
				else break;
			}
			orig_content[i][j] = ch;
		}
	}
	for (int j = 0; j < 5; j++) {
		if (orig_content[file_size / 5 - 1][j] == 'z')
			orig_content[file_size / 5 - 1][j] = NULL;

	}

	for (int i = 0; i < file_size / 5; i++) {
		for (int j = 0; j < 5; j++) {
			outres << orig_content[i][j];
		}
	}
	in.close();
	outres.close();
}


int main() {
	setlocale(LC_ALL, "Russian");
	int length = getFileSize("asd.doc");
	cout << "bytes - " << length;

	ifstream is("asd.doc", ifstream::binary);
	if (is) {
		char* buffer = new  char[length];

		is.read(buffer, length);

		int sum = 0;
		int f[256];
		float temp;
		for (int i = 0; i < 256; ++i) f[i] = 0;

		for (int i = 0; i < length; ++i) {
			++f[(unsigned char)buffer[i]];
		}
		cout << "\n";
		for (int i = 0; i < 256; ++i) {
			temp = float(f[i]);
			cout << i << "- " << temp * 100 / length << " % \n";
			sum += f[i];
		}
		cout << "control sum =" << sum << endl;
		delete[] buffer;
	}
	is.close();
	int choose;
	char filename[20];
	while (true)
	{

		//cout << (char)5;
		cout << "Для шифрования введите 1" << endl;
		cout << "Для расшифровки введите 2" << endl;
		cout << "Для выхода введите 0" << endl;
		cin >> choose;
		switch (choose)
		{
		case 1:
			cout << "Для шифровки введите имя файла" << endl;
			cin >> filename;
			coder(filename);
			break;
		case 2:
			cout << "Для расшифровки введите имя файла" << endl;
			cin >> filename;
			decoder(filename);
			break;
		case 0:
			return 0;
		default:
			break;
		}
	}
}