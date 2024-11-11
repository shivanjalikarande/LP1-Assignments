#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

class Assembler
{
    vector<pair<string,int>>symbolTbl;
    vector<pair<string,int>>literalTbl;
    ifstream fin, fLit, fSym;
    ofstream fout;
    public:
        Assembler()
        {
            fLit.open("literalTbl.txt");
            fSym.open("symbolTbl.txt");
            fin.open("input.txt");
            fout.open("machine.txt");
            if(fin.fail() || fSym.fail() || fLit.fail() || fout.fail())
            {
                cout<<"\nError while opening files"<<endl;
            }
            insertToTables();
        }

        void insertToTables()
        {
            //scanning symbol.txt & literal.txt

            //add to symbol table
            string line,symbol;
            int address;
            while(getline(fSym,line))
            {
                stringstream ss(line);
                ss >> symbol >> address;
                symbolTbl.push_back({symbol,address});
            }

            //add to literal table
            string literal;
            while(getline(fLit,line))
            {
                stringstream ss(line);
                ss >> literal >> address;
                literalTbl.push_back({literal,address});
            }
        }

        void execute()
        {
            string line,word;
            while(getline(fin,line))
            {
                string opcode,location,operand;
                stringstream ss(line);
                ss >> location >> opcode;
                
                 //There are 3 possibilities opcode -> IS, DL, AD
                // AD will not processed in pass2
                if(opcode.find("IS") != string::npos)
                {
                    string ISInd, regValue, symInd, litInd;
                    ss >> ISInd;       //opcode
                    fout<<location<<" "<<ISInd.substr(0,ISInd.size()-1);

                    while(ss >> word)
                    {
                        if(word == "(S,")
                        {
                            ss >> symInd;
                            int symIndex = stoi(symInd.substr(0,symInd.size()-1));
                            fout<<" "<<symbolTbl[symIndex-1].second;
                        }
                        else if(word == "(L,")
                        {
                            ss >> litInd;
                            int litIndex  = stoi(litInd.substr(0,litInd.size()-1));
                            fout<<" "<<literalTbl[litIndex-1].second;
                        }
                        else
                        {
                            //register value or anything value
                            fout<<" "<<word.substr(1,1);
                        }
                    }

                }
                else if(opcode.find("DL") != string::npos)
                {
                    string dl, ct;
                    ss >> dl;
                    // dl = dl.substr(0,dl.size()-1);
                    //2 possibilities -> DC, DS
                    if(dl == "01)")   //DC  -> process to only constant
                    {
                        ss >> word >> ct;
                        fout<<location<<" 00 "<<" 0 "<<ct.substr(0,ct.size()-1)<<endl;
                    }
                    else if(dl == "02)")   //DS -> no processing
                    {
                        fout<<location<<" - "<<" - "<<" - "<<endl;
                    }
                }
                fout<<"\n"<<endl;
            }
        }

        ~Assembler()
        {
            fLit.close();
            fSym.close();
            fin.close();
            fout.close();
        }
};

int main()
{
    Assembler m;
    m.execute();
}