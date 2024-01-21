// For the Windows MASM/assembly language tutorial: https://www.youtube.com/watch?v=I8TL2BbKnbQ
// SimpleMessageBox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>

int main()
{
	MessageBoxA(NULL, "Hello world, MessageBox!", "MessageBox Title from C++", MB_OK);
}
