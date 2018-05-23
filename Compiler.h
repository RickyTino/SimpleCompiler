#include<bits/stdc++.h>
#include<conio.h>
using namespace std;

/*--------------------Const--------------------*/

//Lexeme Analysis
const string rw_arr[] = {
	"if", "then", "else", "while", "do", "int", "float"
};

const string relop_arr[] = {
	">", "<", "==", ">=", "<=", "!="
};

const char opc[] = {
	'+', '-', '*', '/', '(', ')', ';', '\'', '>', '<', '=', '!'
};

const char relopc[] = {
	'>', '<', '=', '!'
};

const vector<string> reserveWord (begin(rw_arr), end(rw_arr));
const vector<string> relops (begin(relop_arr), end(relop_arr));


enum SymType {
	ID = 0, INTG, OP, ROP, RW
};

enum ResvWordType {
	IF = 0, THEN, ELSE, WHILE, DO, INT, FLOAT
};

enum RelOpType {
	GT = 0, LT, EQ, GTE, LTE, NE
};

//Parsing
const char EMP = '#'; //epsilon
const char END = '$'; //end of a sentence

enum ActionType {
	ERR = 0, MOV, RDC, ACC
};

enum ParsingType {
    SLR = 0, LR1
};

/*--------------------Type--------------------*/

class Symbol {
public:
    string lexeme;
    int type, value;

    Symbol(string l, int t, int v);
	bool operator == (Symbol s);
};

class Token {
public:
    int type;
    int property;

    Token(int t, int p);
};

class Production {
public:
    char left;
    string right;

    Production();
    Production(char l, string r);
    string lmDerive(string s);
    string lmReduct(string s);
    friend ostream& operator << (ostream& out, Production& p);
};

template <class T>
class Set {
public:
    vector<T> items;

    Set<T>();
    size_t size();
    size_t index(T t);
    bool exist(T t);
    bool push(T t);
    bool pop(T t);
    T& operator [](int i);
    Set<T>& operator += (Set<T> st);
    bool operator == (Set<T> st);
    bool operator != (Set<T> st);
    //friend ostream& operator << (ostream& out, Set<T>& st);
};

//typedef Set<char> CharSet;

class CharSet : public Set<char> {
public:
    friend ostream& operator << (ostream& out, CharSet& cs);
};


class LRItem : public Production {
public:
    int dot;
    CharSet exp;

    LRItem();
    LRItem(char l, string r, int d);
    LRItem(Production p, int d);
    LRItem(Production p, int d, char e);
    void addExp(char c);
    bool resemble(Production p);
    bool resemble(LRItem itm);
    bool operator == (LRItem itm);
    bool operator != (LRItem itm);
    friend ostream& operator << (ostream& out, LRItem& itm);
};

//typedef Set<LRItem> ItemSet;
class ItemSet : public Set<LRItem> {
public:
    bool push_exp(LRItem itm);
    friend ostream& operator << (ostream& out, ItemSet& its);
};

//typedef Set<ItemSet> Collection;
class Collection : public Set<ItemSet> {
public:
    friend ostream& operator << (ostream& out, Collection& c);
};


class Grammar {
public:
    vector<Production> prods;
    bool nTerm[128];
    char startsign;
    CharSet signset;
    CharSet FirstSet[128], FollowSet[128];

    Grammar();
    Grammar(char sts);
    void addProd(Production p);
    CharSet First(char c);
    CharSet First(string s);
    CharSet Follow(char c);
} ;

class ActionItem {
public:
	int act, val;

	ActionItem(int a, int v);
};

class ActionRow {
public:
    vector<ActionItem> row;

    ActionRow();
    ActionItem& operator [] (int i);
};

class ActionForm {
public:
    vector<ActionRow> acts;

    ActionForm();
    ActionForm(int sz);
    void setSize(int row);
    ActionRow& operator [] (int i);
};

class LRParser {
public:
	Grammar g;
	int mode; //0:LR(0)+SLR  1:LR(1)
	Collection col;
	ActionForm Action;
	vector<int> stkstate;
	vector<char> stksign;

	LRParser(int m, Grammar grm);
	ItemSet Closure(ItemSet its);
    ItemSet Goto(ItemSet its, char x);
    void initStacks();
    void setCollection();
    void setAction();
    bool Parse(string s, ostream& out);
    string outputAction();
};

/*--------------------Global Variables--------------------*/

extern vector<Token> tokenStream;
extern vector<Symbol> symTable;

/*--------------------Function--------------------*/

string outputTokens();
string outputSymTable();
bool LexAnl(string instr);
string tokenToStr();
