#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void execute(int index);
void execute3(int index,char *ch,char *msg);
void execute2(int index, char *ch);


//1.topiccreate
//2.topiclookup
//3.topicsubscriber
//4.topicpublisher
//5.pulish
//6.retrieve

void execute(int index)
{
	printf("%s",TopicLookup());
}
void execute3(int index,char *ch,char *msg)
{
	int ret=Publish(ch,msg);
			if(ret==0)
			{
				printf("\n\n");
				printf("\nPublished successfully\n");
			}
			else if(ret==-2)
			{
				printf("\nUnable to publish as the topic messages are full\n");
				

			}
			else if(ret==2)
			{
				printf("\nPublisher not registered\n");
				

			}
			else if(ret==-1)
			{
				printf("\nUnable to publish \n");
				

			}

}
void execute2(int index, char *ch)
{
	switch(index)
	{
	case 3:
	
			if(TopicSubscriber(ch)==0)
			{
				printf("\nadded\n");
			}
			else
			{
				printf("\nnot added\n");
			}
	break;
	case 1:
	TopicCreate(ch);
	break;
	case 4:
	if(TopicPublisher(ch)==0)
			{
				printf("\nadded\n");
			}
			else
			{
				printf("\nnot added\n");
			}
	break;
	case 6:

						
			printf("\n %s \n",Retrieve(ch));
	break;
	}
}
	