#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum symKind
{
    CONST,
    VAR,
    FUNC,
};

enum symType
{
    TYPERROR = -1,
    VOID,
    INT,
    CHAR,
    STRING,
    BOOL,
};

enum offsetRefer
{
    GLOBAL,
    FP,
};

class symAttr
{
public:
    string name;
    symType type;
    symKind kind;
    int len; /*为0表示不是数组, 大于0表示是数组*/

    int size; /*表示该单元在内存中所占空间大小,对函数而言就是sp需要移动的长度*/
    int SymId;
    int offsetRel;
    offsetRefer refer;
    string value;
    vector<symType> args;
    vector<int> argsId;
    int align = 4; /*TODO: 完善更多情况*/

    void addArgs(int id, symType arg);
    vector<symType> getArgs();

    symAttr(string name, symType type, symKind kind)
    {
        this->name = name;
        this->type = type;
        this->kind = kind;
        this->len = 0;
        this->size = 0;
        this->offsetRel = 0;
    }

    symAttr(string name, symType type, symKind kind, int len)
    {
        this->name = name;
        this->type = type;
        this->kind = kind;
        this->len = len;
        this->size = 0;
        this->offsetRel = 0;
    }
    void SHOW_ATTR();
};

class symbols
{
private:
    symbols();
    symbols(const symbols &) = delete;
    symbols &operator&(const symbols &) = delete;

public:
    static symbols &get_instance()
    {
        static symbols instance;
        return instance;
    }

private:
    vector<int> indexes;
    vector<int> idStack;
    vector<symAttr> id2sym;

public:
    void direct();
    void redirect();

    void insert(symAttr item);

    bool has(string name);
    bool hasNowSeg(string name);
    bool hasNearFunc();

    symAttr get(string name);
    symAttr getNowSeg(string name);
    symAttr getNearFunc();

    int get_id(string name);
    symAttr get_by_id(int id);
    symAttr *get_pointer_by_id(int id) { return &id2sym[id]; }

    void addArgsForNearFunc(string name, symType arg);

    void DEBUG_PRINT_LIST();
    void DEBUG_PRINT_ALL_SYM();
};

#endif