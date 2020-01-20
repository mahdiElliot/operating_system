#include<bits/stdc++.h>

using namespace std;

int main()
{
    int counter = 0;
    if(fork()>0)
    {
        counter++;
        cout<<"A/"<<counter<<endl;
        if(fork() == 0)
        {
            counter++;
            cout<<"C/"<<counter<<endl;
            if(fork()>0)
                wait(NULL);
            else
                cout<<"D/"<<counter<<endl;
        }
        else wait(NULL);
    }
     else
    {
        cout<<"B/"<<counter<<endl;
        if(fork() == 0)
        {
            counter++;
            cout<<"C/"<<counter<<endl;
            if(fork()>0)
                wait(NULL);
            else
                cout<<"D/"<<counter<<endl;
        }
        else wait(NULL);
    }

}