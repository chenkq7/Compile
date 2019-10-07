#include "syntacticAnalysis.h"

syntacticAnalysis::syntacticAnalysis(string filename, string outfile) : lexical(filename), sym(lexical.getSym()), out(outfile)
{
}

void syntacticAnalysis::printToken(token key)
{
    out << key.getName << " " << key.getValue << endl;
}

void syntacticAnalysis::printLine(string s)
{
    out << s << endl;
}

bool syntacticAnalysis::isTypeIdentifier(token key)
{
    return typeIdentifier.find(key.getKey()) != typeIdentifier.end();
}

bool syntacticAnalysis::isFuncWithRet(token key)
{
    return funcWithRet.find(key.getValue()) != funcWithRet.end();
}

bool syntacticAnalysis::isFuncWithoutRet(token key)
{
    return funcWithoutRet.find(key.getValue()) != funcWithoutRet.end();
}

bool syntacticAnalysis::isAddOp(token key)
{
    return addOp.find(key.getKey()) != addOp.end();
}

bool syntacticAnalysis::isMulOp(token key)
{
    return mulOp.find(key.getKey()) != mulOp.end();
}

bool syntacticAnalysis::isRelOp(token key)
{
    return relOp.find(key.getKey()) != relOp.end();
}

bool isCharacter(token key)
{
    return chars.find(key.getKey()) != chars.end();
}

void syntacticAnalysis::procedureCheck()
{
    if (sym.getKey() == CONSTTK)
    {
        sym = lexical.getSym();
        constState();
    }
    if (isTypeIdentifier(sym))
    {
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);

        sym = lexical.getSym();
        if (sym.getKey() == ASSIGN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            variableState();
        }
        else if (sym.getKey() == LPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            funcWithReturn();
        }
        else
        {
            assert(false); /*程序到达此处说明有BUG*/
        }
    }

    while (!(sym.getKey() == VOIDTK && lexical.peek().getKey() == MAINTK))
    {
        if (sym.getKey() == VOIDTK)
        {
            funcWithoutReturn();
        }
        else if (isTypeIdentifier(sym))
        {
            funcWithReturn();
        }
        else
        {
            assert(false); /*程序到达此处说明有BUG*/
        }
    }
    mainFunc();
    printLine("<程序>");
    //assert(!lexical.hasSym()); /*main 函数后不应该有其他token*/
}

void syntacticAnalysis::strConCheck()
{
    assert(sym.getKey() == STRCON);
    printToken(sym);
    sym = lexical.getSym();
    printLine("<字符串>");
}

void syntacticAnalysis::constState()
{
    while (sym.getKey() == CONSTTK)
    {
        printToken(sym);
        sym = lexical.getSym();
        constDefine();
        assert(sym.getKey() == SEMICN);
        printToken(sym);
        sym = lexical.getSym();
    }
    printLine("<常量说明>");
}

void syntacticAnalysis::constDefine()
{
    if (sym.getKey() == INTTK)
    {
        do
        {
            printToken(sym);

            sym = lexical.getSym();
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            sym = lexical.getSym();
            assert(sym.getKey() == ASSIGN);
            printToken(sym);

            sym = lexical.getSym();
            integer();
        } while (sym.getKey() == COMMA);
    }
    else if (sym.getKey() == CHARTK)
    {
        do
        {
            printToken(sym);

            sym = lexical.getSym();
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            sym = lexical.getSym();
            assert(sym.getKey() == ASSIGN);
            printToken(sym);

            sym = lexical.getSym();
            character();
        } while (sym.getKey() == COMMA);
    }
    else
    {
        assert(false); /*得到的类型标识符不是int,char*/
    }
    printLine("<常量定义>");
}

void syntacticAnalysis::unsignedInteger()
{
    assert(sym.getKey() == INTCON);
    printToken(sym);

    sym = lexical.getSym();
    printLine("无符号整数");
}

void syntacticAnalysis::integer()
{
    if (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
    }
    printToken(sym);
    unsignedInteger();
    printLine("<整数>");
}

void syntacticAnalysis::character()
{
    assert(isCharacter(sym));
    printToken(sym);
    sym = lexical.getSym();
}

string syntacticAnalysis::stateHead()
{
    assert(isTypeIdentifier(sym));
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    string name = sym.getValue();
    printToken(sym);

    sym = lexical.getSym();
    printLine("<声明头部>");
    return name;
}

