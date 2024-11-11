#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class PageReplacement
{
    vector<int>pageTabel;
    vector<int>input;
    int frameSize, noOfPages;

    public:
        PageReplacement()
        {
            cout<<"\nEnter no of pages: ";
            cin>>noOfPages;
            int n;
            cout<<"\nEnter pages"<<endl;
            for(int i=0;i<noOfPages;i++)
            {
                cin>>n;
                input.push_back(n);
            }
            cout<<"\nEnter frame size: ";
            cin>>frameSize;
        }

        bool isHit(int page)
        {
            if(find(pageTabel.begin(),pageTabel.end(),page) != pageTabel.end())
            {
                return true;
            }
            return false;
        }


        void FIFO()
        {
            int hits = 0;
            int faults = 0;
            for(int i=0;i<noOfPages;i++)
            {
                if(!isHit(input[i]))
                {
                    faults++;
                    if(pageTabel.size() == frameSize)
                    {
                        cout<<"Miss: Replacing "<<pageTabel[0]<<" with "<<input[i];
                        pageTabel.erase(pageTabel.begin());
                    }
                    pageTabel.push_back(input[i]);
                }
                else
                {
                    cout<<"Hit"<<endl;
                    hits++;
                }
            }
            cout<<"\nTotal no of page faults: "<<faults<<endl;
            cout<<"\nHit Ratio: "<<(float)hits/noOfPages<<endl;
        }

        void LRU()
        {
            int hits = 0;
            int faults = 0;
            map<int,int>mp;
            for(int i=0;i<noOfPages;i++)
            {
                mp[i] = INT_MIN;
            }
            for(int i=0;i<noOfPages;i++)
            {
                if(!isHit(input[i]))
                {
                    faults++;
                    cout<<"Miss";
                    if(pageTabel.size() == frameSize)
                    {
                        for(int j=0;j<i;j++)
                        {
                            mp[input[j]] = j;
                        }
                        int min = INT_MAX;
                        int ind = 0;
                        for(int k=0;k<frameSize;k++)
                        {
                            if(mp[pageTabel[k]] < min)
                            {
                                min = mp[pageTabel[k]];
                                ind = k;
                            }
                        }
                        cout<<": Replacing "<<pageTabel[ind]<<" with "<<input[i]<<endl;
                        pageTabel.erase(find(pageTabel.begin(),pageTabel.end(),pageTabel[ind]));
                    }
                    pageTabel.push_back(input[i]);
                }
                else
                {
                    cout<<"Hit"<<endl;
                    hits++;
                }
            }
            cout<<"\nTotal no of page faults: "<<faults<<endl;
            cout<<"\nHit Ratio: "<<(float)hits/noOfPages<<endl;
        }

        void Optimal()
        {
            int hits = 0;
            int faults = 0;
            map<int,int>mp;
            for(int i=0;i<noOfPages;i++)
            {
                mp[input[i]] = INT_MAX;
            }
            for(int i=0;i<noOfPages;i++)
            {
                if(!isHit(input[i]))
                {
                    faults++;
                    cout<<"Miss";
                    if(pageTabel.size() == frameSize)
                    {
                        for(int j=i+1;j<noOfPages;j++)
                        {
                            mp[input[j]] = j;
                        }
                        int max = INT_MIN;
                        int ind;
                        for(int k=0;k<frameSize;k++)
                        {
                            if(mp[pageTabel[k]] > max)
                            {
                                max = mp[pageTabel[k]];
                                ind = k;
                            }
                        }
                        cout<<": Replacing "<<pageTabel[ind]<<" with "<<input[i];
                        pageTabel.erase(find(pageTabel.begin(),pageTabel.end(),pageTabel[ind]));
                    }
                    pageTabel.push_back(input[i]);
                    cout<<endl;
                }
                else
                {
                    cout<<"Hit"<<endl;
                    hits++;
                }
            }
            cout<<"\nTotal no of page faults: "<<faults<<endl;
            cout<<"\nHit Ratio: "<<(float)hits/noOfPages<<endl;
        }
};

int main()
{
    PageReplacement pr;
    // pr.FIFO();
    // pr.LRU();
    pr.Optimal();

}