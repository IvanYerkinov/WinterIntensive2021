#include "compiler.h"

bool buildHelper(Lexer::expression &e)
{
    switch (e.type)
    {
        case Lexer::ex_type::str:
            return 1;
        default:
            return 0;
    }
}

void buildHelper2(Lexer::expression &e, std::vector<std::string > &strings)
{
    if(buildHelper(e))
            {
                strings.push_back(e.strvalue + '\0');
            }


    if(e.params.size() > 0)
        {
            for (auto &a : e.params)
            {
                buildHelper2(a, strings);
            }
        }
   /* else if (e.params.size() == 1)
        {
            buildHelper2(e.params.front(), strings);
        } */

}

void Compiler::compilation::build_strings(std::vector<Lexer::func> &funcList)
{
    std::vector<std::string > strings;

    for (auto a : funcList)
    {
        Lexer::expression tempexpr = a.code.back();

        buildHelper2(tempexpr, strings);
    }

    std::sort(strings.begin(), strings.end(),
            [](const std::string& a, const std::string& b)
            {
                return a.size()==b.size() ? (a<b) : (a.size()>b.size());
            });

    for(const auto& s: strings)
        if(stringConstants.find(s) == stringConstants.npos)
        {
            //std::cout << s << "\n";
            stringConstants += s;
        }

}


void Compiler::compilation::Compile(std::vector<Lexer::func> &funcList)
{
    build_strings(funcList);

    for(auto &a : funcList) CompileFunction(a);
}

