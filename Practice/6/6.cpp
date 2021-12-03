#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#define FILE_NAME "words.txt"
#define COUNT 10
using namespace std;

char search_words[COUNT][20] = {"Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit", "Donec", "eleifend"};
int count[COUNT];

int is_alpha(char c) {
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int is_equal(char *a, const char *key) {
	char b[20];
	strcpy(b, key);
	int i, lena = strlen(a);
	int lenb = strlen(b);
	if(lena != lenb)
		return 0;
	for(i = 0; i < lena; i++) {
		if(a[i] > 90)
			a[i] -= 32;
		if(b[i] > 90)
			b[i] -= 32;
	}
	return (strcmp(a,b)==0);
}

void readword(FILE *fp, char *temp) {
	char c = fgetc(fp);
	int i = 0;
	while(c != EOF && is_alpha(c) != 0)
	{
		temp[i++] = c;
		c = fgetc(fp);
	}
	temp[i] = '\0';
}

int determine_count(const char * file, const char *key) {
	int wcount = 0;
	FILE *fp = fopen(file, "r");
	char temp[40];
	while(feof(fp) == 0)
	{
		readword(fp, temp);
		if(is_equal(temp, key) != 0)
			wcount++;
	}
	return wcount;
}

int main(int argc, char **argv) {
	int nt, i;
	char *iname = NULL;
	if(argc != 2)
		error(1, 0, "Usage: ws filename.txt\n");
	iname = argv[1];
	for(i = 0; i < COUNT; i++) {
		count[i] = 0;
	}
	cout << "Enter number of threads\n";
	cin >> nt;
	double t = omp_get_wtime();
	#pragma omp parallel for num_threads(nt)
	for(i = 0; i < COUNT; i++)
		count[i] = determine_count(iname, search_words[i]);
	t = omp_get_wtime() - t;
	cout << "Time taken: " << t << "Threads: " << nt << "\n";
	for(i = 0; i < COUNT; i++)
		cout << search_words[i] << " : " << count[i] << "\n";
	return 0;
}
