#include <iostream>

#include <stdio.h>

#include <stdint.h>

#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;



BITMAPFILEHEADER head;

BITMAPINFOHEADER info;

RGBQUAD pixel;

void hide_byte_into_pixel(RGBQUAD* pixel, uint8_t hide_byte) // сокрытие одного байта информации в одном пикселе

{
	//Убираем 2 младшиих бита FC = 11111100 за счет условия и
	pixel->rgbBlue &= (0xFC);
	//записываем 2 последних бита за счет условия или, 3 = 00000011
	pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
	//очищаем
	pixel->rgbGreen &= (0xFC);
	//записываем два незаписанных из hide_byte
	pixel->rgbGreen |= (hide_byte >> 4) & 0x3;

	pixel->rgbRed &= (0xFC);

	pixel->rgbRed |= (hide_byte >> 2) & 0x3;

	pixel->rgbReserved &= (0xFC);

	pixel->rgbReserved |= (hide_byte) & 0x3;

}

uint8_t decode(RGBQUAD pixel) {

	uint8_t hide;
	//считываем два последних байта
	hide = pixel.rgbBlue & 0x3;
	// сделали сдвиг для возможности сохранить след 2 байта
	hide = hide << 2;

	hide |= pixel.rgbGreen & 0x3;

	hide = hide << 2;

	hide |= pixel.rgbRed & 0x3;

	hide = hide << 2;

	hide |= pixel.rgbReserved & 0x3;

	return hide;

}

int main() {

	setlocale(LC_ALL, "Russian");



	bool flag = false;

	int choose = -1;

	do
	{
		cout << "Чтобы раскодировать информацию введите 1" << endl;
		cout << "Чтобы закодировать информацию введите 2" << endl;
		cout << "Чтобы выйти введите 0" << endl;
		cin >> choose;
	} while (choose <= -1 || choose >= 3);

	if (choose == 0)
	{
		return 0;
	}

	if (choose == 1)
	{
		flag = true;
	}
	
	if (choose == 2)
	{
		flag = false;
	}

	if (flag) {
		
		string filename;
		
		cout << "Введите имя файла для раскодировки" << endl;

		cin >> filename;

		FILE* picture = fopen(filename.c_str(), "rb");

		fread(&head, sizeof(head), 1, picture);

		fread(&info, sizeof(info), 1, picture);

		FILE* text = fopen("decode.txt", "wb");

		while (true) {

			fread(&pixel, sizeof(pixel), 1, picture);

			uint8_t byte = decode(pixel);

			if (byte == 0xFF)

				break;

			else

				fwrite(&byte, 1, 1, text);

		}

		fclose(picture);

		fclose(text);

	}

	else {

		FILE* picture = fopen("pict2.bmp", "rb");

		fread(&head, sizeof(head), 1, picture);

		fread(&info, sizeof(info), 1, picture);

		FILE* text = fopen("info_text.txt", "rb");

		FILE* newPicture = fopen("pict_new.bmp", "wb");

		fwrite(&head, sizeof(head), 1, newPicture);

		fwrite(&info, sizeof(info), 1, newPicture);

		while (true)
		{
			//считываем и прячем пока есть символы
			uint8_t letter;

			size_t x = fread(&letter, 1, 1, text);

			if (x != 1) letter = 0xFF;

			fread(&pixel, sizeof(pixel), 1, picture);

			hide_byte_into_pixel(&pixel, letter);

			fwrite(&pixel, sizeof(pixel), 1, newPicture);
			
			if (letter == 0xFF)
			{
				break;
			}

		}

		fclose(picture);

		fclose(text);

		fclose(newPicture);

	}

	return 0;

}