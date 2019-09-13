#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TopicHandler.h"
#include<semaphore.h>


/*struct Topic
{
  char *name;
  int writeIndex;
  int readIndex;
  struct Tmessage *tMessage[5];
  struct Publisher *pStartnode;
  struct Subscriber *sStartnode;
  struct Topic *nextNode;
};

struct Publisher
{
  int pid;
  struct Publisher *nextnode;
};

struct Subscriber
{
  int pid;
  struct Subscriber *nextnode;
};

struct Tmessage 
{
  int cnt;
  char *msg;
};
*/
struct Topic *startNode=NULL;

//returns string consisting of all topic names present
char * getAllTopicName( void )
{
	//String will hold all topic names seperated by newline char
	char *topicNames; 

	int sizeOfTopicNames=sizeof(char);
	int indexInTopicNames=0;
	topicNames=(char *)malloc(sizeof(char *));
	struct Topic *traverseNode=startNode;

	//case where there is no topics
	if(traverseNode==NULL)
	{
		char ch[]="No Topic Exists\n";
		int leng=strlen(ch);
		topicNames=realloc(topicNames,leng+1);
		topicNames=strcpy(topicNames,ch);
		topicNames[leng]='\0';
		return topicNames;
	}
	
	//initialize length of the topic name
	int lengthOfName=strlen(traverseNode->name)+1;

	//creating memory to accomodate the the new name and copying new name to topicNames string
	topicNames=realloc(topicNames,sizeOfTopicNames+lengthOfName);
	//printf("%s",traverseNode->name);
	topicNames=strcpy(topicNames,traverseNode->name);
	topicNames=strcat(topicNames,"\n");
	
	//updating string index and size
	indexInTopicNames+=lengthOfName;
	sizeOfTopicNames+=lengthOfName;

	traverseNode=traverseNode->nextNode;
	
	//traversing other topics to add to topicNames
	while(traverseNode!=NULL)
	{
		//initialize length of the topic name
		lengthOfName=strlen(traverseNode->name)+1;

		//creating memory to accomodate the the new name and copying new name to topicNames string
		topicNames=realloc(topicNames,sizeOfTopicNames+lengthOfName);
		//printf("%s",traverseNode->name);
		
		topicNames=strcat(topicNames,traverseNode->name);
		topicNames=strcat(topicNames,"\n");

		//updating string index and size
		indexInTopicNames+=lengthOfName;
		sizeOfTopicNames+=lengthOfName;

		traverseNode=traverseNode->nextNode;
		
	}
	
	topicNames[indexInTopicNames]='\0';
	return topicNames;
}

void topicCreate(char *topicName)
{
	struct Topic *traverseNode=NULL;

	//creating a new node
	struct Topic *newTopicNode=(struct Topic *)malloc(sizeof(struct Topic));

	//creating the topicName string
	int length=strlen(topicName);
	char *newTopicName=(char *)malloc(sizeof(char)*(length+1));
	newTopicName=strcpy(newTopicName,topicName);
	newTopicName[length]='\0';

	//initializing the members of new node
	newTopicNode->name=newTopicName;
	newTopicNode->writeIndex=0;
	//need to think of cases handling this variable hint:this has to be updated on message removal
	newTopicNode->cntOfSubscribers=0;
	newTopicNode->availableMsgSlots=5;
	newTopicNode->semaphore=1;
	newTopicNode->pStartnode=NULL;
	newTopicNode->sStartnode=NULL;
	for(int i=0;i<5;i++)
	{
		newTopicNode->tMessage[i]=NULL;
	}
	newTopicNode->nextNode=NULL;

	//First Topic node case
	if(startNode==NULL)
	{	
		startNode=newTopicNode;
		return;
	}
	
	//Multiple topics already present case
	traverseNode=startNode;
	if(traverseNode->nextNode==NULL)
	{
		if(strcmp(traverseNode->name,topicName)==0)
		{
			//topicName already exists
			//test purposses
			printf("topic Name already exists\n");
			return;
		}
		traverseNode->nextNode=newTopicNode;
		return;
	}
	else
	{
		do 	
		{
			if(strcmp(traverseNode->name,topicName)==0)
			{
				//topicName already exists
				//test purposses
				printf("topic Name already exists");
				return;
			}
			traverseNode=traverseNode->nextNode;
			
		}while(traverseNode->nextNode!=NULL);
		//checking last node
		if(strcmp(traverseNode->name,topicName)==0)
			{
				//topicName already exists
				//test purposses
				printf("topic Name already exists");
				return;
			}

		traverseNode->nextNode=newTopicNode;
		return;
	}
	
	
}

