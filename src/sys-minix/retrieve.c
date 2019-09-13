#include<unistd.h>
#include<stdio.h>
#include<lib.h>
#include<stdlib.h>
#include<string.h>

char * Retrieve(char *topicName)
{
	message m;
	char *msg=(char *)malloc(sizeof(char )*100);
	char *ch=(char *)malloc(sizeof(char )*10);

	ch=strcpy(ch,topicName);
	ch[strlen(topicName)+1]='\0';
	//printf("retrieved message ");
	m.m1_p1=ch;
	m.m1_i1=strlen(topicName);
	m.m1_p2=msg;
	//printf("\nin lib call:%s\n",ch);
	_syscall(PM_PROC_NR,RETRIEVE,&m);
	return msg;
}