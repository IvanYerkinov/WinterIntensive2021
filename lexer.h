#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <list>

namespace Lexer
{

#define ENUM_IDENTIFIERS(o) \
        o(undefined)                              /* undefined */ \
        o(func)                               /* a pointer to given function */ \
        o(parameter)                              /* one of the function params */ \
        o(variable)                               /* a local variable */
#define o(n) n,
enum class id_type {  ENUM_IDENTIFIERS(o) };
#undef o

#define ENUM_EXPRESSIONS(o) \
        o(nop) o(str) o(number) o(ident)       /* atoms */ \
        o(add) o(neg) o(eq) o(mul) o(div)                       /* transformation */ \
        o(cor) o(cand) o(loop) o(cif)                    /* logic. Loop is: while(param0) { param1..n } */ \
        o(gr) o(ls) o(greq) o(lseq) \
        o(addrof) o(deref)                        /* pointer handling */ \
        o(fcall)                                  /* function param0 call with param1..n */ \
        o(cpy)                                   /* assign: param1 <<- param0 */ \
        o(comma)                                  /* a sequence of expressions */ \
        o(ret)                                    /* return(param0) */

#define o(n) n,
enum class ex_type {  ENUM_EXPRESSIONS(o) };
#undef o

    struct identifier
    {
        id_type id = id_type::undefined;
        std::size_t index = 0; //Function #, parameter #, Variable #
        std::string name;

        identifier(id_type i, std::size_t t, std::string n) : id(i), index(t), name(n) {}
        identifier(){}
    };

    typedef std::list<struct expression> expr_vec;
    struct expression
    {
        ex_type type;
        identifier ident{};
        std::string strvalue{};
        long numvalue = 0;
        expr_vec params;

        expression(){type = ex_type::nop;}
        expression(std::string& s){type = ex_type::str; strvalue = s;}
        expression(long&& i): type(ex_type::number), numvalue(i) {}
        expression(identifier&& i): type(ex_type::ident), ident(std::move(i)){}
    };

    #define o(n) \
inline bool is_##n(const identifier& i) { return i.id == id_type::n; }
ENUM_IDENTIFIERS(o)
#undef o

#define o(n) \
inline bool is_##n(const expression& e) { return e.type == ex_type::n; } \
template<typename... T> \
inline expression e_##n(T&&... args) { return expression(ex_type::n, std::forward<T>(args)...); }
ENUM_EXPRESSIONS(o)
#undef o

/*
    typedef std::list<struct statement> state_vec;
    struct statement
    {
        state_type type;

        identifier ident{}; //for 'set" operation
        expression exp{};
        state_vec next;
    };
*/

    struct func
    {
        std::string name;
        expr_vec code; //First X are parameters, last one is code
        unsigned int num_parameters = 0, num_vars = 0;

        func(){}
    };


    struct lexcon
    {
        std::vector<std::pair<unsigned int, std::string> > tokenlist;
        std::vector<Lexer::func> funclist;
        unsigned int tempcounter = 0;
        Lexer::func fun;


        identifier defvar(const std::string& name){return identifier(id_type::variable, fun.num_vars++, name);}
        identifier defun(const std::string& name){return identifier(id_type::func, funclist.size(), name);}
        identifier defparm(const std::string& name){return identifier(id_type::parameter, fun.num_parameters++, name);}


        void add_function(std::string&& name, Lexer::expression&& code)
        {
            fun.code.push_back(code);
            fun.name = name;
            funclist.push_back(fun);
            fun = {};
        }

        void add_expression(identifier&& id)
        {
            fun.code.push_back(expression(std::move(id)));
        }

    };

    enum Type {NONE, ID, INT, STR, END, IF, WHILE, RETURN, SET, ADD, SUB, DIV, MUL, PL_EQ, SUB_EQ, INC, DEINC, ASSIGN, EQUAL, AND, OR, GREATER, LESS, GR_EQ, LESS_EQ, P_O, P_C, B_O, B_C, C_O, C_C, NOT, NOT_EQ, COM, COL, SCOL, REF, DREF, F_CALL};

    std::vector<std::pair<unsigned int, std::string> > readTokenFile(std::string filename);

    std::vector<std::pair<unsigned int, std::string> > tokenize(std::string line);

    void readTokenList(std::vector<std::pair<unsigned int, std::string> > &a);

    Lexer::expression checkExpression(std::vector<std::pair<unsigned int, std::string> >::iterator &it, Lexer::lexcon &l);
    Lexer::expression checkStatement(std::vector<std::pair<unsigned int, std::string> >::iterator& it, Lexer::lexcon &l);
    void checkFunction(std::vector<std::pair<unsigned int, std::string> >::iterator &it, Lexer::lexcon &l);

    int getPrecedence(std::pair<unsigned int, std::string> token);
    bool comparePrecedence(std::pair<unsigned int, std::string> token1, std::pair<unsigned int, std::string> token2);
    //
    std::vector<std::pair<unsigned int, std::string> > toPostFix(std::vector<std::pair<unsigned int, std::string> > &infix);
    Lexer::expression ToExpression(std::vector<std::pair<unsigned int, std::string> >&& postfix, Lexer::lexcon &l);

    std::vector<Lexer::func> parseTokens(std::string filename);

    void readFuncList(std::vector<Lexer::func> flist);
    std::string readExprType(Lexer::expression& e);

}

#endif // LEXER_H
