#include<iostream>

using namespace std;

int main(int argc,char * argv[])
{
    cout<<"参数个数为："<<argc<<std::endl;
    for(int i =0;i<argc;i++)
    {
        cout<<"参数为："<<argv[i]<<endl;
    }
}