
struct Topic
{
  char *name;
  int writeIndex;
  struct Tmessage *tMessage[5];
  struct Publisher *pStartnode;
  struct Subscriber *sStartnode;
  int cntOfSubscribers;
  int availableMsgSlots;
  int semaphore;  
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
  int readIndex;
  int noOfMessagesToBeRead;
  struct Subscriber *nextnode;
};

struct Tmessage 
{
  int cnt;
  char *msg;
};

char * getAllTopicName( void );
void topicCreate(char *topicName);
struct Topic * searchTopic(char *topicName);
int addTopicPublisher(struct Topic *topic, int publisherPid);
int addTopicSubscriber(struct Topic *topic,int pid);
char * retrieve(struct Topic *topic,int curr_pid);
int publish(struct Topic *topic, char *ch,int curr_pid);
int getPublishAccessOfTopic(struct Topic *topic);
int releaseSem(struct Topic *topic);