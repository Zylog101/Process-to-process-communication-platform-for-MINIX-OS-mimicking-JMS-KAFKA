#include<stdio.h>
#include "pm.h" 
#include<string.h>
#include<stdlib.h>
#include "TopicHandler.h"
//todo:handle concurrent topiclookup clall as there will be call twice 
int do_topiclookup(void)
{
	
	char *ch=(char *)malloc(sizeof(char *));
	char *tempch=getAllTopicName();	
	ch=realloc(ch,strlen(tempch)+1);
	ch=strcpy(ch,tempch);
	int length =strlen(ch);
	ch[length+1]='\0';
	length=length+1;
	
	sys_datacopy(SELF,(vir_bytes)ch,who_e,(vir_bytes)m_in.m1_p1,length);
		
	
	//printf("\n Pm call: %d %s \n",length,ch);
	//m_in.m1_p1=(char *)malloc(length+1);
	//sys_datacopy(SELF,(vir_bytes)length,who_e,(vir_bytes)m_in.m1_i1,sizeof(int));


	return 0;
}
int do_topiccreate(void)
{	
	int length = m_in.m1_i1;
	char *ch = (char *)malloc((sizeof(char)*length+1));
	message msg;
	sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF,
	(vir_bytes)ch , length+1);
	ch[length+1]='\0';
	topicCreate(ch);

	return 0;
}

int do_topic_publisher(){
    
    int length = m_in.m1_i1;    // provides length of the message
    
    char *ch = (char *)malloc((sizeof(char)*length)+1);    // dynamically allocate the memory whose length is equal to length of in-coming message
    
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF,
                 (vir_bytes)ch , length+1); 
	ch[length]='\0';
    struct Topic *tempTopic=searchTopic(ch);
	if(tempTopic==NULL)
	{
		printf("\nTopic doesnt exit\n");
		//failed to add
		return 1;
	}
    //need to get the pid of the sender
    int addSuccess=addTopicPublisher(tempTopic,who_e);	
    //printf("System call: do_topic_publisher called with value %s\n",ch);
    if(addSuccess!=0)
	{
		printf("\nAlready A publisher\n");
	}
    return addSuccess;
}

int do_topic_subscriber()
{
    
    int length = m_in.m1_i1;    // provides length of the message
    char *ch = (char *)malloc((sizeof(char)*length));    // dynamically allocate the memory whose length is equal to length of in-coming message
    
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF,
                 (vir_bytes)ch , length+1); 
	ch[length]='\0';    struct Topic *tempTopic=searchTopic(ch);
	if(tempTopic==NULL)
	{	
	printf("\nTopic doesnt exit\n");
		//failed to add
		return 1;
	}
    //need to get the pid of the sender
    	
int addSuccess=addTopicSubscriber(tempTopic,who_e);
    //printf("System call: do_topic_publisher called with value %s\n",ch);
    if(addSuccess!=0)
	{
		printf("\nAlready A subscriber\n");
		return addSuccess;
	}
    
    return 0;
    
}
//ret 3 on search topic fail 
//ret 0 on success 1 on topic message full and 2 on pid is not a publisher
int do_publish(){
    int length_TN = m_in.m1_i1;    // provides length of the topic name
    
    int length_msg = m_in.m1_i2;    // provides length of the message
    
    char *ch = (char *)malloc((sizeof(char)*length_TN+1));    // dynamically allocate the memory whose length is equal to length of topic name.
    
    char *pub_msg = (char *)malloc((sizeof(char)*length_msg+1));    // dynamically allocate the memory whose length is equal to length of message.
    
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)ch , length_TN+1); // sys_datacopy(SELF, src_addr, dst_proc, dst_addr, len) Copying topicName from user space to PM
    
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p2, SELF, (vir_bytes)pub_msg , length_msg+1); //Copying the message from user space to PM
    ch[length_TN]='\0';
    pub_msg[length_msg]='\0';
	printf("\nin syscall %d %s",length_TN,ch);
    struct Topic *TA = searchTopic(ch); //Search whether the given topic exites or not. If it exists, TA holds address of that topic otherwise NULL
    if(TA==NULL)
    {
	//Todo:should remove
	printf("topic not found.\n");
	return 3;
    }   
	int pubSuccess=publish(TA, pub_msg, who_e); //Publish the message to the topic if process is a publisher to the topic else return simply 
	if(pubSuccess==0)
	{
	printf("\nSuccessfully Published\n");
	}
	else if(pubSuccess==1)
	{
	return 1;
	}
	else if(pubSuccess==-1)
	{
	return -1; 
	}
	else
	{
	printf("\nPid is not a pulisher under this topic\n");
	}
    return pubSuccess;
}

