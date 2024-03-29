#include "token.h"
using namespace std;

token::token(token_key key, int line)
{
    assert(key >= RESERVED_BEGIN);
    this->key = key;
    this->value = TOKEN_VALUE[key];
    this->line = line;
}
token::token(token_key key, string value, int line)
{
    assert(key < VALUE_REQUIRED_END);
    this->key = key;
    this->value = value;
    this->line = line;
}
token_key token::getKey()
{
    return key;
}
string token::getName()
{
    return TOKEN_NAME[key];
}
string token::getValue()
{
    return value;
}

int token::getLine()
{
    return line;
}

bool isOpPrefix(char c)
{
    return OpPrefix.find(c) != OpPrefix.end();
}

bool isOpSecond(char c)
{
    return OpSecond.find(c) != OpSecond.end();
}

bool isSingle(char c)
{
    return Single.find(c) != Single.end();
}