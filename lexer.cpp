#include "lexer.h"
#include <iostream>

std::vector<std::pair<unsigned int, std::string> > Lexer::readTokenFile(std::string filename){
    std::string line;

    std::vector<std::pair<unsigned int, std::string> > tokenlist;

    std::ifstream myfile(filename);
    if(myfile.is_open())
    {
        while(std::getline(myfile, line))
        {
            //break into words
            //tokenize(line);
            for (auto a : Lexer::tokenize(line))
            {
                tokenlist.push_back(a);
            }
        }
    }
    tokenlist.push_back(std::pair<unsigned int, std::string>(Lexer::Type::END, ""));
    return tokenlist;
}


std::vector<std::pair<unsigned int, std::string> > Lexer::tokenize(std::string line){
    char now;
    char next;

    std::vector<std::pair<unsigned int, std::string> > tokenList;

    for(auto i = line.begin(); i != line.end(); i++)
    {
        int type = 0;
        std::string currtoken;
        now = *i;
        next = *(i + 1);

        //checkLit
        if (now == ' ' || now == '\n')
        {

        }
        else if ((now >= 'a' && now <= 'z') || (now >= 'A' && now <= 'Z') || (now == '_') )
        {
            currtoken.push_back(now);
            while(((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z') ) || (next >= '0' && next <= '9') || (next == '_'))
            {
                currtoken.push_back(next);
                i++;
                now = *(i);
                next = *(i + 1);
            }
            type = Lexer::Type::ID;

            if (currtoken == "while") type = Lexer::Type::WHILE;
            else if (currtoken == "set") type = Lexer::Type::SET;
            else if (currtoken == "if") type = Lexer::Type::IF;
            else if (currtoken == "return") type = Lexer::Type::RETURN;

            std::pair<unsigned int, std::string> token(type, currtoken);
            tokenList.push_back(token);
            //i++;
            //get all letters/numbers in row
            //push to token

        }
        else if (now == '\"')
        {
            currtoken.push_back(now);
            if (next != '\"')
            {
            while(next != '\"')
            {

                now = *(i);
                next = *(i + 1);

                currtoken.push_back(next);
                i++;
            }
            }
            else
            {
                currtoken.push_back(next);
                i++;
            }

            std::pair<unsigned int, std::string> token(Lexer::Type::STR, currtoken);
            tokenList.push_back(token);
            //i++;

        }
        else if (now >= '0' && now <= '9')
        {
            currtoken.push_back(now);
            while(next >= '0' && next <= '9')
            {
                i++;
                now = *(i);
                next = *(i + 1);

                currtoken.push_back(now);
            }
            std::pair<unsigned int, std::string> token(Lexer::Type::INT, currtoken);
            tokenList.push_back(token);
            //i++;
            //get all numbers in row
            //[ush to token
        }
        else if (now != ' ' || now != '\n')
        {

            currtoken.push_back(now);

            if(now == '|')
            {
                //type = Lexer::Type::
                if(next == '|')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::OR;
                    i++;
                }
            }
            else if(now == '+')
            {

                type = Lexer::Type::ADD;

                if(next == '+')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::INC;
                    i++;
                }

                if(next == '=')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::PL_EQ;
                    i++;
                }
            }
            else if (now == '-')
            {
                type = Lexer::Type::SUB;

                if(next == '-')
                {
                    type = Lexer::Type::DEINC;
                    currtoken.push_back(next);
                    i++;
                }


                if(next == '=')
                {
                    type = Lexer::Type::SUB_EQ;
                    currtoken.push_back(next);
                    i++;
                }
            }
            else if (now == '&')
            {
                type = Lexer::Type::REF;

                if(next == '&')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::AND;
                    i++;
                }
            }
            else if (now == '!')
            {
                type = Lexer::Type::NOT;

                if(next == '=')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::NOT_EQ;
                    i++;
                }
            }
            else if (now == '=')
            {

                type = Lexer::Type::ASSIGN;

                if(next == '=')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::EQUAL;
                    i++;
                }
            }
            else if (now == '<')
            {
                type = Lexer::Type::LESS;
                if(next == '=')
                {
                    type = Lexer::Type::LESS_EQ;
                    currtoken.push_back(next);
                    i++;
                }
            }
            else if (now == '>')
            {
                type = Lexer::Type::GREATER;
                if(next == '=')
                {
                    currtoken.push_back(next);
                    type = Lexer::Type::GR_EQ;
                    i++;
                }
            }
            else if (now == '(') type = Lexer::Type::P_O;
            else if (now == ')') type = Lexer::Type::P_C;
            //else if (now == '[') type = Lexer::Type::B_O;
            //else if (now == ']') type = Lexer::Type::B_C;
            else if (now == '{') type = Lexer::Type::C_O;
            else if (now == '}') type = Lexer::Type::C_C;
            else if (now == '*') type = Lexer::Type::MUL;
            else if (now == '/') type = Lexer::Type::DIV;
            else if (now == ',') type = Lexer::Type::COM;
            else if (now == ':') type = Lexer::Type::COL;
            else if (now == ';') type = Lexer::Type::SCOL;
            else
            {
               throw [](){std::cout << "Unknown Token!" << "\n"; return "One"; }();
            }
            tokenList.push_back(std::pair<unsigned int, std::string>(type, currtoken));
        }
    }

    return tokenList;

}