//Function to search existing topic by name.
//Returns the topic node address from the topic list if found otherwise returns NULL
struct Topic * searchTopic(char *topicName){
    
    struct Topic *topic;    
    topic = startNode;
    
    while (topic!=NULL) {
       	    if(strcmp(topic->name, topicName)==0){
	    //Todo:remove comments later
            //printf("topic found.\n");
            return(topic); // Found
        }
        topic = topic->nextNode;
    }
    //Todo:remove comments later
    //printf("topic not found.\n");
    return(NULL); // Not found
    
}

//Creats publisher node and Adds to topic publisher list if not already present
int addTopicPublisher(struct Topic *topic, int publisherPid){
    
    //For the 1st time (when pStartnode is Null), just add pid to head of pStartnode.
    if (topic->pStartnode == NULL) {
        struct Publisher *pub;
        pub=(struct Publisher *)malloc(sizeof(struct Publisher));
        pub->pid=publisherPid;
        pub->nextnode=NULL;
	topic->pStartnode=pub;
	return 0;
    }
    
    //Check if pid is already a publisher or not
    //If already a pub, return 1 saying you are already a publisher.
    //Else add a node to publisher return 0.
    
    struct Publisher *tempPublisher;
  	
        tempPublisher = topic->pStartnode;
	//Create and init new node
	struct Publisher *pub;
        pub=(struct Publisher *)malloc(sizeof(struct Publisher));
        pub->pid=publisherPid;
        pub->nextnode=NULL;
	
	if(tempPublisher->nextnode==NULL)
	{
		if(tempPublisher->pid==publisherPid)
		{
			//printf("Current process(%d) is already a publisher under this topic",publisherPid);
                	return(1);
		}
		tempPublisher->nextnode=pub;
		return(0);	
	}
    else{
        	do
        	{
	            	if(tempPublisher->pid == publisherPid){   //Found
                	//printf("Current process(%d) is already a publisher under this topic",publisherPid);
        	        return(1);
            		}
            
            		tempPublisher = tempPublisher->nextnode;
        	}while(tempPublisher->nextnode!=NULL);
	//check last node
        if(tempPublisher->pid == publisherPid){   //Found
                	//printf("Current process(%d) is already a publisher under this topic",publisherPid);
        	        return(1);
            		}

        //Not found, then add a new node to tempPublisher->nextnode
	tempPublisher->nextnode = pub;
        return 0;
    }
}

int addTopicSubscriber(struct Topic *topic,int pid)
{ 
	struct Subscriber *tempSubscriber;
        tempSubscriber = topic->sStartnode;
	struct Subscriber *sub;
        sub=(struct Subscriber *)malloc(sizeof(struct Subscriber));
        sub->pid=pid;
	sub->readIndex=topic->writeIndex;
	//not sure if always this is true TODO: this has to be handled
	sub->nextnode=NULL;
	sub->noOfMessagesToBeRead=0;	
	topic->cntOfSubscribers++;

   if (tempSubscriber==NULL)
    {                      
            topic->sStartnode=sub ;
            return 0;
    }

	if(tempSubscriber ->nextnode==NULL)
	{
		if(tempSubscriber->pid == pid)
		{   //Found
                	//printf("Current process(%d) is already a subscriber under this topic\n",pid);
                	return(1);
            	}
		tempSubscriber ->nextnode=sub;
		return(0);	
	}

//*if there are no subscriber ,insert one*//  
    else{
        
        do
        {
            if(tempSubscriber->pid == pid){   //Found
                //printf("Current process(%d) is already a subscriber under this topic\n",pid);
                return(1);
            }
            
            tempSubscriber = tempSubscriber->nextnode;
        }while(tempSubscriber->nextnode!=NULL);
	//check last node
	 if(tempSubscriber->pid == pid){   //Found
               // printf("Current process(%d) is already a subscriber under this topic\n",pid);
                return(1);
            }

        tempSubscriber->nextnode = sub;
   	return 0;
  }
}

