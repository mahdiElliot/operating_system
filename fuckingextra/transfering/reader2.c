
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>  


#define NAME "sharedmem"
#define NAME2 "shm2"
#define NAME3 "shm3"
#define NAME4 "shm4"


#define SIZE 4096

int main() {

    FILE *f = fopen("w.txt","w");  //file
    
    int fd = shm_open(NAME, O_RDONLY, 0666);
    if (fd < 0) {
        perror("shm_open()");
        return EXIT_FAILURE;
    }

    int fd2 = shm_open(NAME2, O_RDONLY, 0666);
    if(fd2 < 0)
    {
        perror("shm_open()");
        return EXIT_FAILURE;
    }
    int *d = (int *) mmap(0, 5, PROT_READ, MAP_SHARED, fd2, 0);

    int semfd = shm_open(NAME3,O_EXCL | O_RDWR,0600);
    if(semfd < 0)
    {
        perror("shm_open()");
        return EXIT_FAILURE;
    }


    sem_t *mutex = (sem_t *)mmap(0,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED, semfd, 0);


    int semfd2 = shm_open(NAME4,O_EXCL | O_RDWR,0600);
    if(semfd2 < 0)
    {
        perror("shm_open()");
        return EXIT_FAILURE;
    }

    sem_t *mutex2 = (sem_t *)mmap(0,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED, semfd2, 0);

    int i = 0;
    while(i<d[0])
    {
        i++;
        sem_wait(mutex2);
        char *data =
            (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        printf("receiver mapped address: %p\n", data);

        fputs(data,f);

        munmap(data, SIZE);
        sem_post(mutex);
    }

    munmap(d,5);
    munmap(mutex,sizeof(sem_t));
    munmap(mutex2,sizeof(sem_t));

    close(semfd);
    close(semfd2);
    close(fd2);
    close(fd);

    shm_unlink(NAME);
    shm_unlink(NAME2);
    shm_unlink(NAME3);
    shm_unlink(NAME4);

    fclose(f);//file
    
    return EXIT_SUCCESS;
}