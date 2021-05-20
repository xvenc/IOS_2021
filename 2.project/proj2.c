#define _GNU_SOURCE

#include "proj2.h"
//names for semaphores
#define SEM_WRITE_NAME "/xkorva03_write"
#define SEM_SANTA_NAME "/xkorva03_santa"
#define SEM_ELF_WAIT_NAME "/xkorva03_elf_wait"
#define SEM_ELF_HELPED_NAME "/xkorva03_elf_help" 
#define SEM_RD_WAIT_NAME "/xkorva03_rd_wait"
#define SEM_RD_HITCHED_NAME "/xkorva03_rd_hitched"
#define SEM_MUTEX_NAME "/xkorva03_mutex"
#define SEM_SLEEP "/xkorva03_sleep"




//function to parse argumets, if any of the argumets is invalid it returns 1
//and the program ends
int arg_parse(char **argv, int argc, arguments_t *arg) {
    
    char *garbage = NULL;
    //check if there is correct amount of arguments
    if (argc != 5) {
        return 1;
    }
    //read the arguments to the structure
    arg->n_elfs = strtol(argv[1], &garbage, 10);
    arg->n_reindeers = strtol(argv[2], &garbage, 10);
    arg->time_elf = strtol(argv[3], &garbage, 10);
    arg->time_reindeer = strtol(argv[4], &garbage, 10);
    //check if only number were set
    if (*garbage != '\0') {
        return 1;
    }
    //check if the argument were valid numbers
    if (arg->n_elfs <= 0 || arg->n_elfs >= 1000) {
        fprintf(stderr, "Invalid number of elfs were set\n");
        return 1;
    }else if (arg->n_reindeers <= 0 || arg->n_reindeers >= 20) {
        fprintf(stderr, "Invalid number of reindeers were set\n");
        return 1;
    } else if (arg->time_elf < 0 || arg->time_elf > 1000) {
        fprintf(stderr, "Invalid time for elfs work was set\n");
        return 1;
    }else if (arg->time_reindeer < 0 || arg->time_reindeer > 1000) {
        fprintf(stderr, "Invalid time for reindeers was set\n");        
        return 1;
    }
    
    return 0;
}

//functio to unlink all used semaphores
void semaphore_unlink() {

    //unlink all named semaphored and check if it was completed succesfuly
    if ((sem_unlink(SEM_WRITE_NAME)) == -1) {
        fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_WRITE_NAME);
        exit(1);
    } else if ((sem_unlink(SEM_SANTA_NAME)) == -1) {
        fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_SANTA_NAME);
        exit(1);
    } else if ((sem_unlink(SEM_ELF_WAIT_NAME)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_ELF_WAIT_NAME);
        exit(1);
	} else if ((sem_unlink(SEM_ELF_HELPED_NAME)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_ELF_HELPED_NAME);
        exit(1);
	} else if ((sem_unlink(SEM_RD_WAIT_NAME)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_RD_WAIT_NAME);
        exit(1);
	} else if ((sem_unlink(SEM_RD_HITCHED_NAME)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_RD_HITCHED_NAME);
        exit(1);
	} else if ((sem_unlink(SEM_MUTEX_NAME)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_MUTEX_NAME);
        exit(1);
	} else if ((sem_unlink(SEM_SLEEP)) == -1) {
		fprintf(stderr, "ERROR: Couldnt unlink semaphore %s\n",SEM_SLEEP);
        exit(1);
	} 
}