void syntacticAnalysis::variableState()
{
    do
    {
        variableDefine();
        assert(sym.getKey() == SEMICN);
        printToken(sym);

        sym = lexical.getSym();
        if (!isTypeIdentifier(sym))
        {
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym();
        if (sym.getKey() != IDENFR)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym();
        if (!(sym.getKey() == LBRACK || sym.getKey() == SEMICN || sym.getKey() == COMMA))
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            printLine("<变量说明>");
            return;
        }
        lexical.unGetSym();
        lexical.unGetSym();
        lexical.unGetSym();
    } while (true);
}

void syntacticAnalysis::variableDefine()
{
    assert(isTypeIdentifier(sym));
    do
    {
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        sym = lexical.getSym();
        if (sym.getKey() == LBRACK)
        {
            printToken(sym);

            unsignedInteger();

            assert(sym.getKey() == RBRACK);
            printToken(sym);

            sym = lexical.getSym();
        }
    } while (sym.getKey() == COMMA);
}

void syntacticAnalysis::funcWithReturn()
{
    string name = stateHead();
    assert(sym.getKey() == LPARENT);
    printToken(sym);
    sym = lexical.getSym();
    argumentList();
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym();
    funcWithRet.insert(name);
    printLine("<有返回值函数定义>");
}

void syntacticAnalysis::funcWithoutReturn()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    string name = sym.getValue();
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    argumentList();

    assert(sym.getKey() == RPARENT);
    printToken(sym);
    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym();
    funcWithoutRet.insert(name);
    printLine("<无返回值函数定义>");
}

void syntacticAnalysis::compoundStatement()
{
    if (sym.getKey() == CONSTTK)
    {
        sym = lexical.getSym();
        constState();
    }
    if (isTypeIdentifier(sym))
    {
        sym = lexical.getSym();
        variableState();
    }
    statementList();
    printLine("<复合语句>");
}

void syntacticAnalysis::argumentList()
{
    if (!isTypeIdentifier(sym))
    {
        printLine("<参数表>");
        return;
    }
    lexical.unGetSym();
    do
    {
        sym = lexical.getSym();
        assert(isTypeIdentifier(sym));
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        sym = lexical.getSym();
    } while (sym.getKey() == COMMA);
    printLine("<参数表>");
}

void syntacticAnalysis::mainFunc()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == MAINTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym();
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);
    printLine("<主函数>");
}

void syntacticAnalysis::expression()
{
    if (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
    }
    term();
    while (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        term();
    }
    printLine("<表达式>");
}

void syntacticAnalysis::term()
{
    factor();
    while (isMulOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        factor();
    }
    printLine("<项>");
}

void syntacticAnalysis::factor()
{
    if (sym.getKey() == IDENFR)
    {
        if (lexical.peek().getKey() == LPARENT)
        {
            invokeFuncWithReturn();
        }
        else if (lexical.peek().getKey() == LBRACK)
        {
            printToken(sym);
            sym = lexical.getSym();
            printToken(sym);
            sym = lexical.getSym();
            expression();
            assert(sym.getKey() == RBRACK);
            printToken(sym);
            sym = lexical.getSym();
        }
        else
        {
            sym = lexical.getSym();
        }
    }
    else if (sym.getKey() == LPARENT)
    {
        printToken(sym);

        sym = lexical.getSym();
        expression();
        assert(sym.getKey() == RPARENT);
        printToken(sym);

        sym = lexical.getSym();
    }
    else if (isCharacter(sym))
    {
        character();
    }
    else if (isAddOp(sym) || sym.getKey() == INTCON)
    {
        integer();
    }
    else
    {
        assert(false);
    }
    printLine("<因子>");
}

void syntacticAnalysis::statement()
{
    if (sym.getKey() == SEMICN)
    {
        sym = lexical.getSym();
    }
    else if (sym.getKey() == IFTK)
    {
        conditionalStatement();
    }
    else if (sym.getKey() == WHILETK || sym.getKey() == FORTK || sym.getKey() == DOTK)
    {
        loopStatement();
    }
    else if (sym.getKey() == LBRACE)
    {
        printToken(sym);
        sym = lexical.getSym();
        statementList();
        assert(sym.getKey() == RBRACE);
        printToken(sym);

        sym = lexical.getSym();
    }
    else
    {
        if (sym.getKey() == IDENFR)
        {
            sym = lexical.getSym();
            if (isFuncWithRet(sym))
            {
                invokeFuncWithReturn();
            }
            else if (isFuncWithoutRet(sym))
            {
                invokeFuncWithoutReturn();
            }
            else if (lexical.peek().getKey() == ASSIGN)
            {
                assignmentStatement();
            }
            else
            {
                assert(false);
            }
        }
        else if (sym.getKey() == SCANFTK)
        {
            readStatement();
        }
        else if (sym.getKey() == PRINTFTK)
        {
            writeStatement();
        }
        else if (sym.getKey() == RETURNTK)
        {
            returnStatement();
        }
        else
        {
            assert(false);
        }
        assert(sym.getKey() == SEMICN);
        printToken(sym);
        sym = lexical.getSym();
    }
    printLine("<语句>");
}