void Lexer::readTokenList(std::vector<std::pair<unsigned int, std::string> > &a)
{
    for (auto b : a)
    {
        std::string prin;
        switch (b.first)
        {
        case Lexer::Type::IF:
            prin = "IF";
            break;
        case Lexer::Type::AND:
            prin = "AND";
            break;
        case Lexer::Type::OR:
            prin = "OR";
            break;
        case Lexer::Type::WHILE:
            prin = "WHILE";
            break;
        case Lexer::Type::RETURN:
            prin = "RETURN";
            break;
        case Lexer::Type::SET:
            prin = "VAR";
            break;
        case Lexer::Type::ID:
            prin = "IDENTIFIER";
            break;
        case Lexer::Type::INT:
            prin = "INTEGER";
            break;
        case Lexer::Type::STR:
            prin = "STRING";
            break;
        default:
            prin = "";
            break;
        }
        std::cout << "<" << prin << " " << b.second << ">  ";
    }
}

bool checkIter(int a, int b)
{
    if(a == b)
    {
        return true;
    }
    else return false;
}

void Lexer::checkFunction(std::vector<std::pair<unsigned int, std::string> >::iterator &it, Lexer::lexcon &l)
{
    std::string name;

    if ((*it).first == Lexer::Type::ID)
    {
        name = (*it).second;
        it++;
    }
    else throw [=](){std::cout << "Expected function ID, got " << (*it).second << "\n"; return "";}();

    while((*it).first != Lexer::Type::COL)
    {
        if((*it).first == Lexer::Type::ID)
        {
            l.add_expression(l.defparm((*it).second));

            it++;

            if(!checkIter((*it).first, Lexer::Type::COL))
            {

                if(checkIter(((*it).first), Lexer::Type::COM))
                {
                    it++;
                } else throw [=](){std::cout << "Expected comma after parameter, got " << (*it).second << "\n"; return "";}();
            }

        } else throw [=](){std::cout << "Expected parameter ID, got " << (*it).first << "\n"; return "";}();

    }
    it++;
    l.add_function(std::move(name), Lexer::checkStatement(it, l));
    /*
    checkIdentifier(it, currentFunc, Function #);
    iterator++;

    while (curr_token is not ':')
    {
        checkIdentifier(Parameter #)
        iterator++;
        Add parameter
        if curr_token is ','
            iterator++
        else
            throw error
    }
    checkStatement()
    return func
    */
}


