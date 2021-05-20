#ifndef _PROJ2_H_
#define _PROJ2_H_

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

//struct for the program arguments
typedef struct {
    int n_elfs;
    int n_reindeers;
    int time_elf;
    int time_reindeer;
} arguments_t;

//struct for shared resources
typedef struct {
    int elf_waiting;
    int count;
	int rd_returned;
    int christmas;
    int hitched;
    int helped;
}shared_t;

//shared memory
shared_t* mem = NULL;
//semaphores
sem_t* sem_write_lock; //semaphore for write to file
sem_t* sem_santa; //semaphore for waking up santa (either from elfs or rd)
sem_t* sem_elf_wait; //semaphore for elf waiting before works shop
sem_t* sem_elf_helped; //semaphore for elf to wait, so there are 3 of them so santa can help
sem_t* sem_rd_wait; //semaphore for rd to wait for the hitch so christmas can start
sem_t* sem_rd_hitched; //semaphore to signalize all rd are hitched
sem_t* sem_mutex; //semaphore for writing to shared memory
sem_t* sem_sleep; //semaphore to say that santa go to sleep after he helps all 3 elfs

//poiter to file, so we can write to in
FILE* filehandle = NULL;

//function to parse argumets, if any of the argumets is invalid it returns 1
//and the program ends
int arg_parse(char **argv, int argc, arguments_t *arg);
//functio to unlink all used semaphores
void semaphore_unlink();
//function to create shared memory, open semaphores and open file with write option
void create_resources();
//function to destroy shared memory, close file and all the semaphores
void destroy_resources();
//function main santa process, its only one
void santa_process(arguments_t arg);
//function for NE elf processes
void elf_process(int id, arguments_t arg);
//function for NRD processes
void reindeer_process(int id, arguments_t arg);

#endif /*END_IF_PROJ2_H_*/