//function to create shared memory, open semaphores and open file with write option
void create_resources() {
    //if program crashed before unlink semaphores, so it doesnt make mess
    sem_unlink(SEM_WRITE_NAME);
    sem_unlink(SEM_SANTA_NAME);
	sem_unlink(SEM_ELF_WAIT_NAME);
	sem_unlink(SEM_ELF_HELPED_NAME);
	sem_unlink(SEM_RD_WAIT_NAME);
	sem_unlink(SEM_RD_HITCHED_NAME);
    sem_unlink(SEM_MUTEX_NAME);
    sem_unlink(SEM_SLEEP);
    //open file for writing
    filehandle = fopen("proj2.out","w");
    if (filehandle == NULL) {
        fprintf(stderr, "ERROR: Couldnt open file proj2.out\n");
        exit(1);
    }
    //create all semaphores that are needed
    if ((sem_write_lock = sem_open(SEM_WRITE_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED ) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_WRITE_NAME);
        exit(1);
    }
    if ((sem_santa = sem_open(SEM_SANTA_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_WRITE_NAME);
        exit(1);
    }
	if ((sem_elf_wait = sem_open(SEM_ELF_WAIT_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_ELF_WAIT_NAME);
        exit(1);
    }
	if ((sem_elf_helped = sem_open(SEM_ELF_HELPED_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_ELF_HELPED_NAME);
        exit(1);
    }
	if ((sem_rd_wait = sem_open(SEM_RD_WAIT_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_RD_WAIT_NAME);
        exit(1);
    }
	if ((sem_rd_hitched = sem_open(SEM_RD_HITCHED_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_RD_HITCHED_NAME);
        exit(1);
    }
	if ((sem_mutex = sem_open(SEM_MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
		fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_RD_HITCHED_NAME);
        exit(1);
	}
	if ((sem_sleep = sem_open(SEM_SLEEP, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "ERROR: Cannot create semaphore %s\n",SEM_SLEEP);
        exit(1);
	}
    

    //creating shared memory
    if((mem = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        fprintf(stderr, "ERROR: Cannot create shared memory\n");
        exit(1);
    }
}

//function to destroy shared memory, close file and all the semaphores
void destroy_resources() {

    //destroy shared memory
    if((munmap(mem, sizeof(shared_t))) == -1) {
        fprintf(stderr, "ERROR: Couldnt destroy shared memory\n");
        exit(1);
    }
    //close the named semaphores
    if (sem_close(sem_write_lock) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_WRITE_NAME);
        exit(1);
    } else if (sem_close(sem_santa) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_SANTA_NAME);
        exit(1);
    } else if (sem_close(sem_elf_wait) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_ELF_WAIT_NAME);
        exit(1);
    } else if (sem_close(sem_elf_helped) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_ELF_HELPED_NAME);
        exit(1);
    } else if (sem_close(sem_rd_wait) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_RD_WAIT_NAME);
        exit(1);
    } else if (sem_close(sem_rd_hitched) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_RD_HITCHED_NAME);
        exit(1);
    } else if (sem_close(sem_mutex) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_MUTEX_NAME);
        exit(1);
    } else if (sem_close(sem_sleep) == -1) {
        fprintf(stderr, "ERROR: Couldnt close semaphore %s\n",SEM_SLEEP);
        exit(1);
    } 
    //removes named semaphores
    semaphore_unlink();
    //close the file
    fclose(filehandle);
}

//function main santa process, its only one
void santa_process(arguments_t arg) {
    
    //santa starts with writing that he goes to sleep
    sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: Santa: going to sleep\n",mem->count);
    fflush(filehandle);
    sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);



    while (1) {
        //santa sleeps until 3 elfes need help or rd returned form vacation
        sem_wait(sem_santa);
        if (mem->rd_returned == arg.n_reindeers && mem->christmas == 0) {
		    //last rd returned so hitch all
            sem_wait(sem_write_lock);
            fprintf(filehandle,"%d: Santa: closing workshop\n",mem->count);
            fflush(filehandle);
            sem_wait(sem_mutex);
            mem->count++;
            sem_post(sem_mutex);
            sem_post(sem_write_lock);
            
            //post all waiting rd for hitch
            for (int i = 0; i < arg.n_reindeers; i++) {               
                sem_post(sem_rd_hitched);
            }
            //wait for rd to get hitched
            sem_wait(sem_rd_wait);
	    } else {
            //help all 3 elfes
            sem_wait(sem_write_lock);
            fprintf(filehandle,"%d: Santa: helping elves\n",mem->count);
            fflush(filehandle);
            sem_wait(sem_mutex);
            mem->count++;
            sem_post(sem_mutex);
            sem_post(sem_write_lock);
            
            //post all 3 waiting elfes
		    sem_post(sem_elf_helped);
		    sem_post(sem_elf_helped);
		    sem_post(sem_elf_helped);
            
            //wait for the elfs to write that they got help, so he can go to sleep
            sem_wait(sem_sleep);
            //if christmas started he cant go to sleep
            if (mem->christmas == 1) {
                continue;
            } 
            //go to sleep after helping elfes
            sem_wait(sem_write_lock);
            fprintf(filehandle,"%d: Santa: going to sleep\n",mem->count);
            fflush(filehandle);
            sem_wait(sem_mutex);
            mem->count++;
            sem_post(sem_mutex);
            sem_post(sem_write_lock);
        }
        //if christmas started end this process and let elfes go to vacation
        if (mem->christmas == 1) {
            sem_wait(sem_write_lock);
            fprintf(filehandle,"%d: Santa: Christmas started\n",mem->count);
            fflush(filehandle);
            sem_wait(sem_mutex);
            mem->count++;
            sem_post(sem_mutex);
            sem_post(sem_write_lock);
            sem_post(sem_elf_helped);
            break;
        }       
    }
}

