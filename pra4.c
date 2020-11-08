#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>

int main(int argc, char **argv){
	FILE* f1 = fopen(argv[1], "r");
	FILE* f2 = fopen(argv[1], "r");
	char c= '0', c1 = '0';
	int res, i = 0, j = 0, poz_f1 = 0, poz_f2 = 0;
	char s[1024];
	bool flag = false;
	if(f1!=NULL && f2!=NULL){		
		while(c1!='\n')
			c1 = fgetc(f2); // переходим на вторую строчку
		while(!feof(f1)){
			while ((c!='\n')&&(c!=EOF)&&(c1!=EOF)) {
				c = fgetc(f1);
				c1 = fgetc(f2);
				i++;//записываю сколько символов уже прочитали
				poz_f1 = ftell(f1);
				poz_f2 = ftell(f2);
				if((c==c1)&&(c!='\n')&&(c1!='\n')){
					j++;
				}
				else if ((c!=c1)&&(poz_f1<poz_f2)&&(j!=i)){
					flag = true;
					while((c1!='\n')&&(c1!=EOF))
						c1 = fgetc(f2); 
					if(fseek(f1, -i, SEEK_CUR)!=0)
						printf("error:%s\n", strerror(errno));// возвращаемся в начало строки
					i = 0;
					j = 0;
					c='0';
				}
				else if(poz_f1 == poz_f2){
					break;
				}
				else{
					break;
				}
			}

			if(flag == true){
				while ((c!='\n')&&(c!=EOF)){// выводим строчку
					c = fgetc(f1);
					printf("%c", c);	
				}
			}
			else{
				while ((c!='\n')&&(c!=EOF))
					c = fgetc(f1);

			}
			poz_f1  = ftell(f1);
			rewind(f2);//возвращает значение. Успех fseek - 0, ftell - текущее смещение
			if(fseek(f2, poz_f1, SEEK_CUR)!=0)
				printf("error:%s\n", strerror(errno));
			c1='0';
			while ((c1!='\n')&&(c1!=EOF))
				c1 = fgetc(f2);
			c = '0';
			c1 = '0';
			i = 0;
			j = 0;
			flag = false; 
		}
	}
	else
		perror("Openning error");
	res = fclose(f1);
	if(res == EOF)
		perror("Closing error");
	res = fclose(f2);
	if(res == EOF) 
		perror("Closing error");
}