Compiler::statement::reg_type Compiler::compilation::Compile(const Lexer::expression& code, compilation_context& ctx)
{
    statement::reg_type result = ~statement::reg_type();

    auto make = [&](){return ctx.counter++;};
    auto put  = [&](statement* s) { for(*ctx.tgt = s; s; s = *ctx.tgt) ctx.tgt = &s->next; };

    Compiler::statement* state = new Compiler::statement();

    switch(code.type)
    {
    case Lexer::ex_type::str:
        {
            //CREATE Init command

            result = make();
            state->ident = "$STR:";
            state->value = (long)stringConstants.find(code.strvalue + '\0');
            state->params.push_back(result);

            put(s_init(state));
            //put(s_init(result = make(), "$STR", (long)stringConstants.find(code.strvalue + '\0')));
            break;
        }
    case Lexer::ex_type::ident:
        {
            auto& ide = code.ident;

            switch(ide.id)
            {
                case Lexer::id_type::func:
                    {
                        //Compiler::statement* state = new Compiler::statement();
                        result = make();
                        state->ident = ide.name;
                        state->value = 01;
                        state->params.push_back(result);
                        put(s_init(state));
                        //put(s_init(result = make(), id.name, 01 ) );
                        break;
                    }
                case Lexer::id_type::variable:
                    {
                        //std::cout << "TEST VARIABLE\n";
                        result = ctx.regmap.emplace(ide.index, make()).first->second; break;
                    }
                case Lexer::id_type::parameter: result = ide.index; break;
                case Lexer::id_type::undefined: std::cerr << "UNDEFINED IDENTIFIER, DON'T KNOW WHAT TO DO\n"; break;
            }
            break;
        }
    case Lexer::ex_type::neg:
        {
            //Compiler::statement* state = new Compiler::statement;
            result = make();
            state->params.push_back(result);
            state->params.push_back(Compile(code.params.front(), ctx));
            put(s_neg(state));
            break;
            //put(s_neg(result = make(), Compile(code.params.front(), ctx) ) ); break;
        }
    case Lexer::ex_type::ret:
        {
            //Compiler::statement* state = new Compiler::statement;
            result = Compile(code.params.front(), ctx);
            state->params.push_back(result);
            put(s_ret(state));
            break;
            //put(s_ret(result = Compile(code.params.front(), ctx))); break;
        }
    case Lexer::ex_type::number:
        {
            result = make();
            state->ident = "";
            state->value = code.numvalue;
            state->params.push_back(result);
            put(s_init(state));
            break;
            //put(s_init(result = make(), "", code.numvalue)); break;
        }
    case Lexer::ex_type::nop:
        {
            result = make();
            state->ident = "";
            state->value = 0L;
            state->params.push_back(result);
            put(s_init(state));
            break;
            //put(s_init(result = make(), "", 0L)); break;
        }
    case Lexer::ex_type::addrof: std::cerr << "NO IDEA WHAT TO DO" << '\n'; break; // Unhandleable

    case Lexer::ex_type::add:
        {
            result = make();
            state->params.push_back(result);
            state->params.push_back(Compile(code.params.front(), ctx));
            state->params.push_back(Compile(code.params.back(), ctx));
            put(s_add(state));
            break;
        }
    case Lexer::ex_type::eq:
        {
            result = make();
            state->params.push_back(result);
            state->params.push_back(Compile(code.params.front(), ctx));
            state->params.push_back(Compile(code.params.back(), ctx));
            put(s_eq(state));
            break;
        }
    case Lexer::ex_type::comma:
        {

            Compiler::statement::reg_type last;
        for (auto i = code.params.begin(); i != code.params.end(); i++)
        {
            last = Compile(*i, ctx);
        }
            result = last;
            break;
        }

    case Lexer::ex_type::cpy:
        {
            //std::cout <<"EQTEST"<<"\n";
            auto temp = Compile(code.params.back(), ctx);
            state->params.push_back(result = Compile(code.params.front(), ctx));
            state->params.push_back(temp);
            put(s_copy(state));
            //put(s_copy(result = Compile(code.params.back(), ctx), temp));
            break;
        }
    case Lexer::ex_type::fcall:
        {
            Compiler::statement *state = new Compiler::statement();
            result = make();
            state->params.push_back(result);

            Lexer::expression temp = code;
            temp.type = Lexer::ex_type::ident;
            result = Compile(temp, ctx);
            state->params.push_back(result);

            state->type = Compiler::st_type::fcall;

            bool lop = 1;
            Lexer::expression current = code.params.front();
            while(lop)
            {
                if (current.type == Lexer::ex_type::comma)
                {
                    //std::cout << Lexer::readExprType(current.params.front()) << " " << Lexer::readExprType(current.params.back()) << "\n";
                    state->params.push_back(Compile(current.params.front(), ctx));
                    current = current.params.back();
                }
                else
                {
                    lop = 0;
                }

            }
            state->params.push_back(Compile(current, ctx));

            put(state);
            break;
        }


    case Lexer::ex_type::cand:
        {

            result = make();
            state->params.push_back(Compile(code.params.front(), ctx));
            s_ifnz(state);
            put(state);

            Compiler::statement *cond2 = new Compiler::statement;
            s_ifnz(cond2);
            cond2->params.push_back(Compile(code.params.back(), ctx));
            state->cond = cond2;

            Compiler::statement *res1 = new Compiler::statement;
            s_init(res1);
            res1->params.push_back(result);
            res1->ident = "";
            res1->value = 0;
            cond2->next = res1;
            put(res1);

            Compiler::statement *res2 = new Compiler::statement;
            s_init(res2);
            res2->params.push_back(result);
            res2->ident = "";
            res2->value = 1;
            cond2->cond = res2;

            Compiler::statement *endpoint = new Compiler::statement;
            s_nop(endpoint);
            res2->next = endpoint;

            put(endpoint);

            break;
        }
    case Lexer::ex_type::cor:
        {
            result = make();
            state->params.push_back(Compile(code.params.front(), ctx));
            s_ifnz(state);
            put(state);

            Compiler::statement *cond2 = new Compiler::statement;
            s_ifnz(cond2);
            cond2->params.push_back(Compile(code.params.back(), ctx));
            put(cond2);

            Compiler::statement *res1 = new Compiler::statement;
            s_init(res1);
            res1->params.push_back(result);
            res1->ident = "";
            res1->value = 0;
            put(res1);

            Compiler::statement *res2 = new Compiler::statement;
            s_init(res2);
            res2->params.push_back(result);
            res2->ident = "";
            res2->value = 1;
            cond2->cond = res2;
            state->cond = res2;

            Compiler::statement *endpoint = new Compiler::statement;
            s_nop(endpoint);
            res2->next = endpoint;

            put(endpoint);

            break;
        }

    case Lexer::ex_type::cif:
    {
        //result = make();
        s_ifnz(state);
        state->params.push_back(Compile(code.params.front(), ctx));
        put(state);


        Compiler::statement* beg = state;

        bool lop = 0;


        for(auto b : code.params.back().params)
        {
            //std::cout << Lexer::readExprType(b) << "\n";
            Compiler::statement *cond = new Compiler::statement;

            s_ifnz(cond);
            cond->params.push_back(Compile(b, ctx));
            cond->ident = "";
            cond->value = 0L;

        }


        //cond->params.push_back(result);


        //state->cond = cond;

        Compiler::statement *endpoint = new Compiler::statement;
        s_nop(endpoint);
        put(endpoint);
        endpoint->ident = "";
        endpoint->value = 0L;
        beg->cond = state->next;
        beg->next = endpoint;
        //endpoint->params.push_back(Compile(code.params.back(), ctx));
        //*ctx->next = endpoint;

        break;

    }

    case Lexer::ex_type::loop:
        {
            //result = Compile(code.params.front(), ctx);
            Compiler::statement* start = new Compiler::statement;
            s_ifnz(start);
            start->params.push_back(result = Compile(code.params.front(), ctx));
            put(start);

            s_ifnz(state);
            state->params.push_back(Compile(code.params.front(), ctx));

            bool lop = 0;

            Compiler::statement* beg = start;


            put(state);

            Compiler::statement* jump;


            for(auto b : code.params.back().params)
                {
            //std::cout << Lexer::readExprType(b) << "\n";
            Compiler::statement *cond = new Compiler::statement;
            s_ifnz(cond);
            cond->ident = "";
            cond->value = 0L;
            cond->params.push_back(Compile(b, ctx));

                }

            //put(jump);

            Compiler::statement *endpoint = new Compiler::statement;
            s_ifnz(endpoint);
            endpoint->params.push_back(result);
            endpoint->ident = "";
            endpoint->value = 0L;
            //beg->next = endpoint;
            start->next = endpoint;
            beg->cond = state->next;

            endpoint->cond = state->next;
            //state->cond = start;
            //beg->next = state->cond;
            //start->cond = endpoint;
            //state->next = endpoint;
        //endpoint->params.push_back(Compile(code.params.back(), ctx));
        //*ctx->next = endpoint;
            put(endpoint);
            break;
        }

/*
    case Lexer::ex_type::loop:
    case Lexer::ex_type::cand:
    case Lexer::ex_type::cor:
    //case Lexer::ex_type::cif:
        {
            const bool is_and = !is_cor(code);
            result = make();

            Compiler::statement* b_then = new Compiler::statement;
            b_then->type = Compiler::st_type::init;
            b_then->ident = "";
            b_then->value = is_and ? 1l : 0l;
            b_then->params.push_back(result);
            //Compiler::statement* b_then = s_init(result, "", is_and ? 1l : 0l);

            Compiler::statement* b_else = new Compiler::statement;
            b_else->type = Compiler::st_type::init;
            b_else->ident = "";
            b_else->value = is_and ? 0l : 1l;
            b_else->params.push_back(result);
            //Compiler::statement* b_else = s_init(result, "", is_and ? 0l : 1l);

            Compiler::statement* en    = new Compiler::statement;
            s_nop(en); b_then->next = b_else->next = en;

            Compiler::statement*& beg = *ctx.tgt;
            for(auto i = code.params.begin(); i != code.params.end(); ++i)
            {
                Compiler::statement::reg_type var = Compile(*i, ctx);

                if(is_loop(code) && i != code.params.begin()) {continue;}


                statement* ifn = new Compiler::statement;
                ifn->params.push_back(var);
                //ifn->next = nullptr;
                //s_ifnz(var, nullptr);
                statement* condition = *ctx.tgt = s_ifnz(ifn);;

                if(is_and) {ctx.tgt = &condition->cond; condition->next = b_else;}
                else {ctx.tgt = &condition->next; condition->cond = b_else; }
            }
            *ctx.tgt = is_loop(code) ? beg : b_then;
            ctx.tgt = &en->next;  // Code continues after the end node.
            break;
        }
        */

    }
    return result;
}

