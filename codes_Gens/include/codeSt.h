#pragma once
#include <string>
#include <set>
#include "symbols.h"
#include "debug.h"
#include "token.h"

using namespace std;

const string op_strs[] = {"+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!="};

class codeSt
{
public: //  enum类
    enum codeType
    {
        //读语句
        Scanf, //opeand1
        //写语句
        PrintStr,          //value
        PrintStrNoNewLine, //value
        PrintExp,          //operand1
        //常变量声明
        ConstVarState, //operand1
        //函数定义
        FunctState,              //operand1
        FunctRetWithValue,       //operand1
        FunctInlineRetWithValue, //operand1
        FunctRetWithoutValue,
        FunctInlineRetWithoutValue,
        //函数调用
        FunctArgsPush, //operand1
        FunctCall,     //operand1
        FunctRetUse,   //operand1
        //跳转
        BNZ,  //value
        BZ,   //value
        Jump, //value
        //Label
        Label, // value

        //伪三元
        AssignValue, //operand1,operand2

        //三元式
        AssignConst,   //operand1,value
        ArrayValueGet, //operand1,operand2,idx
        ArrayValuePut, //operand1,idx,operand2
        Condition,     //operand1,op,operand2
        Condition4Num, //operand1

        //四元式
        FourYuan, //operand1,op,operand2,result
    };

    enum op_em
    {
        op_UNKNOWNERROR = -1,
        op_PLUS = 17,
        op_MINU,
        op_MULT,
        op_DIV,
        op_LSS,
        op_LEQ,
        op_GRE,
        op_GEQ,
        op_EQL,
        op_NEQ,
    };

private: // 变量
    codeType codetype;
    string value_const_str;
    op_em op;
    bool inlineRet = false;
    codeSt();

public: //  变量
    int operand1;
    int operand2;
    int result;
    int idx;

public: //  构造器
    codeSt(codeType codetype);

    codeSt(codeType codetype, string value);
    codeSt(codeType codetype, int operand1);

    codeSt(codeType codetype, int operand1, int operand2);

    codeSt(codeType codetype, int operand1, string value);
    codeSt(codeType codetype, int operand1, int idx, int operand2);
    codeSt(codeType codetype, int operand1, op_em op, int operand2);

    codeSt(codeType codetype, int operand1, op_em op, int operand2, int result);

public: //  接口: OP类型的转化
    static op_em token_key2op_em(token_key key)
    {
        if (key <= 26 && key >= 17)
            return (op_em)(int)(key);
        return op_UNKNOWNERROR;
    }

public: //  接口: 内联函数相关
    void setInlineRet()
    {
        if (codetype == FunctRetWithValue)
            codetype = FunctInlineRetWithValue;
        if (codetype == FunctRetWithoutValue)
            codetype = FunctInlineRetWithoutValue;
    }
    bool isInlineRet() { return codetype == FunctInlineRetWithoutValue || codetype == FunctInlineRetWithValue; }

public: //  访问接口: 各个字段的值.
    string getValue() { return value_const_str; }
    symAttr *getOperand1() { return symbols::get_instance().get_pointer_by_id(operand1); }
    symAttr *getOperand2() { return symbols::get_instance().get_pointer_by_id(operand2); }
    symAttr *getResult() { return symbols::get_instance().get_pointer_by_id(result); }
    symAttr *getIdx() { return symbols::get_instance().get_pointer_by_id(idx); }
    op_em getOp() { return op; }
    string getOpStr()
    {
        if (op == op_UNKNOWNERROR)
        {
            return "UNKNOWN";
        }
        return op_strs[op - 17];
    }
    codeType getType() { return codetype; }
    string to_string();

public: //  访问接口: 基本块和流图
    set<int> getLeftValue();
    set<int> getRightValue();
    bool isFuncStart() { return codetype == FunctState; }
    bool isBlockStart() { return codetype == Label; }
    string getBlockEntry()
    {
        assert(isBlockStart());
        return value_const_str;
    }
    bool isBlockEnd() { return codetype == BNZ || codetype == BZ || codetype == Jump; }
    string getBlockDst()
    {
        assert(isBlockEnd());
        return value_const_str;
    }
    bool canGodown() { return codetype != Jump; }

public:
    bool changeLeftValue(int newValue);
};