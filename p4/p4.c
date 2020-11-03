#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno>
int main(int argc, char *argv[]){
	if (argc != 2) 
		perror("Expected:./uniq <PATH\\test.txt>");
		else{
			int fd;
			if ((fd = open(argv[1], O_RDONLY) == -1)
				perror("Error open file");
		else {
			errno = 0;
			char c;
		while ((read(fd, &c, 1) != 0) && (errno == 0)) {//fseek, ftell
			if (putchar(c) == EOF)
				perror ("Error read file");
	}
		if (errno != 0)
			perror ("Error");
		close(fd);
		}
	}
return 0;
}
