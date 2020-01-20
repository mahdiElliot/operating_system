#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>  

#define NAME "sharedmem"
#define NAME2 "shm2"
#define NAME3 "shm3"
#define NAME4 "shm4"

#define SIZE 4096

int main() {

    FILE *f = fopen("m.txt","r");//open file

    //get the size of file
    fseek(f, 0, SEEK_END); 
    int s = ftell(f); 
    fseek(f, 0, SEEK_SET);
   
    //////////
    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
    }
    ftruncate(fd, SIZE);
    ////////

    ////////
    int fd2 = shm_open(NAME2,O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd2 < 0) {
        perror("shm_open()");
    }

    ftruncate(fd2,5);
    int *d = (int *)mmap(0,5,PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    d[0] = (s/SIZE)+1;

    munmap(d,5);
    //////////////

    /////////////
    int semfd = shm_open(NAME3,O_CREAT | O_EXCL | O_RDWR, 0600);
    if (semfd < 0) {
        perror("shm_open()");
    }

    ftruncate(semfd,sizeof(sem_t));
    sem_t *mutex = (sem_t *)mmap(0,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED, semfd, 0);
    sem_init(mutex,1,1);

    ///////////////
        
    int semfd2 = shm_open(NAME4,O_CREAT | O_EXCL | O_RDWR, 0600);
    if (semfd2 < 0) {
        perror("shm_open()");
    }

    ftruncate(semfd2,sizeof(sem_t));
    sem_t *mutex2 = (sem_t *)mmap(0,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED, semfd2, 0);
    sem_init(mutex2,1,0);
    ///////////////////////

    int i = 0;
    while(i<(s/SIZE)+1)
    {
        i++;
        sem_wait(mutex);
        char *data =
            (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        printf("sender mapped address: %p\n", data);
        size_t nread;

        
        int j = 0;
        for(j = 0 ; j < SIZE ; j++)
        {
            data[j] = (char) fgetc(f);
            if(data[j] == EOF)  break;
        }
        while(j<SIZE)
        {
            data[j] =NULL;
            j++;
        }
        
        munmap(data, SIZE);
        sem_post(mutex2);
        
    }

    munmap(mutex,sizeof(sem_t));
    munmap(mutex2,sizeof(sem_t));

    close(fd);
    close(fd2);
    close(semfd);
    close(semfd2);

    fclose(f);//clsoe file

    return EXIT_SUCCESS;
}