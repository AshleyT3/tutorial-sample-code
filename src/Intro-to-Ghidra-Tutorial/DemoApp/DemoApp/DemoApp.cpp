// DemoApp.cpp for use with the "Intro to Ghidra" tutorial: https://www.youtube.com/watch?v=OWEZQMVLMPs
// The purpose of this source file is to provide "known source" from which release/debug binaries can 
// be created for use in learning Ghidra. This source is not meant to convey C++ best-practices.

#include <iostream>
#include <string>

const uint8_t the_big_secret[] = {
	0xde, 0xc2, 0xcf, 0xf5, 0xc8, 0xc3, 0xcd, 0xf5,
	0xd9, 0xcf, 0xc9, 0xd8, 0xcf, 0xde, 0x00
};

const uint8_t the_big_code[] = {
	0xfe, 0xc2, 0xcf, 0x8a, 0xc9, 0xc5, 0xce, 0xcf,
	0x8a, 0xde, 0xc5, 0x8a, 0xcb, 0xc6, 0xc6, 0x8a,
	0xde, 0xc2, 0xc3, 0xc4, 0xcd, 0xd9, 0x90, 0x8a,
	0xec, 0xe6, 0xeb, 0xed, 0xd1, 0x9b, 0x98, 0x99,
	0x9e, 0x9f, 0x9c, 0x9d, 0x92, 0x93, 0xd7, 0x00
};

const char* get_the_code(const char* the_secret) {
	const uint8_t* pc = the_big_secret;
	while (*the_secret == (*pc ^ 0xaa))  the_secret++, pc++;
	if (*pc) return nullptr;
	static std::string buf((const char*)the_big_code);
	for (auto& ch : buf) ch ^= 0xaa;
	return buf.c_str();
}

int main()
{
	std::string username;
	std::cout << "Please, enter your full name: ";
	std::getline(std::cin, username);
	if (username != "le door de back") {
		std::cout << "Hello, " << username << "! I hope you have a great day!";
		return 0;
	}
	std::cout << "Very good...\n";
	std::string user_secret;
	std::cout << "Please, do tell me a secret, will you? ";
	std::getline(std::cin, user_secret);
	const char* the_code = get_the_code(user_secret.c_str());
	if (!the_code) {
		std::cout << "No code was found.\n";
		std::cout << "There are no more features in this program.\n";
		std::cout << "Thank you for stopping by, have a nice day now!\n";
		std::cout << "(c) The Brotherly Institution, Inc.\n";
		return -1;
	}
	std::cout << the_code << std::endl;
	std::cout << "You may now access the special features.\n";
	return 0;
}
