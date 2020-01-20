#include<iostream>
#include <pthread.h>
#include <cstdlib>
#include <mutex>
#include<unistd.h>

using namespace std;

mutex lck1;
mutex lck2;
mutex lck3;

int count = 0;
int m;
int sum = 0;

struct params {
    int room_number;
    int number;
};

void *student(void *num)
{
    if(count >= m)
        lck2.lock();
    
    lck1.lock();
    count++;
    params *p = (params *) num;
    sum +=p->number;
    lck1.unlock();
    sleep(4);

    lck1.lock();
    cout<<endl<<"R"<<p->room_number+1;
    count--;
    if(count == 0)
    {
       lck2.unlock();
       cout<<endl<<sum<<endl;
       cout<<"session done!"<<endl;
       sum = 0;
    }
    lck1.unlock();

    pthread_exit(NULL);
}

int main()
{
    string finish;
    int n;
    
    do{
        int ret;
        pthread_t advisors[n];

        cout<<"enter number of advisors:";
        cin>>n;
        m = n;
        int num[n];
        for(int i = 0 ; i<n ; i++)
        {
            cout<<"number of students for "<<i+1<<"st advisor:";
            cin>>num[i];   
        }

        for(int i = 0 ; i<n ; i++)
        {
            params *p = (params *)malloc(sizeof(struct params));
            p->number = num[i];
            p->room_number = i;
            ret = pthread_create(&advisors[i],NULL,student,(void *)p);
            if(ret)
            {
                cout<<"unable to fill the room "<<i+1<<endl;
                exit(-1);
            }
        }

        for(int i = 0 ; i < n ; i++)
        {
                ret = pthread_join(advisors[i],NULL);
                if (ret) {
                    cout << "Error:unable to join," << ret << endl;
                    exit(-1);
                }
        }
        
        do{
            cout<<"continue?[y/n]: ";
            cin>>finish;
        }while(finish != "n" && finish != "y");
        if(finish == "n")   break;
    }while(true);
    

    pthread_exit(NULL);
    return 0;
}