Lexer::expression compileIf(Lexer::expression &code)
{

}

void Compiler::compilation::CompileFunction(Lexer::func f)
{
    functionParameters[f.name] = f.num_parameters;

    Compiler::compilation_context ctx{f.num_parameters, &entryPoints[f.name], {} };
    Compile(f.code, ctx);
    //Compile(f.code.back().params.front(), ctx);
    //Compile(f.code.back().params.back(), ctx);

}

void Compiler::compilation::Compile(Lexer::expr_vec& v, compilation_context& ctx)
{

for (auto& e : v)
{
    {
            for (auto a = e.params.begin(); a != e.params.end(); a++)
            {


                if((*a).type == Lexer::ex_type::cif)
                {
                    for(auto b : (*a).params.back().params)
                    {
                        //std::cout << Lexer::readExprType(b) << "\n";
                    }
                    (*a).params.push_back((*a).params.back());

                    Compile(*a, ctx);
                }
                else
                {
                    Compile(*a, ctx);
                }

            }
    }
}
}


void Compiler::statement::Dump() const
{
    switch(type)
    {
        #define o(_,n) case st_type::n: std::cout << "\t" #n "\t"; break;
        ENUM_STATEMENTS(o)
        #undef o
    }
    for (auto u : params) std::cout << " R" << u;
    if(type == Compiler::st_type::init) {std::cout <<  " \"" << ident << "\" " << value; }
}



