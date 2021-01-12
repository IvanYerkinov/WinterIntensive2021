#include <iostream>
#include "lexer.h"
#include "compiler.h"

using namespace std;

int main()
{
    Compiler::compilation cmp;
    auto a = Lexer::readTokenFile("test.txt");

    //Lexer::readTokenList(a);
    auto funlist = Lexer::parseTokens("test.txt");

    Lexer::readFuncList(funlist);

    cmp.Compile(funlist);
    cmp.Dump();
    return 0;
}
