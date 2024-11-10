#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
using namespace std;

class Macro
{
    vector<vector<string>> MDT;
    vector<vector<string>> PNTAB;
    vector<tuple<string, int, int, int, int>> MNT;
    vector<pair<string, string>> KPDTAB;

    ifstream mnt, mdt, kt, pt, fin;
    ofstream fout;

public:
    Macro()
    {
        mnt.open("MNT.txt");
        mdt.open("MDT.txt");
        kt.open("KPDTAB.txt");
        pt.open("PNTAB.txt");
        fin.open("input.txt");

        fout.open("output.txt");
        if (mnt.fail() || mnt.fail() || kt.fail() || pt.fail() || fin.fail() || fout.fail())
        {
            cout << "\nError in opening file" << endl;
            exit(1);
        }
        readMNT();
        readMDT();
        readKPDTAB();
        readPNTAB();
    }

    // reading data from all input files & store into data structures -> MNT,MDT,PNTAB,KPDTAB

    void readMNT()
    {
        string line, word;
        while (getline(mnt, line))
        {
            stringstream ss(line);
            tuple<string, int, int, int, int> temp;
            ss >> get<0>(temp) >> get<1>(temp) >> get<2>(temp) >> get<3>(temp) >> get<4>(temp);
            MNT.push_back(temp);
        }
    }
    void readMDT()
    {
        string line, word;
        while (getline(mdt, line))
        {
            stringstream ss(line);
            vector<string> temp;
            while (ss >> word)
            {
                temp.push_back(word);
            }
            MDT.push_back(temp);
        }
    }
    void readKPDTAB()
    {
        string line;
        while (getline(kt, line))
        {
            stringstream ss(line);
            string keyword, value;
            ss >> keyword;
            ss >> value;
            KPDTAB.push_back({keyword, value});
        }
    }
    void readPNTAB()
    {
        string line, word;
        while (getline(pt, line))
        {
            vector<string> temp;
            stringstream ss(line);
            while (ss >> word)
            {
                temp.push_back(word);
            }
            PNTAB.push_back(temp);
        }
    }

    // display  ALA
    void displayALA(vector<string> ALA)
    {
        cout << "\n---- ALA ----" << endl;
        for (int i = 0; i < ALA.size(); i++)
        {
            cout << ALA[i] << endl;
        }
    }

    // display output
    void displayOutput()
    {
        cout << "\n---- OUTPUT EXPANDED CODE ----" << endl;
        for (int i = 0; i < MDT.size(); i++)
        {
            for (int j = 0; j < MDT[i].size(); j++)
            {
                cout << MDT[i][j] << " ";
            }
            cout << endl;
        }
    }

    int getIndfromMNT(string mname)
    {
        int i = 0;
        for (tuple<string, int, int, int, int> t : MNT)
        {
            if (get<0>(t) == mname)
            {
                return i;
            }
            i++;
        }
        return -1;
    }

    int getIndfromPNTAB(string param)
    {
        for (vector<string> temp : PNTAB)
        {
            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i] == param)
                {
                    return i;
                }
            }
        }
        return -1;
    }

    string getValuefromKPDTAB(string keyword)
    {
        for (int i = 0; i < KPDTAB.size(); i++)
        {
            if (KPDTAB[i].first == keyword)
            {
                return KPDTAB[i].second;
            }
        }
        return "NULL";
    }

    // expanding func calls function
    void expand_call(vector<string> fcall)
    {
        string mname = fcall[0]; // extract macro name
        // search corresponding index of macro name from MNT
        int idx = getIndfromMNT(mname);
        if (idx == -1)
        {
            cout << "\nFunction not belogs to existing MNT" << endl;
            exit(1);
        }
        int cntp = get<1>(MNT[idx]);
        int cntkp = get<2>(MNT[idx]);
        int mdtp = get<3>(MNT[idx]);
        int kpdtp = get<4>(MNT[idx]);

        vector<string> ALA((cntp + cntkp), "NULL"); // Argument List Array (Only use in pass2 not in pass1)

        int i;
        for (i = 0; i < cntp; i++) // adding positional parameters directly to ALA from funcion call statement
        {
            ALA[i] = fcall[i + 1];
        }

        for (int j = i; j < fcall.size(); j++)
        {
            if (fcall[j].find("=") != string::npos)
            {
                int ind = fcall[j].find("=");
                string keyword = fcall[j].substr(0, ind);
                string value = fcall[j].substr(ind + 1); // actual value of keyword parameter from function call statement

                int pInd = getIndfromPNTAB(keyword); // get index from PNTAB for keyword parameter if mentioned in actual call
                if (pInd == -1)
                {
                    cout << "Error: Keyword parameter " << keyword << " not found in PNTAB." << endl;
                    exit(1);
                }
                ALA[pInd] = value; // store actual value to keyword parameter to ALA
            }
        }
        for (int i = 0; i < ALA.size(); i++)
        {
            if (ALA[i] == "NULL")
            {
                string value = getValuefromKPDTAB(PNTAB[idx][i]);
                if (value == "NULL" || value == " ")
                {
                    cout << "\nInvalid function call parameters" << endl;
                }
                ALA[i] = value;
            }
        }
        displayALA(ALA);

        // Update MDT
        int limit;
        if (idx + 1 < MNT.size()) // check it is last func call if not then set limit to start of next func call
        {
            limit = get<3>(MNT[idx + 1]);
        }
        else
        {
            limit = MDT.size();
        }
        for (int i = mdtp; i < limit; i++)
        {
            for (int j = 0; j < MDT[i].size(); j++)
            {
                if (MDT[i][j].find("(") != string::npos)
                {
                    int ind = stoi(MDT[i][j].substr(3, 1)) - 1;
                    MDT[i][j] = ALA[ind];
                }
            }
        }
    }

    // process input file
    void pass2()
    {
        int i = 1;
        string line, word;
        while (getline(fin, line))
        {
            stringstream ss(line);
            vector<string> temp;
            while (ss >> word)
            {
                temp.push_back(word);
            }
            cout << "\nMacro " << i << endl;
            i++;
            expand_call(temp);
        }
    }
};

int main()
{
    Macro m;
    m.pass2();
    m.displayOutput();
}