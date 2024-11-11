Semaphores are synchronization tools that control access to a shared resource by multiple threads. In Java (and most programming contexts), there are primarily two types of semaphores:

1. Binary Semaphore
A binary semaphore has only two possible values: 0 and 1.
It acts as a simple lock, similar to a mutex, where:
1 (or available permit) means the resource is free.
0 (or no available permit) means the resource is locked.
Use Case: To provide mutual exclusion, allowing only one thread to access a critical section at a time.

Syntax
In Java, binary semaphores are created by initializing a Semaphore with 1 permit.

java
Copy code
import java.util.concurrent.Semaphore;

Semaphore binarySemaphore = new Semaphore(1);


----------------------------------------------------------------------------------------------------------------------------------


2. Counting Semaphore
A counting semaphore allows more than one thread to access a resource up to a specified limit.
The count represents the number of threads that can concurrently access the resource.
Use Case: Managing access to a pool of resources, like database connections, where a limited number of resources are available.
Syntax
In Java, counting semaphores are created by initializing a Semaphore with a specified number of permits.

java
Copy code
import java.util.concurrent.Semaphore;

Semaphore countingSemaphore = new Semaphore(3);

---------------------------------------------------------------------------------------------------------------------------------


Step-by-Step Explanation of Semaphore Value Changes
Let's go through each step in detail:

Initial Semaphore Values:

prod = 1: Allows one thread to access the buffer.
con = 0: No items are in the buffer initially, so consumers will wait until con is released.
limit = 2: Buffer has two available slots, allowing up to two items to be produced before blocking.
Producer Produces Item 0:

limit.acquire(): Decreases limit to 1 (indicating one empty slot is available).
prod.acquire(): Decreases prod to 0, locking buffer access.
Action: Producer adds item 0 to the buffer.
prod.release(): Increases prod back to 1, unlocking the buffer.
con.release(): Increases con to 1, signaling that there is an item available for consumption.
State after Step 2:

Buffer: [0]
prod = 1, con = 1, limit = 1
Consumer Consumes Item 0:

con.acquire(): Decreases con to 0, indicating no items left for other consumers until the next production.
prod.acquire(): Decreases prod to 0, locking buffer access.
Action: Consumer removes item 0 from the buffer.
prod.release(): Increases prod to 1, unlocking the buffer.
limit.release(): Increases limit to 2, allowing space for another item in the buffer.
State after Step 3:

Buffer: []
prod = 1, con = 0, limit = 2
Producer Produces Item 1:

limit.acquire(): Decreases limit to 1.
prod.acquire(): Decreases prod to 0.
Action: Producer adds item 1 to the buffer.
prod.release(): Increases prod to 1.
con.release(): Increases con to 1.
State after Step 4:

Buffer: [1]
prod = 1, con = 1, limit = 1
Consumer Consumes Item 1:

con.acquire(): Decreases con to 0.
prod.acquire(): Decreases prod to 0.
Action: Consumer removes item 1 from the buffer.
prod.release(): Increases prod to 1.
limit.release(): Increases limit to 2.
State after Step 5:

Buffer: []
prod = 1, con = 0, limit = 2
The code will continue in this cycle, with each semaphore controlling the access and synchronization between the producer and consumer.