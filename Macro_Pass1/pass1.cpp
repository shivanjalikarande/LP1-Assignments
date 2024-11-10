#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
using namespace std;

//Data Structure: MDT, MNT, KPTAB, PNTAB  
class Macro
{
    vector<vector<string>>MyToken;
    vector<vector<string>>MDT;
    vector<tuple<string,int,int,int,int>>MNT;
    vector<pair<string,string>>KPTAB;
    vector<vector<pair<int,string>>>PNTAB;

    public:
    Macro()
    {
        ifstream fin;
        fin.open("input.txt");
        if(fin.fail())
        {
            cout<<"\nError in opening file"<<endl;
            exit(1);
        }
        cout<<"\nFile opened successfully"<<endl;
        string line,word;
        while(getline(fin,line))
        {
            stringstream ss(line);
            vector<string>token;
            while(ss >> word)
            {
                token.push_back(word);
            }
            MyToken.push_back(token);
        }
    }
    void displayInput()
    {
        for(int i=0;i<MyToken.size();i++)
        {
            for(int j=0;j<MyToken[i].size();j++)
            {
                cout<<MyToken[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    void tokenize()
    {
        for(int i=0;i<MyToken.size();i++)
        {
            if(MyToken[i][0] == "MACRO")
            {
                i = processInput(i+1);
            }
        }
    }

    int searchIfParam(string para)
    {
        for(int i=0;i<PNTAB.size();i++)
        {
            for(int j=0;j<PNTAB[i].size();j++)
            {
                if(PNTAB[i][j].second == para.substr(0,para.size()-1))
                {
                    return j;
                }
            }
        }
        return -1;
    }

    int processInput(int i)
    {
        //store maro name
        string mname = MyToken[i][0];

        //first line of macro def to store parameters into PNTAB, KPTAB --> to update MNT, KPTAB, PPTAB
        int kptabp = KPTAB.size();
        int mdtp = MDT.size();
        int cntP, cntKP;
        cntP = cntKP = 0;
        string kpname, pname, value;
        vector<pair<int,string>>temp;
        int ptabInd = 1;
        for(int j=1;j<MyToken[i].size();j++)
        {
            if(MyToken[i][j].find("=") != string::npos)
            {
                int ind = MyToken[i][j].find("=");
                kpname  =  MyToken[i][j].substr(0,ind);
                value = MyToken[i][j].substr(ind+1,MyToken[i][j].size()-ind-2);
                KPTAB.push_back({kpname,value});
                cntKP++;
                temp.push_back({ptabInd,kpname});
                ptabInd++;
            }
            else
            {
                pname = MyToken[i][j].substr(0,MyToken[i][j].size()-1);
                temp.push_back({ptabInd,pname});
                ptabInd++;
                cntP++;
            }
            cout<<"\n"<<endl;
        }
        PNTAB.push_back(temp);
        //add to MNT
        MNT.emplace_back(mname,cntP,cntKP,mdtp,kptabp);

        //remaining definition --> to add to MDT
        int j;
        for(j=i+1;j<MyToken.size() && MyToken[j][0]!="MEND";j++)
        {
            vector<string>mdtLine;  //line to add to mdt
            for(int k=0;k<MyToken[j].size();k++)
            {
                if(searchIfParam(MyToken[j][k]) != -1)
                {
                    int idx = searchIfParam(MyToken[j][k]);
                    string str = "(P," + to_string(idx+1) + ")";
                    mdtLine.push_back(str);
                }
                else
                {
                    mdtLine.push_back(MyToken[j][k]);
                }
            }
            MDT.push_back(mdtLine);
        }
        vector<string>mdtLine;
        mdtLine.push_back("MEND");
        MDT.push_back(mdtLine);
        return j;
    }

    void displayMDT()
    {
        cout<<"\n ------ MDT ----"<<endl;
        for(int i=0;i<MDT.size();i++)
        {
            for(int j=0;j<MDT[i].size();j++)
            {
                cout<<MDT[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    void displayMNT()
    {
        cout<<"\n----- MNT ----"<<endl;
        for(tuple<string,int,int,int,int>t : MNT)
        {
            cout<<get<0>(t)<<" "<<get<1>(t)<<" "<<get<2>(t)<<" "<<get<3>(t)<<" "<<get<4>(t)<<endl; 
        }
    }

    void displayKPTAB()
    {
        cout<<"\n ----------- KPTAB ---------"<<endl;
        for(int i=0;i<KPTAB.size();i++)
        {
            cout<<KPTAB[i].first<<" "<<KPTAB[i].second<<endl;
        }
    }

    void displayPNTAB()
    {
        cout<<"-------- PNTAB --------"<<endl;
        for(int i=0;i<PNTAB.size();i++)
        {
            cout<<"\nPNTAB for Macro "<<(i+1)<<endl;
            for(int j=0;j<PNTAB[i].size();j++)
            {
                cout<<PNTAB[i][j].first<<" "<<PNTAB[i][j].second<<endl;
            }
        }
    }

};

int main()
{
    Macro m;
    cout<<"\nMacro Pass1"<<endl;
    m.displayInput();
    m.tokenize();

    m.displayPNTAB();
    m.displayKPTAB();
    m.displayMNT();
    m.displayMDT();
}