Lexer::expression Lexer::checkStatement(std::vector<std::pair<unsigned int, std::string> >::iterator& it, Lexer::lexcon &l)
{
    //returns one Expression struct either with all other expression in tree.
    Lexer::expression ret;

    switch ((*it).first)
    {
    case Lexer::Type::C_O:
        {
            it++;
            Lexer::expression temp;
            //temp.params.push_back(checkStatement(it, l));
            while(!checkIter((*it).first, Lexer::Type::C_C))
            {
                if(checkIter((*it).first, Lexer::Type::END))
                {
                    throw [=](){std::cout << "Expected '}', got End Of File" <<  "\n"; return 5;}();
                }
                temp.params.push_back(checkStatement(it, l));
            }
            if(checkIter((*it).first, Lexer::Type::C_C))
            {
                it++;
                return temp;
            } else
            throw [=](){std::cout << "Expected '}', got " << (*it).second << "\n"; return 5;}();
        }
        break;

    case Lexer::Type::IF:
        {
            it++;
            if(checkIter((*it).first, Lexer::Type::P_O))
            {
                it++;
                ret.params.push_back(checkExpression(it, l));
            } else throw [=](){std::cout << "Expected '(', got " << (*it).first << "\n"; return 6;}();
            if(checkIter((*it).first, Lexer::Type::P_C))
            {
                it++;
                ret.type = Lexer::ex_type::cif;
                ret.params.push_back(checkStatement(it, l));
                return ret;
            } else throw [=](){std::cout << "Expected ')', got " << (*it).first << "\n"; return 7;}();
        }
        break;

    case Lexer::Type::WHILE:
        {
            it++;
            if(checkIter((*it).first, Lexer::Type::P_O))
            {
                it++;
                ret.params.push_back(checkExpression(it, l));
            } else throw [=](){std::cout << "Expected '(', got " << (*it).first << "\n"; return 6;}();
            if(checkIter((*it).first, Lexer::Type::P_C))
            {
                it++;
                ret.type = Lexer::ex_type::loop;
                ret.params.push_back(checkStatement(it, l));
                return ret;
            } else throw [=](){std::cout << "Expected ')', got " << (*it).first << "\n"; return 7;}();
        }
        break;

    case Lexer::Type::RETURN:
        {
            it++;
            ret.params.push_back(checkExpression(it, l));
            if(checkIter((*it).first, Lexer::Type::SCOL))
            {
                it++;
                ret.type = Lexer::ex_type::ret;
                return ret;
            } else throw [=](){std::cout << "Expected ';', got " << (*it).first << "\n"; return 8;}();
        }
        break;
    case Lexer::Type::SCOL:
        {
            it++;
            ret.type = Lexer::ex_type::nop;
            return ret;
        }
        break;

    case Lexer::Type::END:
        {
            throw [=](){std::cout << "Error: End Of File reached while parsing Statements.\n"; return 9;}();
        }
        break;

    case Lexer::Type::SET:
        {
            it++;
            Lexer::expression temp;
            if(checkIter((*it).first, Lexer::Type::ID))
            {
                temp = l.defvar((*it).second);
                it++;
            } else throw [=](){std::cout << "Error: Set requires variable identifier.\n"; return 9;}();

            while(!checkIter((*it).first, Lexer::Type::SCOL))
            {
                /*
                while(checkIter((*it).first, Lexer::Type::COM))
                {
                    //
                    it++;
                }
                */

                if(checkIter((*it).first, Lexer::Type::ASSIGN))
                {
                    it++;
                    ret.type = Lexer::ex_type::cpy;
                    ret.params.push_back(temp);
                    ret.params.push_back(Lexer::checkExpression(it, l));
                }
            }
            if(checkIter((*it).first, Lexer::Type::SCOL))
            {
                it++;
                return ret;
            }
            return ret;

        } break;

    default:
        {
            Lexer::expression exp1 = checkExpression(it, l);
            //std::cout << "TEST3\n";
            if(checkIter((*it).first, Lexer::Type::SCOL))
            {
                it++;
                return exp1;
            } else [=](){std::cout << "Expected ';', got " << (*it).first << "\n"; return 10;}();
        }
        break;

    }
}

Lexer::expression Lexer::checkExpression(std::vector<std::pair<unsigned int, std::string> >::iterator &it, Lexer::lexcon &l)
{
    std::vector<std::pair<unsigned int, std::string> > infixTokens;
    int termchars = 1;

    while(termchars != 0)
    {
        if(checkIter((*it).first, Lexer::Type::SCOL))
        {

            if(!infixTokens.empty())
            {
                return Lexer::ToExpression( Lexer::toPostFix(infixTokens), l );
            }
            else
            {

                Lexer::expression ret;
                ret.type = ex_type::nop;
                return ret;
            }

        }

        if(checkIter((*it).first, Lexer::P_O))
        {
            termchars++;
        }

        if(checkIter((*it).first, Lexer::P_C))
        {
            termchars--;
        }

        if(termchars != 0)
        {
            infixTokens.push_back((*it));
            it++;
        }

        if(checkIter((*it).first, Lexer::Type::END))
        {
            throw [](){std::cout<<"Error parsing expression, end of file\n"; return 1;}();
        }
    }
   return Lexer::ToExpression( Lexer::toPostFix(infixTokens), l );
}