void Compiler::compilation::Dump()
{
    struct data
    {
        std::vector<std::string > labels{};
        std::size_t done{}, referred{};
    };
    std::map<Compiler::statement*, data > statistics;
    std::list<statement* > remaining_statements;

    std::cout << allStatements.size() << "\n";

    auto addLabel = [l=01u](data &d) mutable {d.labels.push_back("L" + std::to_string(l++));};

    for (const auto& s : entryPoints)
    {
        remaining_statements.push_back(s.second);
        statistics[s.second].labels.push_back(s.first);
    }

    for (const auto& s : allStatements)
    {
        if(s->next) { auto& t = statistics[s->next]; if(t.labels.empty() && t.referred++) addLabel(t); }
        if(s->cond) { auto& t = statistics[s->cond]; if(t.labels.empty()) addLabel(t); }
    }

    while(!remaining_statements.empty())
    {
        Compiler::statement* chain = remaining_statements.front(); remaining_statements.pop_front();
        for(bool needs_jump = false; chain != nullptr; chain = chain->next, needs_jump = true)
        {
            auto& stats = statistics[chain];
            if(stats.done++)
            {
                if(needs_jump) {std::cout << "\tJMP " << stats.labels.front() << '\n';}
                break;
            }

            for(const auto& l: stats.labels) std::cout << l << ":\n";
            chain->Dump();
                if(chain->cond)
                {
                    auto& branch_stats = statistics[chain->cond];
                    std::cout << ", JMP " << branch_stats.labels.front();
                    if(!branch_stats.done) { remaining_statements.push_front(chain->cond); }
                }
                std::cout << '\n';
        }
    }
}
