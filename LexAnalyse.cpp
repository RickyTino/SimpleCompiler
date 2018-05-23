#include "Compiler.h"

/*--------------------Var--------------------*/

vector<Token> tokenStream;
vector<Symbol> symTable;

/*--------------------Class Functions--------------------*/

Symbol::Symbol(string l, int t, int v){
    lexeme = l;
    type = t;
    value = v;
}

bool Symbol::operator == (Symbol s)
{
	if(lexeme != s.lexeme) return false;
	if(type != s.type) return false;
	if(value != s.value) return false;
	return true;
}

Token::Token(int t, int p){
    type = t;
    property = p;
}

/*--------------------Functions--------------------*/

bool isNum(char c)
{
    return (c >= '0' && c <= '9');
}

bool isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isOp(char c)
{
    bool flag = false;
    for(int i=0; i<12; i++){
        if(c == opc[i]){
            flag = true;
            break;
        }
    }
    return flag;
}

bool isRelOp(char c)
{
    bool flag = false;
    for(int i=0; i<4; i++){
        if(c == relopc[i]){
            flag = true;
            break;
        }
    }
    return flag;
}

int strtoint(string s)
{
    int n = 0;
    for(int i=0; i<s.size(); i++){
    	n *= 10;
        n += (int)(s[i] - '0');
    }
    return n;
}

int findSym(string lex)
{
    bool flag = false;
    int i;
    for(i=0; i<symTable.size(); i++){
        if(symTable[i].lexeme == lex){
            flag = true;
            break;
        }
    }
    if(flag) return i;
    else return -1;
}

int getSymAddr(string lex, int t, int v)
{
    int psym = findSym(lex);
    if(psym != -1) return psym;
    else{
        symTable.push_back( Symbol(lex, t, v) );
        return symTable.size() - 1;
    }
}

bool LexAnl(string s)
{
	string lexs;
	char c, d;
	int lexbgn = 0, nextlex = 1;

	while(lexbgn < s.size()){
		c = s[lexbgn];
        nextlex = lexbgn + 1;
		//处理注释
		if(c == '/'){
			d = s[lexbgn + 1];
			if(d == '/'){
				nextlex ++;
                while(s[nextlex] != '\n' && nextlex < s.size())
					++nextlex;
			}
			else if(d == '*'){
				nextlex += 3;
                while((s[nextlex - 2] != '*' || s[nextlex - 1] != '/') && nextlex < s.size())
					++nextlex;
			}
			else{ //它只是个除号，为什么要受这种委屈
                tokenStream.push_back(Token(OP, '/'));
            }
		}

		//处理关系运算符
		else if(isRelOp(c)){
			d = s[nextlex];
			if(d == '='){
				nextlex++;
				lexs = s.substr(lexbgn, 2);
			}
			else lexs = s.substr(lexbgn, 1);
			int i;
			bool flag = false;
			for(i=0; i<relops.size(); i++){
				if(relops[i] == lexs){
                    flag = true;
                    break;
				}
			}
			if(flag) tokenStream.push_back(Token(ROP, i));
			else tokenStream.push_back(Token(OP, c));
		}

		//处理运算符
		else if(isOp(c)){
            tokenStream.push_back(Token(OP, c));
		}

		 //处理整数
        else if( isNum(c) ){
            while(isNum(s[nextlex]) && nextlex < s.size())
				++nextlex;
            lexs = s.substr(lexbgn, nextlex - lexbgn);
            tokenStream.push_back(Token(INTG, getSymAddr(lexs, INTG, strtoint(lexs))));
        }

		//处理标识符和关键字
        else if( isLetter(c) ){
            //预处理关键字
            while(isLetter(s[nextlex]) && nextlex < s.size())
				++nextlex;
            lexs = s.substr(lexbgn, nextlex - lexbgn);
			bool isRW = false;
            for(int i=0; i<reserveWord.size(); i++){
				if(lexs == reserveWord[i]){
					tokenStream.push_back(Token(RW, i));
					isRW = true;
					break;
				}
			}
			//处理标识符
			if(!isRW){
				while((isLetter(s[nextlex]) || isNum(s[nextlex])) && nextlex < s.size())
					++nextlex;
                lexs = s.substr(lexbgn, nextlex - lexbgn);
                tokenStream.push_back(Token(ID, getSymAddr(lexs, ID, 0)));
			}
        }

		//其余字符跳过
        lexbgn = nextlex;
	}
	return true;
}


string outputTokens()
{
    stringstream ss;
    for(int i=0; i<tokenStream.size(); i++){
        ss << tokenStream[i].type << " " << tokenStream[i].property << endl;
    }
    return ss.str();
}

string outputSymTable()
{
    stringstream ss;
    for(int i=0; i<symTable.size(); i++){
        ss << symTable[i].lexeme << ": " << symTable[i].type << " "
           << symTable[i].value << endl;
    }
    return ss.str();
}

/* Char representing tokens:
rop     o
op      ASCII(op)
id	    i
intg    n

if	    r
then	t
else	e
while	w
do	    d
int	    z
float	f
*/

string tokenToStr()
{
    string res;
    for(int i=0; i<tokenStream.size(); i++){
        int type = tokenStream[i].type;
        int val = tokenStream[i].property;
        char c;
        switch(type){
            case ID:   c = 'i'; break;
            case INTG: c = 'n'; break;
            case OP:   c = val; break;
            case ROP:  c = 'o'; break;
            case RW:
                switch(val){
                    case IF:    c = 'r'; break;
                    case THEN:  c = 't'; break;
                    case ELSE:  c = 'e'; break;
                    case WHILE: c = 'w'; break;
                    case DO:    c = 'd'; break;
                    case INT:   c = 'z'; break;
                    case FLOAT: c = 'f'; break;
                    default:;
                };
            default:;
        };
        res.push_back(c);
    }
    res.push_back(END);
    return res;
}