//function for NE elf processes
void elf_process(int id, arguments_t arg) {

    //elf process started
    sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: Elf %d: started\n",mem->count,id+1);
    fflush(filehandle);
	sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);



	while (1) {
		//elf working alone
		//elf semafor cekani nez muze jit do rady pred dilnu
        srand(time(0) % getpid());
    	int random = (rand() % (arg.time_elf + 1));
    	usleep(random * 1000);

        //wait so they go to work shop one by one
    	sem_wait(sem_elf_wait);

		sem_wait(sem_write_lock);
    	fprintf(filehandle,"%d: Elf %d: need help\n",mem->count,id+1);
    	fflush(filehandle);
    	sem_wait(sem_mutex);
        mem->count++;
        sem_post(sem_mutex);
    	sem_post(sem_write_lock);

		sem_wait(sem_mutex);
        mem->elf_waiting++;
		sem_post(sem_mutex);

		// if less than 3 elfes need help they will wait
		if (mem->elf_waiting == 3) {
			sem_post(sem_santa);
		} else {
            sem_post(sem_elf_wait);
        }
		//wait for santa to help them	
        if (mem->christmas == 0) {
            sem_wait(sem_elf_helped);
        }
        //if christmas they can go to the vacation
        if(mem->christmas == 1){
            sem_post(sem_elf_wait);
            break;
        }

	    sem_wait(sem_write_lock);
        sem_wait(sem_mutex);
        mem->helped++;
		sem_post(sem_mutex);
    	fprintf(filehandle,"%d: Elf %d: get help\n",mem->count,id+1);
    	fflush(filehandle);

    	sem_wait(sem_mutex);
        mem->count++;
        sem_post(sem_mutex);
        //if all 3 elfes got their help post santa so he can go to sleep
        if(mem->helped == 3) {
            sem_post(sem_sleep);
            sem_wait(sem_mutex);
            mem->helped = 0;
		    sem_post(sem_mutex);
        }
    	sem_post(sem_write_lock);


        sem_wait(sem_mutex);
        mem->elf_waiting--;
        if (mem->elf_waiting == 0) {
            sem_post(sem_elf_wait);          
        }
        sem_post(sem_mutex);

    
	}
    //elfes can go to the vacation
    sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: Elf %d: taking holidays\n",mem->count,id+1);
    fflush(filehandle);
    sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);
    sem_post(sem_elf_helped);

}

//function for NRD processes
void reindeer_process(int id, arguments_t arg) {

    //start of rd process
    sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: RD %d: rstarted\n",mem->count,id+1);
    fflush(filehandle);
    sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);


    //random time for reindeer to be on vacation
	srand(time(0) % getpid());
	// num = (rand() % (upper – lower + 1)) + lower
	int random = (rand() % (arg.time_reindeer - (arg.time_reindeer/2) + 1)) + (arg.time_reindeer/2);
	usleep(random * 1000);

    //after some time rd returns from vacation
	sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: RD %d: return home\n",mem->count,id+1);
    fflush(filehandle);
    sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);

	sem_wait(sem_mutex);
	mem->rd_returned++;
	sem_post(sem_mutex);

    //if all rd returned post santa so christmas can start
    if (mem->rd_returned == arg.n_reindeers) {
		sem_post(sem_santa);
	}
	//wait for santa so he can close workshop and hitche all rd
	sem_wait(sem_rd_hitched);


	sem_wait(sem_write_lock);
    fprintf(filehandle,"%d: RD %d: get hitched\n",mem->count,id+1);
    fflush(filehandle);
    sem_wait(sem_mutex);
    mem->count++;
    sem_post(sem_mutex);
    sem_post(sem_write_lock);

    sem_wait(sem_mutex);
    mem->hitched++;
    sem_post(sem_mutex);

    //check if all rd are hitched
    if (mem->hitched == arg.n_reindeers) {
        sem_wait(sem_mutex);
        mem->christmas = 1;
        sem_post(sem_mutex);
        sem_post(sem_rd_wait);
    }
}

int main(int argc, char **argv) {

    arguments_t arg = {.n_elfs = 0};
    //parse all arguments
    int error = arg_parse(argv, argc, &arg);
    if (error != 0) {
        fprintf(stderr, "Invalid arguments were set\n");
        return 1;
    }
    //create semaphores and shared memory
    create_resources();
    mem->count = 1;
    mem->christmas=0;
    //for process for santa
    int ID_process = fork();
    
    if (ID_process == 0) {
        //This is child
        //Proces for santa
        santa_process(arg);
        exit(0);
    } else if (ID_process > 0) {
        // main process
    } else {
        //Something went wrong
        printf("ERROR: fork failed\n");
    }
    //fork NE processes for elfes
    for (int i = 0; i < arg.n_elfs; i++) {
        pid_t id_elf = fork();
        if (id_elf == 0) {
            //proces for elf
            elf_process(i, arg);
            exit(0);
        }
    }

    //fork NRD proceses for rd
    for (int i = 0; i < arg.n_reindeers; i++) {
        pid_t id_rd = fork();
        if (id_rd == 0) {
            //process for reindeers
            reindeer_process(i, arg);
            exit(0);
        }
    }
    //wait for all child processes
    while(wait(NULL) > 0);
    //destroy semaphores and shared memory
    destroy_resources();
    return 0;
}