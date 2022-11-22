#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 32
 
 
int main(int argc, char **argv) {
 
	int ret;
 
	// Alocam un vector de 2 elemente int in care se vor scrie
	// descriptorii celor doua capete ale pipe-ului
	int pipe_fd[2];
 
	// Obtinem cei doi descriptori ai pipe-ul.
	// [0] - capatul de citire
	// [1] - capatul de scriere
	ret = pipe(pipe_fd);
	if (ret < 0) {
		perror("error on creating pipe: ");
		exit(-1);
	}
 
	int pid;
	pid = fork();
 
	// Daca suntem in procesul parinte
	if (pid > 0) {
		char buffer[BUFFER_SIZE];
		strcpy(buffer, "Hello din parinte!");
 
		// inchidem capatul nefolosit
		close(pipe_fd[READ_END]);
 
		// Trimitem date catre procesul copil
		write(pipe_fd[WRITE_END], buffer, strlen(buffer));
 
		close(pipe_fd[WRITE_END]);
 
		// Asteptam sa se termine copilul
		wait(NULL);
		exit(0);
	}
 
	// Daca suntem ]n procesul copil
	if (pid == 0) {
		// Procesul copil a mostenit descriptorii pipe-ului
 
		// inchidem capatul nefolosit
		close(pipe_fd[WRITE_END]);
 
		char buffer[BUFFER_SIZE];
		int ret;
 
		// Citim din pipe
		ret = read(pipe_fd[READ_END], buffer, BUFFER_SIZE);
 
		// Adaugam finalul de sir
		buffer[ret] = '\0';
 
		// Afisam mesajul la terminal
		printf("proces fiu: %s", buffer);
 
		close(pipe_fd[READ_END]);
		exit(0);
	}
	return 0;
}	