#include<unistd.h>
#include<lib.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//-2 could not publish as it is full, -1 unable to obtain semophore ,0 on success
int Publish(char *topicName,char *msg)
{
	message m;
	int lengthtn=strlen(topicName);
	char *ch=(char *)malloc(lengthtn+1);
	ch=strcpy(ch,topicName);
	ch[lengthtn]='\0';

	int lengthmsg=strlen(msg);
	char *topicMsg=(char *)malloc(lengthmsg+1);
	topicMsg=strcpy(topicMsg,msg);
	topicMsg[lengthmsg]='\0';
	//printf("\ninlibcall:%s\n",ch);
	m.m1_p1=ch;
	m.m1_i1=strlen(topicName);

	m.m1_i2=strlen(msg);
	m.m1_p2=topicMsg;
	
//m2
message m2;
		lengthtn=strlen(topicName);
		char *ch2=(char *)malloc(lengthtn+1);
		ch2=strcpy(ch2,topicName);
		ch2[lengthtn]='\0';
		
		 lengthmsg=strlen(msg);
		char *topicMsg2=(char *)malloc(lengthmsg+1);
		topicMsg2=strcpy(topicMsg2,msg);
		topicMsg2[lengthmsg]='\0';
	
		m2.m1_p1=ch2;
		m2.m1_i1=strlen(topicName);

		m2.m1_i2=strlen(msg);
		m2.m1_p2=topicMsg2;
//m3
message m3;
		lengthtn=strlen(topicName);
		char *ch3=(char *)malloc(lengthtn+1);
		ch3=strcpy(ch3,topicName);
		ch3[lengthtn]='\0';
		
		 lengthmsg=strlen(msg);
		char *topicMsg3=(char *)malloc(lengthmsg+1);
		topicMsg3=strcpy(topicMsg3,msg);
		topicMsg3[lengthmsg]='\0';
	
		m3.m1_p1=ch3;
		m3.m1_i1=strlen(topicName);

		m3.m1_i2=strlen(msg);
		m3.m1_p2=topicMsg3;
message m4;
					lengthtn=strlen(topicName);
					char *ch4=(char *)malloc(lengthtn+1);
					ch4=strcpy(ch4,topicName);
					ch4[lengthtn]='\0';
		
					 lengthmsg=strlen(msg);
					char *topicMsg4=(char *)malloc(lengthmsg+1);
					topicMsg4=strcpy(topicMsg4,msg);
					topicMsg4[lengthmsg]='\0';
			
					m4.m1_p1=ch4;
					m4.m1_i1=strlen(topicName);
			
					m4.m1_i2=strlen(msg);
					m4.m1_p2=topicMsg4;
	int retval;
	int time=100;
	int semGot=_syscall(PM_PROC_NR,PUBLISHACCESS,&m);

	message m1;
	lengthtn=strlen(topicName);
	char *ch1=(char *)malloc(lengthtn+1);
	ch1=strcpy(ch1,topicName);
	ch1[lengthtn]='\0';

	 lengthmsg=strlen(msg);
	char *topicMsg1=(char *)malloc(lengthmsg+1);
	topicMsg1=strcpy(topicMsg1,msg);
	topicMsg1[lengthmsg]='\0';
	
	m1.m1_p1=ch1;
	m1.m1_i1=strlen(topicName);

	m1.m1_i2=strlen(msg);
	m1.m1_p2=topicMsg1;
	if(semGot==-1)
	{
		return -1;
	}
	if(semGot)
	{	
		retval=_syscall(PM_PROC_NR,PUBLISH,&m1);
		if(retval==2)
		{
			printf("\n not registered to publish\n");
			_syscall(PM_PROC_NR,RELEASESEM,&m2); 	
			return retval;
		}
		if(retval==-1)
		{
			printf("\ncouldnt publish either no subscribers or some other publisher is publishing\n");
			_syscall(PM_PROC_NR,RELEASESEM,&m2); 	
			return retval;
		}
		else if(retval==0)
		{
			//printf("\npublished now trying to releasing sem\n");
			_syscall(PM_PROC_NR,RELEASESEM,&m2);
			return retval;
		}
		//fullcase
		else if(retval==1)
		{
				
			//printf("\nreached full case going to sleep\n");
			//block untill flag is 1
			sleep(10);
			retval=_syscall(PM_PROC_NR,PUBLISH,&m2);
			if(retval==1)
			{						
				_syscall(PM_PROC_NR,RELEASESEM,&m3);
				return -2;
			}
	
			else
			{
				_syscall(PM_PROC_NR,RELEASESEM,&m3);
				return retval;
			}
					
		}
		_syscall(PM_PROC_NR,RELEASESEM,&m4);
		return retval;
	}
	else
	{
		while(retval==0&&time>0)
		{
			//block untill retval is 1
			sleep(5);
			time-=10;
			retval=_syscall(PM_PROC_NR,PUBLISHACCESS,&m1);
		}
		if(time==0)
		{
			_syscall(PM_PROC_NR,RELEASESEM,&m2);
			return -1;
		}
		else
		{
			retval=_syscall(PM_PROC_NR,PUBLISH,&m2);
			if(retval==0)
			{
			
			_syscall(PM_PROC_NR,RELEASESEM,&m3);
			return retval;
			}
				//fullcase
			else if(retval==1)
			{
				
				
					//block untill flag is 1
					sleep(10);
					retval=_syscall(PM_PROC_NR,PUBLISH,&m3);
	
					if(retval==1)
					{
						
						_syscall(PM_PROC_NR,RELEASESEM,&m4);

						return -2;
					}
					else
					{
					_syscall(PM_PROC_NR,RELEASESEM,&m4);
					return retval;
					}


			}
			_syscall(PM_PROC_NR,RELEASESEM,&m3);
			return retval;
		}
		
		
	}
}