//ret 0 on success 1 on failure 
int do_retrieve(){
    
    int length_TN = m_in.m1_i1;    // provides length of the topic namw
    
    char *ch = (char *)malloc((sizeof(char)*length_TN+1));    // dynamically allocate the memory whose length is equal to length of topic name.
    
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)ch , length_TN+1); // sys_datacopy(SELF, src_addr, dst_proc, dst_addr, len) Copying topicName from user space to PM
    ch[length_TN]='\0';
    struct Topic *TA = searchTopic(ch); //Search whether the given topic exites or not. If it exists, TA holds address of that topic otherwise NULL
	char *cha=(char *)malloc(sizeof(char *));
    if(TA!=NULL)
    {
	
	if(TA->semaphore==0)
	{
		char errormsg[]="cannot retreive at this moment as the Topic is being updated";
		int length=strlen(errormsg);
		cha = realloc(cha,length+1);
		strcpy(cha,errormsg);
		cha[length]='\0';
		sys_datacopy(SELF, (vir_bytes)cha, who_e, (vir_bytes)m_in.m1_p2, length);   //Copies msg to m_in.m1_p2 so that it can be accessed in user space.
		return 1;

	}
	
	char *msg = retrieve(TA, who_e); // Retrieves the message from this topic if the process has subscribed to the topic before the publish happend.
	//printf("inside%s",msg);
	cha=realloc(cha,strlen(msg )+1);
	cha=strcpy(cha,msg );
	int length =strlen(cha);
	cha[length]='\0';
	length=length+1;
	//printf("%s",cha);
	sys_datacopy(SELF, (vir_bytes)cha, who_e, (vir_bytes)m_in.m1_p2, length);   //Copies msg to m_in.m1_p2 so that it can be accessed in user space.
	return 0;
   }
	char errormsg[]="Topic Not Found";
		int length=strlen(errormsg);
		cha = realloc(cha,length+1);
		cha=strcpy(cha,errormsg);
		cha[length]='\0';
		sys_datacopy(SELF, (vir_bytes)cha, who_e, (vir_bytes)m_in.m1_p2, length);   //Copies msg to m_in.m1_p2 so that it can be accessed in user space.
		return 1;    
   
    
}
int do_getPublishAccessOfTopic(){
    int s;
	int length_TN = m_in.m1_i1; 
 char *ch = (char *)malloc((sizeof(char)*length_TN+1));    // dynamically allocate the memory whose length is equal to length of topic name.
  sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)ch , length_TN+1); 
 ch[length_TN]='\0';
	  struct Topic *TA = searchTopic(ch); //Search whether the given topic exites or not. If it exists, TA holds address of that topic otherwise NULL
    if(TA==NULL)
    {printf("in getpublishaccess\n");
	return -1;
    }
    s = getPublishAccessOfTopic(TA);
    return s;
}
int do_releaseSem()
{printf("\nIn release semophore\n");
int length_TN = m_in.m1_i1;    // provides length of the topic name
        
  
 char *ch = (char *)malloc((sizeof(char)*length_TN+1));    // dynamically allocate the memory whose length is equal to length of topic name.
  sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)ch , length_TN+1); 
 ch[length_TN]='\0';

         struct Topic *TA = searchTopic(ch);
    	releaseSem(TA);
	return 0;
}
