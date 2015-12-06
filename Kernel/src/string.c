#include "../include/string.h"

int strcmp(char * str1, char * str2){
	int i;
	int length1 = strlen(str1);
	int lenght2 = strlen(str2);

	if(length1 != lenght2){
		return length1 - lenght2;
	}

	for(i = 0; i < length1; i++){
		if(str1[i] != str2[i]){
			return str1[i] > str2[i] ? 1 : -1;
		}
	}
	return 0;
}

int strlen(char * str){
	int i = 0;
	
	while (str[i]){
		i++;
	}
	return i;
}