#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

class Assembler
{
    map<string, pair<string, string>> opcodeTbl;
    map<string, pair<int, string>> symbolTbl;
    vector<pair<string, int>> literalTbl;
    vector<string> poolTbl;
    int litInd = 0;
    int lc = -1;
    ifstream fin;
    ofstream fout;

public:
    Assembler()
    {
        fin.open("input.txt");
        fout.open("intermediateCode.txt");
        if (fin.fail() || fout.fail())
        {
            cout << "\nError while opening file" << endl;
            exit(1);
        }
    }

    void initializeOpcodeTbl()
    {
        opcodeTbl =
            {
                {"START", {"AD", "00"}},
                {"END", {"AD", "01"}},
                {"ORIGIN", {"AD", "03"}},
                {"EQU", {"AD", "04"}},
                {"LTORG", {"AD", "05"}},
                {"STOP", {"IS", "00"}},
                {"ADD", {"IS", "01"}},
                {"SUB", {"IS", "02"}},
                {"MULT", {"IS", "03"}},
                {"MOVER", {"IS", "04"}},
                {"MOVEM", {"IS", "05"}},
                {"COMP", {"IS", "06"}},
                {"BC", {"IS", "07"}},
                {"DIV", {"IS", "08"}},
                {"READ", {"IS", "09"}},
                {"PRINT", {"IS", "10"}},
                {"DC", {"DL", "01"}}, 
                {"DS", {"DL", "02"}}, 
                {"AREG", {"1", ""}},
                {"BREG", {"2", ""}},
                {"CREG", {"3", ""}},
                {"DREG", {"4", ""}},
                {"LT", {"1", ""}},
                {"LE", {"2", ""}},
                {"EQ", {"3", ""}},
                {"GT", {"4", ""}},
                {"GE", {"5", ""}},
                {"ANY", {"6", ""}}};
    }

