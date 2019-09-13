#include <lib.h>    // provides _syscall and message
#include <stdio.h>
#include <stdlib.h> // provides atoi
#include <string.h>
#include<unistd.h>

int TopicPublisher(char * topicName) 
{
    
    message m;      // Minix uses message to pass parameters to a system call
    int lengthtn=strlen(topicName);
	char *ch=(char *)malloc(lengthtn+1);
	ch=strcpy(ch,topicName);
	ch[lengthtn]='\0';
    
    
    m.m1_p1 =ch;    // set first integer of message to i
    
    m.m1_i1 = strlen( topicName);
    
   int tpRet= _syscall(PM_PROC_NR, TOPICPUBLISHER, &m);
	//printf("\nreturn from Tp %d\n",tpRet);
	return tpRet;

}