//Retrieve a message from the topic
char * retrieve(struct Topic *topic,int curr_pid){
    
    //Check if curr_pid is subscriber of this topic
    //If it is subscriber, then retrieve the message and send to subscriber(return msg)
    //Else return 0 saying curr_pid is NOT a subscriber to this topic.
	
    struct Subscriber *tempSubscriber = topic->sStartnode;
    while(tempSubscriber!=NULL)
    {
        if(tempSubscriber->pid == curr_pid)
	{  
            //for dubug purposses
           // printf("Current process(%d) is subscriber under this topic\n",curr_pid);
		if(tempSubscriber->noOfMessagesToBeRead==0)
		{
			char ch[]="\nNo messages to retreive\n";
			int leng=strlen(ch);
			char *topicMessages=(char *)malloc(leng+1);
			topicMessages=strcpy(topicMessages,ch);
			topicMessages[leng]='\0';
			return topicMessages;
		}
            struct Tmessage *tm = (struct Tmessage *)malloc(sizeof(struct Tmessage));
		
		char *topicMessages=(char *)malloc(sizeof(char *));
		topicMessages=strcpy(topicMessages," ");

		int indexIntopicMessages=0;
		int sizeOftopicMessages=sizeof(char);
		for(int iteration=tempSubscriber->noOfMessagesToBeRead;iteration>0;iteration--)
		{
			tm=topic->tMessage[tempSubscriber->readIndex];
			//String will hold all topic names seperated by space char
			int lengthOfName=strlen(tm->msg)+1;

			//creating memory to accomodate the the new name and copying new name to topicMessages string
			topicMessages=realloc(topicMessages,sizeOftopicMessages+lengthOfName);
						
			topicMessages=strcat(topicMessages,tm->msg);
			topicMessages=strcat(topicMessages," ");
			tm->cnt=tm->cnt-1;
			tempSubscriber->noOfMessagesToBeRead--;
			//updating string index and size
			indexIntopicMessages+=lengthOfName;
			sizeOftopicMessages+=lengthOfName;
			tempSubscriber->readIndex=(tempSubscriber->readIndex+1)%5;
		}
		topicMessages[indexIntopicMessages]='\0';
		return topicMessages;
            
        }
        
        tempSubscriber = tempSubscriber->nextnode;
    }
		//case where no messages are present
		char ch[]="You have not subscribed to this topic\n";
		int leng=strlen(ch);
		char *topicMessages=(char *)malloc(leng+1);
		topicMessages=strcpy(topicMessages,ch);
		topicMessages[leng]='\0';
		return topicMessages;
	
}