    void execute()
    {
        initializeOpcodeTbl();

        // scan the code line by line
        string line, word;
        while (getline(fin, line))
        {
            stringstream ss(line);
            while (ss >> word)
            {
                // word = label field / opcode
                // 1. if label -> put to symbol table
                string label = "";
                if (opcodeTbl.count(word) == 0)
                {
                    if (symbolTbl.count(word) == 0)
                    {
                        symbolTbl[word] = {lc, to_string(symbolTbl.size() + 1)};
                    }
                    else
                    {
                        symbolTbl[word].first = lc;
                    }
                    label = word;
                    ss >> word;
                }
                string operation = word;

                // 2. if opcode -> decide which type of opcode & perform tasks accordingly
                if (operation == "START")
                {
                    fout << "\t(" << opcodeTbl[operation].first << "," << opcodeTbl[operation].second << ")";
                    ss >> word;
                    lc = stoi(word);
                    fout << " (C," << word << ")" << endl;
                }
                else if (operation == "END" || operation == "LTORG")
                {
                    if (operation == "END")
                    {
                        fout << "\t(" << opcodeTbl[operation].first << "," << opcodeTbl[operation].second << ")" << endl; // will not print if LTORG
                    }
                    poolTbl.push_back("#" + to_string(litInd + 1)); // add entry to pool tbl
                    // process for literals that was previously added to literal table
                    while (litInd < literalTbl.size())
                    {
                        fout << lc << "\t" << "(DL,01)";
                        literalTbl[litInd].second = lc;
                        string literal = literalTbl[litInd].first;
                        string sublit = literal.substr(2, literal.length() - 3);
                        fout << "\t" << "(C," << sublit << ")" << endl;
                        litInd++;
                        lc++;
                    }
                }

                else if (operation == "EQU")
                {
                    // no any output to IC, no lc updation as AD, Only changes in symbol table
                    // assigns LC value to label field equal to calculation of operand field
                    ss >> word;
                    int signInd = 0;
                    for (int i = 0; i < word.size(); i++)
                    {
                        if (word[i] == '+' || word[i] == '-')
                        {
                            signInd = i;
                        }
                    }
                    string symbolName, offset;
                    int symLC, newSymLC;
                    if (signInd != 0)
                    {
                        // eg. LOOP+2 -> symbolName+offset
                        symbolName = word.substr(0, signInd);
                        offset = word.substr(signInd + 1);
                        symLC = symbolTbl[symbolName].first;
                        if (word[signInd] == '+')
                        {
                            newSymLC = symLC + stoi(offset);
                        }
                        else
                        {
                            newSymLC = symLC - stoi(offset);
                        }
                    }
                    else
                    {
                        // eg. LOOP ->symbolName+0
                        symbolName = word.substr(0);
                        symLC = symbolTbl[symbolName].first;
                        newSymLC = symLC;
                    }
                    symbolTbl[label] = {newSymLC, to_string(symbolTbl.size() + 1)};
                }

                else if (operation == "ORIGIN")
                {
                    // LC updation to value mentioned in terms operand , no lc processing as AD
                    // eg. 207 ORIGIN LOOP+2  -> value of lc updated to valueOfLC(LOOP) + 2

                    fout << "\t(" << opcodeTbl[operation].first << "," << opcodeTbl[operation].second << ")";
                    ss >> word;
                    int signInd = 0;
                    for (int i = 0; i < word.size(); i++)
                    {
                        if (word[i] == '+' || word[i] == '-')
                        {
                            signInd = i;
                        }
                    }
                    string symName, offset;
                    int symLC, updateLC;
                    if (signInd != 0)
                    {
                        symName = word.substr(0, signInd);
                        offset = word.substr(signInd + 1);
                        symLC = symbolTbl[symName].first;
                        if (word[signInd] == '+')
                        {
                            updateLC = symLC + stoi(offset);
                        }
                        else
                        {
                            updateLC = symLC - stoi(offset);
                        }
                        fout << "\t" << "(S," << symbolTbl[symName].second << ")" << word.substr(signInd) << endl;
                    }
                    else
                    {
                        symName = word.substr(0);
                        updateLC = symbolTbl[symName].first;
                        fout << "\t" << "(S," << symbolTbl[symName].second << ")" << endl;
                    }
                    lc = updateLC;
                }

                // Other than symbol and AD
                else
                {
                    fout << lc << "\t(" << opcodeTbl[word].first << "," << opcodeTbl[word].second << ")";
                    while (ss >> word)
                    {
                        if (operation == "DC")
                        {
                            fout << "\t" << "(C," << word << ")";
                        }

                        else if (operation == "DS")
                        {
                            fout << "\t" << "(C," << word << ")";
                            lc += stoi(word) - 1;
                        }
                        else if (word[0] == '=')
                        {
                            literalTbl.push_back({word, lc});
                            fout << "(L," << literalTbl.size() << ")";
                        }
                        else if (opcodeTbl.count(word) != 0)
                        {
                            fout << "(" << opcodeTbl[word].first << ")";
                        }
                        else
                        {
                            if (symbolTbl.count(word) == 0)
                            {
                                symbolTbl[word] = {lc, to_string(symbolTbl.size() + 1)};
                            }
                            fout << "(S," << symbolTbl[word].second << ")";
                        }
                    }
                    lc++;
                }
            }
            fout << endl;
        }
    }

    void displaySymbolTbl()
    {
        cout<<"---- SYMBOL TABLE ----"<<endl;
        for(auto itr=symbolTbl.begin(); itr!= symbolTbl.end(); itr++)
        {
            cout<<itr->second.second<<"\t"<<itr->first<<" "<<itr->second.first<<endl;
        }
    }
    void displayPoolTbl()
    {
        cout<<"---- POOL TABLE ----"<<endl;
        for(int i=0;i<poolTbl.size();i++)
        {
            cout<<poolTbl[i]<<endl;
        }
    }
    void displayLiteralTbl()
    {
        cout<<"---- LITERAL TABLE ----"<<endl;
        for(auto itr=literalTbl.begin(); itr!= literalTbl.end(); itr++)
        {
            cout<<itr->first<<" "<<itr->second<<endl;
        }
    }

};

int main()
{
    Assembler asmb;
    asmb.execute();
    asmb.displaySymbolTbl();
    asmb.displayLiteralTbl();
    asmb.displayPoolTbl();
}