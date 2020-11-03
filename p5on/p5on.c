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
	int count = 0;
	char name[255];
	char name1[255] = "test";
	char name2[255] = "";

	DIR* p = opendir(name1);
	if (p == NULL) {
		perror("Open dile\n");
		information = 1;
	}
	else {
		struct dirent* item;
		while ((item = readdir(p))) {
			if ((*item).d_type == DT_DIR) {
			