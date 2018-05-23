#include "Compiler.h"

ifstream inf;
ofstream outf;

int main()
{
    string filename;

    cout << "Open file:";
    cin >> filename;
    inf.open(filename, ios::in);
	if(!inf.is_open()){
        cout << "Unable to open the input file. " << endl;
        cout << "Press any key to continue.." << endl;
        getch();
        return 0;
	}
	outf.open("out.txt", ios::out);
	if(!outf.is_open()){
        cout << "Unable to open the output file. " << endl;
        cout << "Press any key to continue.." << endl;
        getch();
        return 0;
	}

	string buf;
	stringstream temp;
	temp << inf.rdbuf();
	buf = temp.str();

    if(LexAnl(buf)){
        outf << "Lexeme analysis successfully completed." << endl << endl
             << "Tokens:" << endl << outputTokens() << endl
             << "Symbol table:" << endl << outputSymTable() << endl;
	}
	else{
        outf << "Lexeme analysis failed." << endl;
	}

	string sentence = tokenToStr();
	outf << sentence << endl << endl;

	//string sentence = "zi;zi;zi;i=n;i=n;w(ion)dr(ioi)ti=i+i;ei=i-i;$";

	Grammar g('Z');
	g.addProd(Production('Z',"P"));
	g.addProd(Production('P',"DS"));
	g.addProd(Production('D',"Li;D"));
	g.addProd(Production('D',""));
	g.addProd(Production('L',"z"));
	g.addProd(Production('L',"f"));
	g.addProd(Production('S',"i=E"));
	g.addProd(Production('S',"r(C)S"));
	g.addProd(Production('S',"r(C)SeS"));
	g.addProd(Production('S',"w(C)S"));
	g.addProd(Production('S',"S;S"));
	g.addProd(Production('S',"S;"));
	g.addProd(Production('C',"EoE"));
	g.addProd(Production('E',"E+T"));
	g.addProd(Production('E',"E-T"));
    g.addProd(Production('E',"T"));
    g.addProd(Production('T',"T*F"));
    g.addProd(Production('T',"T/F"));
    g.addProd(Production('T',"F"));
    g.addProd(Production('F',"(E)"));
    g.addProd(Production('F',"i"));
    g.addProd(Production('F',"n"));

    //建立LR语法分析器
    LRParser lrp(LR1, g);
    lrp.setCollection();
    lrp.setAction();
    //输出LR0项集族
    outf << lrp.col << endl;

    //输出文法的FIRST和FOLLOW集
    for(int i=0; i<g.signset.size(); i++){
        char ch = g.signset[i];
        CharSet fi, fo;
        fi = g.First(ch);
        fo = g.Follow(ch);
        outf << ch << ": FIRST = { ";
        for(int j=0; j<fi.size(); j++)
            outf << fi[j] << " ";
        outf << "}, FOLLOW = { ";
        for(int j=0; j<fo.size(); j++)
            outf << fo[j] << " ";
        outf << "}; " << endl;
    }

    outf << endl;

    //打ACTION和GOTO表
    outf << lrp.outputAction() << endl;

    //语法分析和结果反馈
    if(lrp.Parse(sentence, outf)){
        cout << "Analysis successful." << endl;
    }
    else{
        cout << "Analysis failed." << endl;
    }
    cout << "Report generated in out.txt" << endl << endl;
    cout << "Press any key to continue.." << endl;
    getch();
/*
Z -> P
P -> DS
D -> LI;D | emp
L -> z | f

S -> i=E | r(C)S | r(C)SeS | w(C)SdS | S;S
C -> EoE
E -> E+T | E-T | T
T -> T*F | T/F | F
F -> (E) | i | n
*/
/*
    Grammar g('S');

    g.addProd(Production('S',"A"));
    g.addProd(Production('A',"Ab"));
    g.addProd(Production('A',"bBa"));
    g.addProd(Production('B',"aAc"));
    g.addProd(Production('B',"aAb"));
    g.addProd(Production('B',"a"));


    g.addProd(Production('S',"E"));
    g.addProd(Production('E',"E+T"));
    g.addProd(Production('E',"T"));
    g.addProd(Production('T',"T*F"));
    g.addProd(Production('T',"F"));
    g.addProd(Production('F',"(E)"));
    g.addProd(Production('F',"i"));


    g.addProd(Production('S',"R"));
    g.addProd(Production('R',"CC"));
    g.addProd(Production('C',"cC"));
    g.addProd(Production('C',"d"));

    //建立LR语法分析器
    LRParser lrp(SLR, g);
    lrp.setCollection();
    lrp.setAction();
    //输出LR0项集族
    outf << lrp.col << endl;

    //输出文法的FIRST和FOLLOW集
    for(int i=0; i<g.signset.size(); i++){
        char ch = g.signset[i];
        CharSet fi, fo;
        fi = g.First(ch);
        fo = g.Follow(ch);
        outf << ch << ": FIRST = { ";
        for(int j=0; j<fi.size(); j++)
            outf << fi[j] << " ";
        outf << "}, FOLLOW = { ";
        for(int j=0; j<fo.size(); j++)
            outf << fo[j] << " ";
        outf << "}; " << endl;
    }

    outf << endl;

    //打ACTION和GOTO表
    lrp.outputAction();
    outf << endl;

    while(1){
        string str;
        cin >> str;
        bool flag = lrp.Parse(str);
        outf << endl;
    }
*/
    return 0;
}
