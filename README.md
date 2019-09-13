# Process-to-process-communication-platform-for-MINIX-OS-mimicking-JMS-KAFKA
An API which provides a topic-based publisher subscriber communication mechanism for processes. A set of system calls allow user processes to send and receive messages to one another asynchronously.

**Key features:** buffered message passing system to handle communication, deadlock prevention mechanism

**The following system calls are implemented:**

. TopicLookup: that will allow a process to discover what interest groups are there

. TopicCreate: that will allow a process to create an interest group

. TopicPublisher: that will allow a process to declare itself a publisher of a specific interest group

. TopicSubscriber: that will allow a process to declare itself a subscriber to an interest group

. Publish: that will allow a publisher to send a message to an interest group

. Retrieve: that will allow a subscriber to retrieve one message from an interest group

**Functionality**
* While a publisher is sending to a topic, no other user can publish to or retrieve from the topic. However, multiple subscribers can be retrieving messages from the same topic at the same time.
* A buffer is maintained that can contain 5 messages for each topic. No publisher can send to the topic when the buffer is full. On the other hand, retrieve is nonblocking. A message is removed from the buffer after it has been retrieved by all the subscribers.
* A message is published once and retrieved by all subscribers who had subscribed to the topic before the message was published, at least once and at most once.
* There is no occurrence of deadlock as condition of deadlock is detected which is - publisher publishing till the buffer is full when no subscribers have registered. Avoided by not allowing Publishers to publish when there are no subscribers registered before it starts to publish.
