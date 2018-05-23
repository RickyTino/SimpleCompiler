#include "Compiler.h"

/*--------------------Class Functions--------------------*/

/*--------------------Production--------------------*/
Production::Production()
{

}

Production::Production(char l, string r)
{
    left = l;
    right = r;
}

string Production::lmDerive(string s)
{
    size_t found = s.find(left);
    if(found != string::npos){
        s.erase(found, 1);
        s.insert(found, right);
    }
    return s;
}

string Production::lmReduct(string s)
{
    size_t found = s.find(right);
    if(found != string::npos){
        s.erase(found, right.size());
        s.insert(s.begin()+found, left);
    }
    return s;
}

/*--------------------Template _Set--------------------*/

template <class T>
Set<T>::Set()
{

}

template <class T> size_t Set<T>::size()
{
    return items.size();
}

template <class T> size_t Set<T>::index(T t)
{
    for(int i=0; i<size(); i++){
        if(items[i] == t) return i;
    }
    push(t);
    return size()-1;
}

template <class T> bool Set<T>::exist(T t)
{
    for(int i=0; i<size(); i++){
        if(items[i] == t) return true;
    }
    return false;
}

template <class T> bool Set<T>::push(T t)
{
    if(!exist(t)){
        items.push_back(t);
        return true;
    }
    else return false;
}

template <class T> bool Set<T>::pop(T t)
{
    for(int i=0; i<size(); i++){
        if(items[i] == t){
            items.erase(items.begin() + i);
            return true;
        }
    }
    return false;
}

template <class T> T& Set<T>::operator [](int i)
{
    return items[i];
}

template <class T> Set<T>& Set<T>::operator += (Set<T> st)
{
    for(int i=0; i<st.size(); i++)
        push(st[i]);
}

template <class T> bool Set<T>::operator == (Set<T> st)
{
    if(size() != st.size()) return false;
    for(int i=0; i<size(); i++)
        if(!st.exist(items[i]) || !exist(st[i]))
            return false;
    return true;
}

template <class T> bool Set<T>::operator != (Set<T> st)
{
    return !(*this == st);
}

/*--------------------LRItem : Production--------------------*/

LRItem::LRItem() : Production()
{

}

LRItem::LRItem(char l, string r, int d) : Production(l, r)
{
    dot = d;
}

LRItem::LRItem(Production p, int d) : Production(p.left, p.right)
{
    dot = d;
}

LRItem::LRItem(Production p, int d, char e) : Production(p.left, p.right)
{
    dot = d;
    exp.push(e);
}

void LRItem::addExp(char c)
{
    exp.push(c);
}

bool LRItem::resemble(Production p)
{
    return (left == p.left && right == p.right);
}

bool LRItem::resemble(LRItem itm)
{
    return (left == itm.left && right == itm.right && dot == itm.dot);
}

bool LRItem::operator == (LRItem itm)
{
    if(left != itm.left) return false;
    if(right != itm.right) return false;
    if(dot != itm.dot) return false;
    if(exp != itm.exp) return false;
    return true;
}

bool LRItem::operator != (LRItem itm)
{
    return !(*this == itm);
}

/*--------------------ItemSet : Set<LRItem>--------------------*/

bool ItemSet::push_exp(LRItem itm)
{
    for(int i=0; i<size(); i++){
        if(items[i].resemble(itm)){
            items[i].exp += itm.exp;
            return true;
        }
    }
    push(itm);
}

/*--------------------Grammar--------------------*/

Grammar::Grammar()
{
    for(int i=0; i<128; i++){
    	if(i >= 'A' && i <= 'Z') nTerm[i] = true;
        else nTerm[i] = false;
	}

}

Grammar::Grammar(char sts) : Grammar()
{
    startsign = sts;
    signset.push(startsign);
    signset.push(END);
}

void Grammar::addProd(Production p)
{
    signset.push(p.left);
    for(int i=0; i<p.right.size(); i++) signset.push(p.right[i]);
    prods.push_back(p);
}

