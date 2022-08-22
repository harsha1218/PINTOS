
/* Tests producer/consumer communication with different numbers of threads.
 * Automatic checks only catch severe problems like crashes.
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

//The word which has to be produced by the producer into the buffer and has to be read and printed by the consumer.
char* word="Hello World";
//These two variables are used to keep track of the number of characters that producer has put into the buffer and the consumer has printed from the buffer.
int pos_prod=0,pos_cons=0;
//The variable for the length of the string.
int str_length=11;
//Variable that for length of the buffer.
const int buff_size=7;
// Buffer for the characters.
char buffer[7];
//These variables will act as the indices of buffer to which producer must put to or consumer must print from.
int for_producer= 0;
int for_consumer= 0;
//This variable is used to check if the buffer is full or empty.
int checker= 0;

//This is the put function that is used by Producer.
void put(char value) {
    buffer[for_producer] = value;
    for_producer = (for_producer + 1) % buff_size;
    pos_prod++;
    checker++;
}
//This is the get function that is used by consumer.
char get() {
    char tmp = buffer[for_consumer];
    for_consumer = (for_consumer + 1) % buff_size;
    checker--;
    pos_cons++;
    return tmp;
}
//This is the initialization of condition variables and the lock which will be used for this program.
struct condition empty;
struct condition fill;
struct lock lock;

//This is the producer funtion that a thread produced in the producer loop will call.
void *producer(void *args) {
    while(pos_prod<str_length){
        lock_acquire(&lock); 
        while (checker == buff_size && pos_prod<str_length) 
            cond_wait (&empty,&lock); 
        if(pos_prod<str_length){
            put(word[pos_prod]); 
            cond_signal (&fill,&lock);
            lock_release(&lock);
        }     
        else{
            cond_signal (&fill,&lock);
            lock_release(&lock);
        }
    }
}

//This is the consumer funtion that a thread produced in the consumer loop will call.
void *consumer(void *arg) {
    while(pos_cons<str_length){
        lock_acquire(&lock); 
        while (checker == 0 && pos_cons<str_length) 
            cond_wait (&fill,&lock);
        if(pos_cons<str_length){
            char tmp = get();
            printf("%c", tmp);
            cond_signal (&empty,&lock);
            lock_release(&lock);
        }
        else{
            cond_signal (&empty,&lock);
            lock_release(&lock);
        }
    }
}



void producer_consumer(unsigned int num_producer, unsigned int num_consumer);


void test_producer_consumer(void)
{
    /*producer_consumer(0, 0);
    producer_consumer(1, 0);
    producer_consumer(0, 1);
    producer_consumer(1, 1);
    producer_consumer(3, 1);
    producer_consumer(1, 3);
    producer_consumer(4, 4);
    producer_consumer(7, 2);
    producer_consumer(2, 7);*/
    producer_consumer(1, 10);
    pass();
}


void producer_consumer(UNUSED unsigned int num_producer, UNUSED unsigned int num_consumer)
{
//Here, we are creating an array of tid_t to spawn threads for given number of producers and consumers.
    tid_t produce[num_producer],consume[num_consumer];
//Here, we are initializing the conditional variables and the lock for this program which were initialized globally.
    cond_init(&empty); cond_init(&fill);lock_init(&lock);
// Following are the loops which are used to spawn threads.
    for(unsigned int i=0;i<num_producer;i++){
        produce[i]=thread_create("thisone",PRI_DEFAULT,producer,NULL);
    }
    for(unsigned int i=0;i<num_consumer;i++){
        consume[i]=thread_create("thisone",PRI_DEFAULT,consumer,NULL);
    }
}
