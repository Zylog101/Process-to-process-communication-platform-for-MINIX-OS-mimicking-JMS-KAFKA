#include<unistd.h>
#include<lib.h>
#include<stdio.h>
#include<string.h>

int TopicCreate(char *topicName)
{
	message m;
	m.m1_i1=strlen(topicName);
	m.m1_p1=topicName;
	
	
	return _syscall(PM_PROC_NR,TOPICCREATE,&m);
}