CharSet Grammar::First(char c)
{
    CharSet res;
    if(FirstSet[c].size() != 0) return FirstSet[c];
    if(!nTerm[c]) res.push(c);
    else{
        for(int i=0; i<prods.size(); i++){
            if(prods[i].left != c) continue;
            string r = prods[i].right;
            if(r.size() == 0) res.push(EMP);
            else{
                CharSet t;
                for(int i=0; i<r.size(); i++){
                    if(r[i] == c) break;
                    t = First(r[i]);
                    res += t;
                    if(!t.exist(EMP)) break;
                }
            }
        }
    }
    FirstSet[c] = res;
    return res;
}

CharSet Grammar::First(string s)
{
    CharSet res, t;
    for(int i=0; i<s.size(); i++){
        t = First(s[i]);
        res += t;
        if(!t.exist(EMP)) break;
    }
    return res;
}

CharSet Grammar::Follow(char c)
{
	CharSet res;

	if(!nTerm[c]) return res;
	if(FollowSet[c].size() != 0) return FollowSet[c];
	if(c == startsign) res.push(END);

	for(int i=0; i<prods.size(); i++){
		string r = prods[i].right;
		char l = prods[i].left;
		size_t pos = r.find(c);
		if(pos == string::npos) continue;
		string beta = r.substr(pos + 1);
		if(beta.size() != 0){
			CharSet firsts = First(beta);
			res += firsts;
			if(firsts.exist(EMP) && c != l)
				res += Follow(l);
		}
		else if(c != l) res += Follow(l);
	}
	FollowSet[c] = res;
	return res;
}

/*--------------------Action--------------------*/
/*
ActionItem::ActionItem()
{
    act = ERR;
    val = 0;
}
*/
ActionItem::ActionItem(int a = ERR, int v = 0)
{
	act = a;
	val = v;
}

ActionRow::ActionRow()
{
    row.resize(128);
}

ActionItem& ActionRow::operator [] (int i)
{
    return row[i];
}

ActionForm::ActionForm()
{

}

ActionForm::ActionForm(int sz)
{
    setSize(sz);
}

void ActionForm::setSize(int row)
{
    acts.resize(row);
}

ActionRow& ActionForm::operator [] (int i)
{
    return acts[i];
}

/*--------------------LRParser--------------------*/

LRParser::LRParser(int m, Grammar grm)
{
    mode = m;
	g = grm;
}

ItemSet LRParser::Closure(ItemSet its)
{
	for(int i=0; i<its.size(); i++){
        if(its[i].dot >= its[i].right.size()) continue;
        LRItem itm = its[i];
        string beta = itm.right.substr(itm.dot+1);
        char next = itm.right[itm.dot];
        for(int j=0; j<g.prods.size(); j++){
            if(g.prods[j].left != next) continue;
            LRItem res(g.prods[j], 0);
            if(mode == LR1){
                for(int k=0; k<itm.exp.size(); k++){
                    string betaa = beta;
                    betaa.push_back(itm.exp[k]);
                    res.exp += g.First(betaa);
                }
                its.push_exp(res);
            }
            else{
                its.push(res);
            }
        }
    }
    return its;
}

ItemSet LRParser::Goto(ItemSet its, char x)
{
	ItemSet res;
    LRItem t;

    for(int i=0; i<its.size(); i++){
        t = its[i];
        if(t.dot < t.right.size()){
        	if(t.right[t.dot] != x) continue;
            ++t.dot;
            res.push(t);
        }
    }
    return Closure(res);
}

void LRParser::initStacks()
{
    while(stkstate.size()) stkstate.pop_back();
    while(stksign.size()) stksign.pop_back();
    stkstate.push_back(0);
}

void LRParser::setCollection()
{
	ItemSet its, goits;
    char x;

    if(mode == LR1)
        its.push(LRItem(g.prods[0], 0, END));
    else
        its.push(LRItem(g.prods[0], 0));
    col.push(Closure(its));
    for(int i=0; i<col.size(); i++){
        its = col[i];
        for(int j=0; j<g.signset.size(); j++){
            x = g.signset[j];
            goits = Goto(its, x);
            if(goits.size() != 0) col.push(goits);
        }
    }
}