void syntacticAnalysis::conditionalStatement()
{
    assert(sym.getKey() == IFTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    condition();
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    statement();
    if (sym.getKey() == ELSETK)
    {
        printToken(sym);
        sym = lexical.getSym();
        statement();
    }
    printLine("<条件语句>");
}

void syntacticAnalysis::assignmentStatement()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);

    sym = lexical.getSym();
    if (sym.getKey() == LBRACK)
    {
        printToken(sym);

        sym = lexical.getSym();
        expression();
        assert(sym.getKey() == RBRACK);
        printToken(sym);

        sym = lexical.getSym();
    }
    assert(sym.getKey() == ASSIGN);
    printToken(sym);

    expression();
    printLine("<赋值语句>");
}

void syntacticAnalysis::condition()
{
    expression();
    if (isRelOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        expression();
    }
    printLine("<条件>");
}

void syntacticAnalysis::loopStatement()
{
    if (sym.getKey() == WHILETK)
    {
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym();
        condition();
        assert(sym.getKey() == RPARENT);
        printToken(sym);

        sym = lexical.getSym();
        statement();
    }
    else if (sym.getKey() == DOTK)
    {
        printToken(sym);
        sym = lexical.getSym();
        statement();
        assert(sym.getKey() == WHILETK);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym();
        condition();
        assert(sym.getKey() == RPARENT);
        printToken(sym);

        sym = lexical.getSym();
    }
    else if (sym.getKey() == FORTK)
    {
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        expression();
        assert(sym.getKey() == SEMICN);
        printToken(sym);

        sym = lexical.getSym();
        condition();
        assert(sym.getKey() == SEMICN);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        assert(isAddOp(sym));
        printToken(sym);

        sym = lexical.getSym();
        stepLength();

        assert(sym.getKey() == RPARENT);
        printToken(sym);

        sym = lexical.getSym();
    }
    else
    {
        assert(false);
    }
    printLine("<循环语句>");
}

void syntacticAnalysis::stepLength()
{
    unsignedInteger();
    printLine("<步长>");
}

void syntacticAnalysis::invokeFuncWithReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    valueArgumentList();
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<有返回值函数调用语句>");
}

void syntacticAnalysis::invokeFuncWithoutReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    valueArgumentList();
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<无返回值函数调用语句>");
}

// 注:此处使用了外部信息,即参数表后必须有")"
void syntacticAnalysis::valueArgumentList()
{
    if (sym.getKey() == RPARENT)
    {
        printLine("<值参数表>");
        return;
    }
    expression();
    while (sym.getKey() == COMMA)
    {
        printToken(sym);
        sym = lexical.getSym();
        expression();
    }
    printLine("<值参数表>");
}

void syntacticAnalysis::statementList()
{
    assert(sym.getKey() == LBRACK);
    printToken(sym);

    sym = lexical.getSym();
    statement();
    assert(sym.getKey() == RBRACK);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<语句列>");
}

void syntacticAnalysis::readStatement()
{
    assert(sym.getKey() == SCANFTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);

    do
    {
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        sym = lexical.getSym();
    } while (sym.getKey() == COMMA);
    assert(sym.getKey() == RPARENT);
    printToken(sym);
    sym = lexical.getSym();
    printLine("<读语句>");
}

void syntacticAnalysis::writeStatement()
{
    assert(sym.getKey() == PRINTFTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    if (sym.getKey() == STRCON)
    {
        strConCheck();
        if (sym.getKey() == COMMA)
        {
            printToken(sym);
            sym = lexical.getSym();
            expression();
        }
    }
    else
    {
        expression();
    }
    assert(sym.getKey() == RPARENT);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<写语句>");
}

void syntacticAnalysis::returnStatement()
{
    assert(sym.getKey() == RETURNTK);
    printToken(sym);
    sym = lexical.getSym();
    if (sym.getKey() == LPARENT)
    {
        printToken(sym);
        sym = lexical.getSym();
        expression();
        assert(sym.getKey() == RPARENT);
        printToken(sym);
        sym = lexical.getSym();
    }
    printLine("<返回语句>");
}