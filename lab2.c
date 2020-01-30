#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	
	const int SIZE = 128;
	const char *name = "COLLATZ";
	
	const int STR_SIZE = 128;
	char str[STR_SIZE];	

	int shm_fd;
	void *ptr;

	//Create shared memory 'COLLATZ'
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	//Configure size of memory
	ftruncate(shm_fd, SIZE);

	//Mapping 
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);	
	if (ptr == MAP_FAILED) {
		printf("MAP IS A FAILURE. : (\n");
		return -1;
	}
		
	pid_t pid;
	pid = fork();	
	if (argc != 2) { 
	 	fprintf(stderr, "Usage: ./lab1 <int value>\n");
	}	

	if (pid < 0) {
		fprintf(stderr, "It forkin failed\n");
		return 1;
	} else if (pid == 0) {
		printf("It forked!\n");
		//execlp("/home/osc/final-src-osc10e/ch3/collatz", "collatz", argv[1], NULL);
		//fprintf(stderr, "Error\n");
		int current = atoi(argv[1]);
		while(current != 1) {
			sprintf(str," %d ", current);
			sprintf(ptr,str);
			ptr += strlen(str);
			if (current % 2 == 0) {
				current = current/2;			
	 		} else {
				current = 3*current+1;
			}
		}
		sprintf(str," %d\n", current);
		sprintf(ptr,str);
		ptr += strlen(str);
	} else {
		wait(NULL);
		shm_fd = shm_open(name, O_RDONLY, 0666);
		if (shm_fd == -1) {

			printf("WE all gonna die\n");
			exit(-1);
		}
		printf("%s", (char *)ptr);
		if (shm_unlink(name) == -1){
			printf("error removing %s\n", name);
			exit(-1);
		}
	}	
	return 0;
}