void LRParser::setAction()
{
    setCollection();
    Action.setSize(col.size());
    for(int i=0; i<col.size(); i++){
        for(int j=0; j<col[i].size(); j++){
            LRItem itm = col[i][j];
            if(itm.dot < itm.right.size()){
                char next = itm.right[itm.dot];
                int tostate = col.index( Goto(col[i], next) );
                Action[i][next].act = MOV;
                Action[i][next].val = tostate;
            }
            else{
                if(itm.left == g.startsign){
                    Action[i][END] = ACC;
                }
                else{
                    int pos = 0;
                    for(pos=0; pos<g.prods.size(); pos++){
                        if(itm.resemble(g.prods[pos])) break;
                    }
                    CharSet follows;
                    if(mode == LR1) follows = itm.exp;
                    else follows = g.Follow(itm.left);
                    for(int k=0; k<follows.size(); k++){
                        char a = follows[k];
                        Action[i][a].act = RDC;
                        Action[i][a].val = pos;
                    }
                }
            }
        }
    }
}

bool LRParser::Parse(string s, ostream& out)
{
    int i = 0;
    int state;
    initStacks();

    while(1){
        state = stkstate.back();

        out << "State:";
        for(int j=0; j<stkstate.size(); j++)
            out << stkstate[j] << " ";
        out << endl << "Sign:";
        for(int j=0; j<stksign.size(); j++)
            out << stksign[j] << " ";
        out << endl;
        out << "Left:" << s.substr(i) << endl;

        if(Action[state][s[i]].act == MOV){
            int t = Action[state][s[i]].val;
            stkstate.push_back(t);
            stksign.push_back(s[i]);
            out << "Move in " << s[i] << endl;
            ++i;
        }
        else if(Action[state][s[i]].act == RDC){
            int t = Action[state][s[i]].val;
            Production p = g.prods[t];
            for(int j=0; j<p.right.size(); j++){
                stkstate.pop_back();
                stksign.pop_back();
            }
            state = stkstate.back();
            stkstate.push_back(Action[state][p.left].val);
            stksign.push_back(p.left);
            out << "Reduct with " << p << endl;
        }
        else if(Action[state][s[i]].act == ACC){
            out << "Accepted." << endl;
            return true;
        }
        else{
            out << "Error." << endl;
            break;
        }

        out << endl;

    }
    return false;
}

string LRParser::outputAction()
{
    stringstream ss;
    ss << "\t";
    for(int j=1; j<g.signset.size(); j++){
        char k = g.signset[j];
        if(g.nTerm[k]) continue;
        ss << k << "\t";
    }
    for(int j=1; j<g.signset.size(); j++){
        char k = g.signset[j];
        if(!g.nTerm[k]) continue;
        ss << k << "\t";
    }
    ss << endl;

    for(int i=0; i<col.size(); i++){
        ss << i << ":\t";
        for(int j=1; j<g.signset.size(); j++){
            char k = g.signset[j];
            if(g.nTerm[k]) continue;
            if(Action[i][k].act == ERR)
                ss << "\t";
            else if(Action[i][k].act == MOV)
                ss << "s" << Action[i][k].val << "\t";
            else if(Action[i][k].act == RDC)
                ss << "r" << Action[i][k].val << "\t";
            else if(Action[i][k].act == ACC)
                ss << "acc\t";
        }
        for(int j=1; j<g.signset.size(); j++){
            char k = g.signset[j];
            if(!g.nTerm[k]) continue;
            if(Action[i][k].act == ERR)
                ss << "\t";
            else if(Action[i][k].act == MOV)
                ss << Action[i][k].val << "\t";
        }
        ss << endl;
    }

    ss << endl;
    return ss.str();
}

/*--------------------Override Operators--------------------*/

ostream& operator << (ostream& out, CharSet& cs)
{
    for(int i=0; i<cs.size(); i++){
        out << cs[i] << " ";
    }
    return out;
}

ostream& operator << (ostream& out, Production& p)
{
    out << p.left << " -> " << p.right;
    return out;
}

ostream& operator << (ostream& out, LRItem& itm)
{
    string r = itm.right;
    r.insert(itm.dot, 1, '.');
    out << itm.left << " -> " << r; // << " , " << itm.expc;
    if(itm.exp.size() != 0){
        out << " , " << itm.exp;
    }
    return out;
}

ostream& operator << (ostream& out, ItemSet& its)
{
    for(int i=0; i<its.size(); i++){
        out << its[i] << endl;
    }
    return out;
}

ostream& operator << (ostream& out, Collection& c)
{
    for(int i=0; i<c.size(); i++){
        out << "I" << i << ":" << endl;
        out << c[i] << endl;
    }
    out << endl;
    return out;
}
