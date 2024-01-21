// For the Windows x64 stack and shadow store tutorial: https://www.youtube.com/watch?v=I8TL2BbKnbQ
#include <Windows.h>
#include <iostream>

using namespace std;

void f2(int a, int b, int c, int d, int e)
{
    int tot = a + b + c + d + e;
    cout << tot << endl;
}

void (* volatile pf2)(int a, int b, int c, int d, int e) = f2;

void f1(int a, int b, int c, int d, int e)
{

    // Do not call f2(...) directly because a Release build will optimize out the call.
    // f2(a, b, c, d, e);
    // Call through a volatile function pointer to prevent compiler from optimizing out call to pf2.
    (*pf2)(a, b, c, d, e);
}

int main()
{
    f1(1, 2, 3, 4, 5);
}
