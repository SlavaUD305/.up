#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

struct Titem {
	int data;
	struct Titem* next;
};
int main() {
	struct Titem* first = 0, * l, * j, * g;
	int information = 0;
	int i = 0;
	int h = 1;
	int res = 0;
	int symbol = 0;
	int res1 = 0;
	int count = 0;
	char name1[255] = "";


	char way[255] = "/home/slava/YP/prac5/test/";

	DIR* p = opendir(way);
	if (p == NULL) {
		perror("Open dile\n");
		res = 1;
	}
	else {
		struct dirent* item;
		while ((item = readdir(p))) {

			if ((*item).d_type == DT_DIR) {
				if (strcmp((*item).d_name, ".") != 0) {
					if (strcmp((*item).d_name, "..") != 0) {
						pid_t child_id = fork();
						if (child_id == -1) perror("Error of creating\n");
						else
							if (child_id == 0) {
								//strcpy(name1, "");
								printf("This is child process\n");
								g = first;
								while (g) {
									g->data = -1;
									g = g->next;
								}
								strcat(way, item -> d_name);
								strcat(way, "/");
								//strcat(name1, way);
								//strcat(name1, item -> d_name);
								//strcar(name1, "/");


								closedir(p);
								p = opendir(way);
								if (p == NULL) {
									perror("File openning error\n");
									res = 1;
								}
							}
							else {
								printf("This is parent process");
								g = malloc(sizeof(struct Titem));
								if (g) {
									g->data = child_id;
									g->next = first;
									first = g;
								}
							}
					}
				}
			}
			else if ((*item).d_type == DT_REG) {
				strcat(name1, way);
				strcat(name1, item->d_name);

				int sum_simbol = strlen(item->d_name);
				for (i = 0; i < sum_simbol; ++i) {
					if (!(((item->d_name[i] >= 'A') && (item->d_name[i] <= 'Z'))
								|| ((item->d_name[i] >= 'a') && (item->d_name[i] <= 'z')))) {
						symbol++;
						res1 = 1;
					}
				}
				if ((res1 == 1) && (symbol >= 2)) {
					if(unlink(name1) == -1) {
						perror("Error unlink file");
						res = -1;
					}
				}

			}
		}
		errno = 0;
		if (errno != 0) {
			perror("Проблема открытия каталога");
			res = -1;

		}
		closedir(p);
		int status;
		while (first != NULL) {
			printf ("wait for %d in %d\n",first->data,getpid());
			if (first->data != -1) {
				pid_t result_of_waitpid = waitpid(first->data, &status, 0);
				if (result_of_waitpid == -1) perror("Error of waitpid");
				else
					if (!WIFEXITED(status)) perror("Child process hasn't finished properly\n");
					else
						printf("Child process has returned %d\n", WEXITSTATUS(status));

			}
			g = first;
			first = first->next;
			free(g);


		}


	}
	return res;
}



