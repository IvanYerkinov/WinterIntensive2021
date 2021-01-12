#ifndef COMPILER_H
#define COMPILER_H
#include"lexer.h"
#include<map>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<type_traits>
#include<list>

namespace Compiler
{
#define ENUM_STATEMENTS(o) \
        o(0,nop)        /* placeholder that does nothing */ \
        o(1,init)       /* p0 <- &IDENT + value (assign a pointer to name resource with offset) */ \
        o(0,add)        /* p0 <- p1 + p2           */ \
        o(0,neg)        /* p0 <- -p1               */ \
        o(0,copy)       /* p0 <- p1      (assign a copy of another var) */ \
        o(0,read)       /* p0 <- *p1     (reading dereference) */ \
        o(0,write)      /* *p0 <- p1     (writing dereference) */ \
        o(0,eq)         /* p0 <- p1 == p2          */ \
        o(1,ifnz)       /* if(p0 != 0) JMP branch  */ \
        o(0,fcall)      /* p0 <- CALL(p1, <LIST>)  */ \
        o(0,ret)        /* RETURN p0;              */

#define o(_,n) n,
enum class st_type { ENUM_STATEMENTS(o) };
#undef o


struct statement
{
    typedef unsigned reg_type;

    st_type type{st_type::nop};
    std::string ident{}; //refrence for init, empty = none.
    long value{}; //for init, literal/offset
    std::vector<reg_type> params{}; //variable indexs
    statement* next{nullptr}; //next statment, nullptr = last
    statement* cond{nullptr}; //for conditionals, ifnz var[p0] <> 0 runs this.

    statement() {}

    void Dump() const;

/*
    template<class...T, class=forbid_t<st_type,long,statement*>::in<T...>>
    statement(st_type t, T&&...r)                  : statement(std::forward<T>(r)...) { type=t; }

    template<class...T>
    statement(reg_type tgt, T&&...r)               : statement(&tgt, &tgt+1,  std::forward<T>(r)...) {}

    // Special types that also force the statement type:
    template<class...T, class=forbid_t<st_type,long>::in<T...>>
    statement(std::string_view i, long v, T&&...r) : statement(st_type::init, std::forward<T>(r)...) { ident=i; value=v; }

    template<class...T, class=forbid_t<st_type,statement*>::in<T...>>
    statement(statement* b, T&&...r)               : statement(st_type::ifnz, std::forward<T>(r)...) { cond=b; }

    // An iterator range can be used to assign register params
    template<class...T, class It, class=require_iterator_t<It, reg_type, std::input_iterator_tag>>
    statement(It begin, It end, T&&...r)           : statement(std::forward<T>(r)...) { params.insert(params.begin(), begin, end); }
*/
};

struct compilation_context
{
    Compiler::statement::reg_type counter; //next unused register number
    Compiler::statement** tgt; //where the next instruction will be stored
    std::map< std::size_t, Compiler::statement::reg_type > regmap; // AST variables to register number mapping
};

#define o(_,n) \
inline bool is_##n(const statement& s) { return s.type == st_type::n; }
ENUM_STATEMENTS(o)
#undef o

struct compilation
{
    std::vector<std::unique_ptr<statement > > allStatements; //ALL statements


    template<typename... T>
    statement* CreateStatement(T&&... args) { return CreateStatement(new statement(std::forward<T>(args)...)); }
    statement* CreateStatement(Compiler::statement*s) { allStatements.emplace_back(s); return s; }

    #define o(f,n) /* f: flag that indicates if there's a special constructor that doesn't need the type */ \
    template<typename... T> \
    inline statement* s_##n(Compiler::statement* s) {s->type = st_type::n; return CreateStatement(s);}
    /*
    inline statement* s_##n(T&&... args) { if (f == 1)  return CreateStatement(std::forward<T>(args)...); \
                                           else return CreateStatement(st_type::n, std::forward<T>(args)...); }
    */
    ENUM_STATEMENTS(o)
    #undef o

    std::map<std::string, std::size_t> functionParameters;
    std::map<std::string, Compiler::statement*> entryPoints;

    std::string stringConstants;

    void build_strings(std::vector<Lexer::func> &funcList);

    void Compile(std::vector<Lexer::func> &funcList);
    void CompileFunction(Lexer::func f);
    void Compile(Lexer::expr_vec &v, compilation_context& ctx);
    statement::reg_type Compile(const Lexer::expression& code, compilation_context& ctx);

    void Dump();

};

}

#endif // COMPILER_H
