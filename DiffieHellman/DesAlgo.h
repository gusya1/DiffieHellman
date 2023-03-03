#pragma once

#include <vector>


namespace DES
{
	std::vector<char> encrypt(const std::vector<char>& data, const char key[8]);
	std::vector<char> decrypt(const std::vector<char>& data, const char key[8]);
}
