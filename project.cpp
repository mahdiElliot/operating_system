#include<iostream>
#include <pthread.h>
#include <cstdlib>
#include <mutex>
#include<semaphore.h>
#include<unistd.h>

using namespace std;

sem_t s1;
sem_t s2;

int count = 0;
int sum = 0;

struct params {
    int room_number;
    int number;
};

void *student(void *num)
{
    sem_wait(&s2);

    count++;
    params *p = (params *) num;
    sum +=p->number;
    sleep(4);

    sem_wait(&s1);
    cout<<"R"<<p->room_number+1<<endl;
    count--;
    if(count == 0)
    {
       sem_post(&s2);
       cout<<endl<<sum<<endl;
       cout<<"session done!"<<endl;
       sum = 0;
    }
    sem_post(&s1);

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

        sem_init(&s1,0,1);
        sem_init(&s2,0,n);

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

    sem_destroy(&s1);
    sem_destroy(&s2);
    
    pthread_exit(NULL);
    return 0;
}