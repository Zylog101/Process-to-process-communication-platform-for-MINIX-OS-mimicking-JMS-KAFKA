#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	char *ch = (char *)malloc(10);
	//scanf("%s",ch);
	//printf("%s\n",ch);
	//TopicCreate(ch);
	//printf("%s",TopicLookup());
	
	int selection;
	char *message;
	
	int pid;
	while(1)
	{
		printf("\n1)topicLookup \n 2)topicCreate\n 3)addTopicPublisher\n 4)addTopicSubscriber\n 5)publish\n 6)retrieve\n");
		scanf("%d",&selection);
		switch(selection)
		{
			case 1:
			printf("%s",TopicLookup());
			break;
			
			case 2:
			printf("\nEnter the name of Topic to create\n");
			scanf("%s",ch);
			TopicCreate(ch);
			break;
			case 3:
			//note user should have already created a topic here
			
			printf("\nEnter  a topic name from list\n");
			printf("%s",TopicLookup());
			scanf ("%s",ch);
			if(TopicPublisher(ch)==0)
			{
				printf("\nadded\n");
			}
			else
			{
				printf("\nnot added\n");
			}
			break;

			case 4:
			//note user should have already created a topic here
			
			printf("\nEnter  a topic name from list\n");
			printf("%s",TopicLookup());
			scanf ("%s",ch);

			
			if(TopicSubscriber(ch)==0)
			{
				printf("\nadded\n");
			}
			else
			{
				printf("\nnot added\n");
			}
			
			break;		

			case 5:
			
			message=(char *)malloc(sizeof(char)*10);
			printf("\nEnter the message to be published\n");
			scanf ("%s",message);
			
			printf("\nEnter a topic name from list\n");
			printf("%s",TopicLookup());
			scanf("%s",ch);
			
			int ret=Publish(ch,message);
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

			break;
			case 6:
			printf("\nEnter  a topic name from list\n");
			printf("%s",TopicLookup());
			scanf ("%s",ch);
						
			printf("Messages: %s \n",Retrieve(ch));

			
			break;


			default:
			return 0;
		}
}
}