//input a list of tokens, outputs a list of tokens in postfix notation
std::vector<std::pair<unsigned int, std::string> > Lexer::toPostFix(std::vector<std::pair<unsigned int, std::string> > &infix)
{
    std::vector<std::pair<unsigned int, std::string> > postfix;
    std::vector<std::pair<unsigned int, std::string> > stac;

    for(auto a = infix.begin(); a != infix.end(); a++)
    {
        //if operand put into postfix
        if ((*a).first == Lexer::Type::ID || (*a).first == Lexer::Type::INT || (*a).first == Lexer::Type::STR)
        {
            if((*(a+1)).first == Lexer::Type::P_O)
               {
                   (*a).first = Lexer::Type::F_CALL;
               } else
               {
                   postfix.push_back((*a));
               }



            if( (*a).first == Lexer::Type::ID && (*(a+1)).first == Lexer::Type::P_O && (*(a+2)).first == Lexer::Type::P_C)
            {
                postfix.push_back(std::pair<unsigned int, std::string>(Lexer::Type::NONE, ""));
            }

        }

        //if open parenthesis '(', push onto stack
        if((*a).first == Lexer::Type::P_O)
        {
            stac.push_back((*a));
        }
        //if closed parenthesis ')', pop from stack all operators until '(' is encountered
        if((*a).first == Lexer::Type::P_C)
        {
            auto b = stac.end() - 1;
            while((*b).first != Lexer::Type::P_O)
            {
                postfix.push_back((*b));
                b--;
                stac.pop_back();
            }
        }
        //if operator
        if((*a).first != Lexer::Type::ID && (*a).first != Lexer::Type::INT && (*a).first != Lexer::Type::END && (*a).first != Lexer::Type::IF && (*a).first != Lexer::Type::NONE && (*a).first != Lexer::Type::RETURN && (*a).first != Lexer::Type::SCOL && (*a).first != Lexer::Type::SET && (*a).first != Lexer::STR && (*a).first != Lexer::P_O && (*a).first != Lexer::P_C)
        {
            stac.push_back((*a));
            //check precedence and push all operators of same or higher precedence from stack to postfix
            for(auto b = stac.end()-1; b >= stac.begin(); b--)
            {
                if((*b).first != Lexer::Type::P_O)
                {
                    if (Lexer::comparePrecedence((*a), (*b)))
                    {

                        postfix.push_back(*b);
                        stac.erase(b);
                    }
                }

                if((*b).first == Lexer::Type::P_O) break;
            }
            //push operator to stack

        }

        //else throw [=](){std::cout << "Error: Postfix function encountered unexpected token: " << (*a).second << "\n"; return 4;}();
    }
    // Pop all elements of stack to make sure it is empty.

    for (auto a = stac.rbegin(); a != stac.rend(); a++)
    {
        //std::cout << "STACK: " << (*a).second << "\n";
        postfix.push_back((*a));
    }
/*
    for(auto a: postfix)
    {
        std::cout << a.second << ", ";
    }
    std::cout << "\n";

    for(auto a: infix)
    {
        std::cout << a.second << ", ";
    }
*/
    return postfix;
}

int Lexer::getPrecedence(std::pair<unsigned int, std::string> token)
{
    int ret = 0;
    switch (token.first)
    {
    case Lexer::Type::COM:
        ret = 0;
        break;
    case Lexer::Type::ASSIGN:
    case Lexer::Type::PL_EQ:
    case Lexer::Type::SUB_EQ:
        ret = 1;
        break;
    case Lexer::Type::OR:
        ret = 2;
        break;
    case Lexer::Type::AND:
        ret = 3;
        break;
    case Lexer::Type::EQUAL:
    case Lexer::Type::NOT_EQ:
        ret = 4;
        break;
    case Lexer::Type::GREATER:
    case Lexer::Type::LESS:
    case Lexer::Type::LESS_EQ:
    case Lexer::Type::GR_EQ:
        ret = 5;
        break;
    case Lexer::Type::ADD:
    case Lexer::Type::SUB:
        ret = 6;
        break;
    case Lexer::Type::MUL:
    case Lexer::Type::DIV:
    //mod
        ret = 7;
        break;
    case Lexer::Type::NOT:
        ret = 8;
        break;
    case Lexer::Type::INC:
    case Lexer::Type::DEINC:
    case Lexer::Type::F_CALL:
        ret = 9;
        break;

    default:
        return -10;
        break;

    }
    return ret;
}

//Compares the current token (token1) against the stack tokens (token2)
bool Lexer::comparePrecedence(std::pair<unsigned int, std::string> token1, std::pair<unsigned int, std::string> token2)
{

    int prec1 = Lexer::getPrecedence(token1);
    int prec2 = Lexer::getPrecedence(token2);

    if(prec1 == -10) throw [=](){std::cout << "Unknown Token in precedence comparison: " << token1.second << "\n"; return 17;}();
    if(prec2 == -10) throw [=](){std::cout << "Unknown Token in precedence comparison: " << token2.second << "\n"; return 17;}();

    return prec1 < prec2;
}

