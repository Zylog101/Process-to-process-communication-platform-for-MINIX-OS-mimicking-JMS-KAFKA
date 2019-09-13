#include<unistd.h>
#include<stdio.h>
#include<lib.h>
#include<stdlib.h>

char * TopicLookup(void)
{
	//todo:data should be dynamic in this case it is hardcoded
	message m;
	int length=1000;
	char *ch= (char *)malloc(length);
	//printf("reached TopicLookup");
	m.m1_p1=ch;
	
	_syscall(PM_PROC_NR,TOPICLOOKUP,&m);
	
	//return ch;
	//printf("\n inside library routine received the topic lookup result : %s\n",ch);	
	return ch;
}
