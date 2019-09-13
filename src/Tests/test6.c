#include"testExecuter.h"
//creates a topic publish 5 messages retreive all messages
int main()
{
    execute2(1,"gello");
    execute (2);
    
    execute2(3,"gello");
    execute2(4,"gello");
	int i=5;
	while(i>0)
	{
        execute3(5,"gello","testing");
	i--;
	}
	
	execute2(6,"gello");
	   
}
