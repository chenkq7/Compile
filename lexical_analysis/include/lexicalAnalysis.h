#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include <fstream>
#include <string>
#include <list>

#include "token.h"
#include "trieTree.h"
#include "debug.h"
#include "errorMags.h"
using namespace std;

class lexicalAnalysis
{
private:
    int line;
    ifstream in;
    trieTree reverseWord;
    list<token> symbolics;
    list<token>::iterator pivot;
    /*指向下一次get时应该return出去的元素.若指向end()表示需要解析了.*/
    token genSym();
    token_key checkReservedWord(string s);
    
    errorMags &errmag;

public:
    lexicalAnalysis(string filename);
    bool hasSym();
    token getSym();
    token peek();
    void unGetSym();
};
#endif