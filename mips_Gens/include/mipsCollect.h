#pragma once
#include "symbols.h"
#include "midcode.h"
#include <sstream>
#include <vector>
#include <string>
using namespace std;
class mipsCollect
{
private:
    mipsCollect() {}
    mipsCollect(const mipsCollect &) = delete;
    mipsCollect &operator&(mipsCollect &) = delete;
    vector<string> Register2Str;

public:
    static mipsCollect &get_instance()
    {
        static mipsCollect instance;

        instance.Register2Str.push_back("$zero");
        instance.Register2Str.push_back("$at");
        for (int i = 0; i <= 1; i++)
            instance.Register2Str.push_back("$v" + to_string(i));
        for (int i = 0; i <= 3; i++)
            instance.Register2Str.push_back("$a" + to_string(i));
        for (int i = 0; i <= 7; i++)
            instance.Register2Str.push_back("$t" + to_string(i));
        for (int i = 0; i <= 7; i++)
            instance.Register2Str.push_back("$s" + to_string(i));
        for (int i = 8; i <= 9; i++)
            instance.Register2Str.push_back("$t" + to_string(i));
        instance.Register2Str.push_back("$gp");
        instance.Register2Str.push_back("$sp");
        instance.Register2Str.push_back("$fp");
        instance.Register2Str.push_back("$ra");
        return instance;
    }
    enum Register
    {
        $ZERO,
        $at,
        $v0,
        $v1,
        $a0,
        $a1,
        $a2,
        $a3,
        $t0,
        $t1,
        $t2,
        $t3,
        $t4,
        $t5,
        $t6,
        $t7,
        $s0,
        $s1,
        $s2,
        $s3,
        $s4,
        $s5,
        $s6,
        $s7,
        $t8,
        $t9,
        $gp,
        $sp,
        $fp,
        $ra,
    };

    static Register getSeriesFP()
    {
        return Register::$fp;
    }
    static Register getSeriesGP()
    {
        return Register::$gp;
    }
    static Register getSeriesSP()
    {
        return Register::$sp;
    }
    static Register getSeriesRA()
    {
        return Register::$ra;
    }
    static Register getSeriesZero()
    {
        return Register::$ZERO;
    }
    static Register getSeriesS(int id)
    {
        assert(id >= 0 && id <= 7);
        return (Register)($s0 + id);
    }
    static Register getSeriesT(int id)
    {
        assert(id >= 0 && id <= 9);
        return (Register)($t0 + id);
    }
    static Register getSeriesA(int id)
    {
        assert(id >= 0 && id <= 3);
        return (Register)($a0 + id);
    }
    static Register getSeriesV(int id)
    {
        assert(id >= 0 && id <= 1);
        return (Register)($v0 + id);
    }

public:
    string get_mips_str() { return ss.str(); }

private:
    stringstream ss;
    bool textNow = true;
    void data();
    void text();

public:
    void comment(string comments);
    void asciiz(string name, string value); /*内含data(),text()*/
    void space(string name, int bytes);
    void word(string name, int num);

    void sw(Register, int, Register);
    void sw(Register, string);
    void sw(Register, string name, Register);
    void lw(Register, int, Register);
    void lw(Register, string name);
    void lw(Register, string name, Register);
    void la(Register, string);
    void la(Register, int, Register);
    void li(Register, int);

    void syscall(int);

    void labelLine(string label);

    void beqz(Register, string);
    void bnez(Register, string);
    void jump(string);

    void move(Register, Register);
    void add(Register, Register, int);
    void add(Register, Register, Register);
    void sub(Register, Register, Register);
    void mul(Register, Register, Register);
    void mul(Register, Register, int);
    void div(Register, Register, Register);
    void slt(Register, Register, Register);
    void sle(Register, Register, Register);
    void sgt(Register, Register, Register);
    void sge(Register, Register, Register);
    void seq(Register, Register, Register);
    void sne(Register, Register, Register);

    void jal(string FuncName);
    void jr(Register);
    void mflo(Register);
    void mfhi(Register);
};