Lexer::expression Lexer::ToExpression(std::vector<std::pair<unsigned int, std::string> >&& postfix, Lexer::lexcon &l)
{
    //input a list of tokens in postfix notation, output an expression struct that links all other expressions in the list.
    Lexer::expression exp{};
    std::vector<Lexer::expression> exp_holder;

    for(auto it = postfix.begin(); it != postfix.end(); it++)
{

        switch((*it).first)
        {
        case Lexer::Type::INT:
            {
                exp_holder.push_back(Lexer::expression(std::stoi((*it).second)));
            }
            break;

        case Lexer::Type::STR:
            {
                exp_holder.push_back(Lexer::expression((*it).second));
            }
            break;

        case Lexer::Type::ID:
            {
                exp_holder.push_back(Lexer::expression(l.defvar((*it).second)));
            }
            break;

        case Lexer::Type::F_CALL:
            {
                if(exp_holder.size() < 1) throw [](){std::cout << "Error: Not enough expressions for function call" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                exp_holder.pop_back();

                exp = Lexer::expression(l.defun((*it).second));
                exp.type = Lexer::ex_type::fcall;
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::ADD:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for addition operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::add;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::SUB:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for subtraction operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::add;

                Lexer::expression sub;
                sub.type = Lexer::ex_type::neg;
                sub.params.push_back(param1);

                exp.params.push_back(param2);
                exp.params.push_back(sub);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::MUL:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for multiplication operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::mul;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::DIV:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for division operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::div;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::ASSIGN:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for subtraction assignment" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::cpy;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::AND:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for AND expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::cand;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }
            break;

        case Lexer::Type::OR:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for OR expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::cor;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            } break;

        case Lexer::Type::EQUAL:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for EQUAL expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::eq;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            } break;

        case Lexer::Type::PL_EQ:
            {

                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for PLUS_EQUAL expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::cpy;

                Lexer::expression add;
                add.type = Lexer::ex_type::add;
                add.params.push_back(param2);
                add.params.push_back(param1);

                exp.params.push_back(param2);
                exp.params.push_back(add);
                exp_holder.push_back(exp);
                exp = {};


            }break;

        case Lexer::Type::SUB_EQ:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for SUB_EQUAL expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::cpy;

                Lexer::expression add;
                Lexer::expression sub;
                sub.type = Lexer::ex_type::neg;
                add.type = Lexer::ex_type::add;
                sub.params.push_back(param1);
                add.params.push_back(param2);
                add.params.push_back(sub);

                exp.params.push_back(param2);
                exp.params.push_back(add);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::NONE:
            {
                exp_holder.push_back(Lexer::expression());
            }break;

        case Lexer::Type::NOT:
            {
                if(exp_holder.size() < 1) throw [](){std::cout << "Error: Not enough expressions for NOT expression" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                exp_holder.pop_back();

                exp = Lexer::expression();
                Lexer::expression zero = Lexer::expression(0);

                exp.type = Lexer::ex_type::eq;
                exp.params.push_back(param1);
                exp.params.push_back(zero);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::COM:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for comma operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::comma;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::GREATER:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for greater than operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::gr;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::LESS:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for less than operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::ls;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::GR_EQ:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for greater equals than operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::greq;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        case Lexer::Type::LESS_EQ:
            {
                if(exp_holder.size() < 2) throw [](){std::cout << "Error: Not enough expressions for less equal than operation" << "\n"; return 12;}();

                auto itr2 = exp_holder.rbegin();
                auto param1 = (*itr2);
                auto param2 = *(itr2+1);
                exp_holder.pop_back();
                exp_holder.pop_back();

                exp = Lexer::expression();
                exp.type = Lexer::ex_type::lseq;
                exp.params.push_back(param2);
                exp.params.push_back(param1);
                exp_holder.push_back(exp);
                exp = {};
            }break;

        }
    }
    if(exp_holder.size() > 1)
    {
        throw [](){std::cout << "Error: Expression mismatch!" << "\n"; return 12;}();
    }

    else if (exp_holder.size() == 0)
    {
        return Lexer::expression();
    }

    return exp_holder[0];

}

std::vector<Lexer::func> Lexer::parseTokens(std::string filename)
{
    lexcon l;
    l.tokenlist = Lexer::readTokenFile(filename);

    auto it = l.tokenlist.begin();

    while((*it).first != Lexer::Type::END)
    {
        Lexer::checkFunction(it, l);
    }

    return l.funclist;
}

std::string Lexer::readExprType(Lexer::expression& e)
{
    switch (e.type)
    {
    case Lexer::ex_type::add:
        return "+";
    case Lexer::ex_type::cand:
        return "and";
    case Lexer::ex_type::cif:
        return "if";
    case Lexer::ex_type::comma:
        return ",";
    case Lexer::ex_type::cor:
        return "or";
    case Lexer::ex_type::cpy:
        return "=";
    case Lexer::ex_type::div:
        return "/";
    case Lexer::ex_type::eq:
        return "==";
    case Lexer::ex_type::fcall:
        return e.ident.name;
    case Lexer::ex_type::gr:
        return ">";
    case Lexer::ex_type::greq:
        return ">=";
    case Lexer::ex_type::ident:
        return e.ident.name;
    case Lexer::ex_type::loop:
        return "while";
    case Lexer::ex_type::ls:
        return "<";
    case Lexer::ex_type::lseq:
        return "<=";
    case Lexer::ex_type::mul:
        return "*";
    case Lexer::ex_type::neg:
        return "-";
    case Lexer::ex_type::number:
        return "INT";
    case Lexer::ex_type::ret:
        return "return";
    case Lexer::ex_type::str:
        return "STRING";
    case Lexer::ex_type::nop:
        return "nop";
    }
}

std::string readExprParameters(Lexer::expression& e)
{
    switch (e.type)
    {
    case Lexer::ex_type::add:
        return "<" + readExprParameters(e.params.front()) + " + " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::cand:
        return "<" + readExprParameters(e.params.front()) + " and " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::cif:
        //std::cout << readExprType(e.params.back()) << " ";
        return "if <" + readExprParameters(e.params.front()) + "> then " + readExprParameters( e.params.back() );
    case Lexer::ex_type::comma:
        return "<" + readExprParameters(e.params.front()) + ", " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::cor:
        return "<" + readExprParameters(e.params.front()) + " or " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::cpy:
        return "<" + readExprParameters(e.params.front()) + " = " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::div:
        return "<" + readExprParameters(e.params.front()) + " / " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::eq:
        return "<" + readExprParameters(e.params.front()) + " == " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::fcall:
        return "<" + e.ident.name + " (" + readExprParameters(e.params.front()) + ")> ";
    case Lexer::ex_type::gr:
        return "<" + readExprParameters(e.params.front()) + " > " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::greq:
        return "<" + readExprParameters(e.params.front()) + " >= " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::ident:
        return e.ident.name;
    case Lexer::ex_type::loop:
        return "while " + readExprParameters(e.params.front()) + " do " + readExprParameters(e.params.back());
    case Lexer::ex_type::ls:
        return "<" + readExprParameters(e.params.front()) + " < " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::lseq:
        return "<" + readExprParameters(e.params.front()) + " <= " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::mul:
        return  "<" + readExprParameters(e.params.front()) + " * " + readExprParameters(e.params.back()) + "> ";
    case Lexer::ex_type::neg:
        return "<-" + readExprParameters(e.params.front()) + "> ";
    case Lexer::ex_type::number:
        return "INT";
    case Lexer::ex_type::ret:
        return "<return " + readExprParameters(e.params.front()) + "> ";
    case Lexer::ex_type::str:
        return "STRING";
    case Lexer::ex_type::nop:
        std::string ret;
        ret += "++<";
        for (auto b : e.params)
        {
            ret += readExprParameters(b);
            ret += " ";

            //std::cout << e.params.size() << "\n";
        }
        ret += ">++ ";
        return  ret;
    }
}

void Lexer::readFuncList(std::vector<Lexer::func> flist)
{
    int i = 0;
    std::cout << "\n";
    for(auto a: flist)
    {
        std::cout << a.name << "\n";
        std::cout << "Number of parameters: " << a.num_parameters << "\n";
        std::cout << "Number of variables: " << a.num_vars << "\n";
        for(auto b: a.code)
        {
            std::cout << "\n";
            if(i < a.num_parameters)
            {
                std::cout << readExprParameters(b);
            }
            else
            {
                //std::cout << readExprParameters(b);
                for(auto c: b.params)
                {
                    //std::cout << "num_params: " << c.params.size();
                    std::cout << readExprParameters(c) << "\n";
                }
            }

            //std::cout << i;
            i++;
        }
        std::cout << "\n";
    std::cout << "\n";
    }

}
