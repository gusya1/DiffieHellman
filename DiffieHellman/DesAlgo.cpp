#include "DesAlgo.h"

#include <cinttypes>
#include <bitset>
#include <ranges>
#include <algorithm>

namespace {
	int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
				  62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
				  57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
				  61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

	int FP[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
				  38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
				  36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
				  34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9,  49, 17, 57, 25 };

	int PC_1[56] = { 57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18,
					10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
					63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
					14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4 };

	int PC_2[48] = { 14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10, 23, 19, 12, 4,
					26, 8,  16, 7,  27, 20, 13, 2,  41, 52, 31, 37, 47, 55, 30, 40,
					51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

	int shiftBits[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

	int E[48] = { 32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9,  10, 11,
				 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
				 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

	int S_BOX[8][4][16] = {
		{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		 {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		 {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		 {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
		{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		 {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		 {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		 {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
		{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		 {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		 {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		 {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
		{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		 {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		 {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		 {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
		{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		 {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		 {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		 {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
		{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		 {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		 {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		 {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
		{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		 {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		 {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		 {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
		{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		 {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		 {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		 {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}} };

	int P[32] =
	{
		16,  7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25
	};

	std::bitset<32> f(std::bitset<32> data, std::bitset<48> key)
	{
		// Расширение электронного блока
		auto expandR = std::bitset<48>{};
		for (int i = 0; i < 48; i++)
			expandR[47 - i] = data[32 - E[i]];
		// XOR
		expandR = expandR ^ key;
		// вместо этого S-поле
		auto output = std::bitset<32>{};
		int x = 0;
		for (int i = 0; i < 48; i = i + 6)
		{
			auto row = expandR[i] * 2 + expandR[i + 5];
			auto col = expandR[i + 1] * 8 + expandR[i + 2] * 4 + expandR[i + 3] * 2 + expandR[i + 4];
			auto num = S_BOX[i / 6][row][col];
			auto temp = std::bitset<4>(num);
			output[x + 3] = temp[0];
			output[x + 2] = temp[1];
			output[x + 1] = temp[2];
			output[x] = temp[3];
			x += 4;
		}
		// Замена P-бокса
		std::bitset<32> tmp = output;
		for (int i = 0; i < 32; i++)
			output[i] = tmp[P[i] - 1];

		return output;
	}

	// Функция сдвига влево
	std::bitset<28> leftshift(std::bitset<28> k, int shift) {
		return (k << shift) | (k >> 28 - shift);
	}

	void generateKeys(const std::bitset<64>& key, std::bitset<48> outKeys[16]) {
		auto real_key = std::bitset<56>{};
		auto left = std::bitset<28>{};
		auto right = std::bitset<28>{};
		auto compressKey = std::bitset<48>{};

		// Во-первых, после выбора и замены ПК-1 удаляем 8-битный бит четности исходного ключа
		// И переставляем
		for (int i = 0; i < 56; i++)
			real_key[i] = key[PC_1[i] - 1];

		for (int round = 0; round < 16; round++)
		{
			for (int i = 0; i < 28; i++)
				left[i] = real_key[i];
			for (int i = 28; i < 56; i++)
				right[i - 28] = real_key[i];
			// Сдвиг влево
			left = leftshift(left, shiftBits[round]);
			right = leftshift(right, shiftBits[round]);
			// Подключаем, заменяем и выбираем ПК-2 для перестановки и сжатия
			for (int i = 0; i < 28; i++)
				real_key[i] = left[i];
			for (int i = 28; i < 56; i++)
				real_key[i] = right[i - 28];
			for (int i = 0; i < 48; i++)
			{
				int m = PC_2[i];
				compressKey[i] = real_key[m - 1];
			}

			outKeys[round] = compressKey;
		}

	}

	std::bitset<64> charToBit(const char s[8]) {
		auto result = std::bitset<64>{};
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				result[i * 8 + j] = ((s[i] >> j) & 1);
		return result;
	}

	void bitToChar(std::bitset<64> data, char s[8]) {
		memset(s, 0, 8);
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				s[i] |= data[i * 8 + j] << j;
	}

	std::bitset<64> DES_crypt(const std::bitset<64>& plain, const std::bitset<48> subkeys[16], bool encrypt) {
		auto cipher = std::bitset<64>{};
		auto currentBits = std::bitset<64>{};
		auto left = std::bitset<32>{};
		auto right = std::bitset<32>{};
		auto newLeft = std::bitset<32>{};

		// Первоначальная замена IP
		for (int i = 0; i < 64; i++)
			currentBits[i] = plain[IP[i] - 1];

		for (int i = 0; i < 32; i++)
			left[i] = currentBits[i];
		for (int i = 32; i < 64; i++)
			right[i - 32] = currentBits[i];

		// Вводим 16 раундов изменения
		auto func = [&](auto round) {
			newLeft = right;
			right = left ^ f(right, subkeys[round]);
			left = newLeft;
		};
		if (encrypt)
			std::ranges::for_each(std::views::iota(0, 16), func);
		else
			std::ranges::for_each(std::views::iota(0, 16) | std::views::reverse, func);
		// Слияние
		for (int i = 0; i < 32; i++)
			cipher[i] = right[i];
		for (int i = 32; i < 64; i++)
			cipher[i] = left[i - 32];
		// Обратная инициализация замены
		currentBits = cipher;
		for (int i = 0; i < 64; i++)
			cipher[i] = currentBits[FP[i] - 1];

		return cipher;
	}

} // namespace

std::vector<char> DES::encrypt(const std::vector<char>& data, const char key[8])
{
	auto mod = data.size() % 8;
	auto result = std::vector<char>((mod == 0) ? data.size() : data.size() + 8 - mod, char(0));

	std::bitset<48> subkeys[16];
	generateKeys(charToBit(key), subkeys);

	for (auto i = 0; i < result.size(); i += 8)
		bitToChar(DES_crypt(charToBit(data.data() + i), subkeys, true), result.data() + i);

	return result;
}

std::vector<char> DES::decrypt(const std::vector<char>& data, const char key[8])
{
	auto mod = data.size() % 8;
	auto result = std::vector<char>((mod == 0) ? data.size() : data.size() + 8 - mod, char(0));

	std::bitset<48> subkeys[16];
	generateKeys(charToBit(key), subkeys);

	for (auto i = 0; i < result.size(); i += 8)
		bitToChar(DES_crypt(charToBit(data.data() + i), subkeys, false), result.data() + i);

	return result;
}