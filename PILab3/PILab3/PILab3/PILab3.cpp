#include <iostream>

#include <stdint.h>

#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;



BITMAPFILEHEADER head;

BITMAPINFOHEADER info;

RGBQUAD pixel;

void hide_byte_into_pixel(RGBQUAD* pixel, uint8_t hide_byte) // сокрытие одного байта информации в одном пикселе

{

	pixel->rgbBlue &= (0xFC);

	pixel->rgbBlue |= (hide_byte >> 6) & 0x3;

	pixel->rgbGreen &= (0xFC);

	pixel->rgbGreen |= (hide_byte >> 4) & 0x3;

	pixel->rgbRed &= (0xFC);

	pixel->rgbRed |= (hide_byte >> 2) & 0x3;

	pixel->rgbReserved &= (0xFC);

	pixel->rgbReserved |= (hide_byte) & 0x3;

}

uint8_t decode(RGBQUAD pixel) {

	uint8_t hide;

	hide = pixel.rgbBlue & 0x3;

	hide = hide << 2;

	hide |= pixel.rgbGreen & 0x3;

	hide = hide << 2;

	hide |= pixel.rgbRed & 0x3;

	hide = hide << 2;

	hide |= pixel.rgbReserved & 0x3;

	return hide;

}

int main() {

	FILE* picture = fopen("pict2.bmp", "rb");

	fread(&head, sizeof(head), 1, picture);

	fread(&info, sizeof(info), 1, picture);

	bool flag = true;

	if (flag) {

		FILE* text = fopen("decode.txt", "wb");

		while (true) {

			fread(&pixel, sizeof(pixel), 1, picture);

			uint8_t byte = decode(pixel);

			if (byte == 0xFF)

				break;

			else

				fwrite(&byte, 1, 1, text);

		}

		fclose(text);

	}

	else {

		FILE* text = fopen("info.txt", "rb");

		FILE* newPicture = fopen("pict2.bmp", "wb");

		fwrite(&head, sizeof(head), 1, newPicture);

		fwrite(&info, sizeof(info), 1, newPicture);

		int size = info.biWidth * info.biHeight;

		for (int i = 0; i < size; i++) {

			uint8_t letter;

			size_t x = fread(&letter, 1, 1, text);

			if (x != 1) letter = 0xFF;

			fread(&pixel, sizeof(pixel), 1, picture);

			hide_byte_into_pixel(&pixel, letter);

			fwrite(&pixel, sizeof(pixel), 1, newPicture);

		}

		fclose(text);

		fclose(newPicture);

	}

	fclose(picture);

	return 0;

}