void updateSubscribersOfNewMessage(struct Topic *topic)
{
	struct Subscriber *tempSub=topic->sStartnode;
	while(tempSub!=NULL)
	{
		tempSub->noOfMessagesToBeRead++;
		tempSub=tempSub->nextnode;	
	}
}
void printAllMessages(struct Topic *topic)
{
	
	for(int i=0;i<5;i++)
	{
		if(topic->tMessage[i]!=NULL)
		printf("%s ",topic->tMessage[i]->msg);
		else
		printf("(NULL)");

	}
}
//Publish a msg to a topic
//ret 0 on success 1 on TopicMessage full and 2 on pid is not a publisher to this topic
int publish(struct Topic *topic, char *ch,int curr_pid){
    struct Publisher *tempPublisher;
    if(topic->cntOfSubscribers==0)
	{
		return -1;
	}
    tempPublisher = topic->pStartnode;
    //Check if curr_pid is publisher of this topic
    //If it is publisher, then publish the message to the topic and return 0 on success.
    //Else return 1 saying curr_pid is NOT a publisher to this topic.
     if(topic->availableMsgSlots == 0 )
    {
	//need to handle deletion of messages
    	//printf("Topic messages are full. Please try again after some time. \n");
	 if(topic->tMessage[topic->writeIndex]->cnt==0)
	{
		topic->tMessage[topic->writeIndex]=NULL;
		 while(tempPublisher!=NULL)
 		   {
       			 if(tempPublisher->pid == curr_pid){  
	   		 //for dubug purposses
            		//printf("Current process(%d) is a publisher under this topic",curr_pid);
            
           		 struct Tmessage *tm = (struct Tmessage *)malloc(sizeof(struct Tmessage));
           		 tm->msg = ch;
           		 tm->cnt = topic->cntOfSubscribers;
          		  topic->tMessage[topic->writeIndex] = tm;
				//printf("%s",topic->tMessage[topic->writeIndex]->msg);
           		 topic->availableMsgSlots=0;
	    		topic->writeIndex=(topic->writeIndex+1)%5;
			//printf("\nwriteIndex:%d\n",topic->writeIndex);
			//updateSubscribers of the new message
			updateSubscribersOfNewMessage(topic);
			return 0;
			}	   
        
    		    tempPublisher = tempPublisher->nextnode;
  		  }
		return 2;

	}
    	return 1;
    }
    
   
    while(tempPublisher!=NULL)
    {
        if(tempPublisher->pid == curr_pid){  
	    //for dubug purposses
            //printf("Current process(%d) is a publisher under this topic",curr_pid);
            
            struct Tmessage *tm = (struct Tmessage *)malloc(sizeof(struct Tmessage));
            tm->msg = ch;
		printf("\n%s\n",tm->msg);
            tm->cnt = topic->cntOfSubscribers;
            topic->tMessage[topic->writeIndex] = tm;
		//printf("%s",topic->tMessage[topic->writeIndex]->msg);
            topic->availableMsgSlots--;
	    topic->writeIndex=(topic->writeIndex+1)%5;
		//printf("\nwriteIndex:%d\n",topic->writeIndex);
		//updateSubscribers of the new message
		updateSubscribersOfNewMessage(topic);
		return 0;
	}	   
        
        tempPublisher = tempPublisher->nextnode;
    }
    //printf("Current process(%d) is NOT a publisher under this topic",curr_pid);
    return 2;
}
int getPublishAccessOfTopic(struct Topic *topic){
   
    if(topic->semaphore == 0)
        return 0;
    else
	{
		topic->semaphore=0;
        	return 1;
	}
}
int releaseSem(struct Topic *topic)
{
	topic->semaphore=1;
	return 1;
}
/*
int main()
{
	int selection;
	char ch[10];
	int pid;
	char *message;
	while(1)
	{
		printf("\n1)topicLookup \n 2)topicCreate\n 3)searchTopic\n 4)addTopicPublisher\n 5)addTopicSubscriber\n 6)retreive\n 7)publish 8)print all messages in a topic\n 9)exit\n");
		scanf("%d",&selection);
		switch(selection)
		{
			case 1:
			printf("%s",getAllTopicName());
			break;
			
			case 2:
			printf("\nEnter the name of Topic to create\n");
			scanf("%s",ch);
			topicCreate(ch);
			
			break;
			
			case 3:
			printf("\nEnter the name of Topic to find\n");
			scanf("%s",ch);
			searchTopic(ch);
			break;
			
			case 4:
			//note user should have already created a topic here
			
			printf("\nEnter publisher id\n");
			scanf("%d",&pid);
			printf("\nEnter  a topic name from list\n");
			printf("%s",getAllTopicName());
			scanf ("%s",ch);
			struct Topic *topic=searchTopic(ch);
			if(topic==NULL)
			{
				printf("entered topic is invalid \n");
			}
			if(addTopicPublisher(topic,pid)==0)
			{
				printf("successfully added\n");
				//printf("Inserted Into P List %d",topic->pStartnode->pid);
			}
			break;

			case 5:
			//note user should have already created a topic here
			printf("\nEnter subscriber id\n");
			scanf("%d",&pid);
			printf("\nEnter  a topic name from list\n");
			printf("%s",getAllTopicName());
			scanf ("%s",ch);
			struct Topic *atopic=searchTopic(ch);
			if(atopic==NULL)
			{
				printf("entered topic is invalid \n");
			}
			if(addTopicSubscriber(atopic,pid)==0)
			{
				printf("successfully added\n");
				//printf("Inserted Into S List %d",atopic->sStartnode->pid);
			}
			break;	

			case 6:
			
			printf("\nEnter  a topic name from list\n");
			printf("%s",getAllTopicName());
			scanf ("%s",ch);
			struct Topic *btopic=searchTopic(ch);
			if(btopic==NULL)
			{
				printf("entered topic is invalid \n");
			}
			else
			{
				printf("Messages: %s \n",retrieve(btopic,1));

			}
			break;
	
			case 7:
			message=(char *)malloc(sizeof(char)*10);
			printf("\nEnter the message to be published\n");
			scanf ("%s",message);
			
			printf("\nEnter a topic name from list\n");
			printf("%s",getAllTopicName());
			scanf("%s",ch);
			struct Topic *ctopic=searchTopic(ch);
			if(ctopic==NULL)
			{
				printf("entered topic is invalid \n");
			}
			else
			{
				//printAllMessages(ctopic);
				publish(ctopic,message,1);
				//printAllMessages(ctopic);

			}
			break;
			
			case 8:
			printf("\nEnter a topic name from list\n");
			printf("%s",getAllTopicName());
			scanf("%s",ch);
			struct Topic *xtopic=searchTopic(ch);
			if(xtopic==NULL)
			{
				printf("entered topic is invalid \n");
			}
			else
			{
				printAllMessages(xtopic);

			}
			break;

			default:
			return 0;
		}
	}
	
}
*/