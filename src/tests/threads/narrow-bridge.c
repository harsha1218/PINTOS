/* Tests producer/consumer communication with different numbers of threads.
* Automatic checks only catch severe problems like crashes.
*/
 
#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
 
struct semaphore left,right,for_lock,for_lock_process;
 
unsigned int le,ln,re,rn,a;
void narrow_bridge(unsigned int num_vehicles_left, unsigned int num_vehicles_right,
       unsigned int num_emergency_left, unsigned int num_emergency_right);
 
 
void test_narrow_bridge(void)
{
   /*narrow_bridge(0, 0, 0, 0);
   narrow_bridge(1, 0, 0, 0);
   narrow_bridge(0, 0, 0, 1);
   narrow_bridge(0, 4, 0, 0);
   narrow_bridge(0, 0, 4, 0);
   narrow_bridge(3, 3, 3, 3);
   narrow_bridge(4, 3, 4 ,3);
   narrow_bridge(7, 23, 17, 1);
   narrow_bridge(40, 30, 0, 0);
   narrow_bridge(30, 40, 0, 0);
   narrow_bridge(23, 23, 1, 11);
   narrow_bridge(22, 22, 10, 10);
   narrow_bridge(0, 0, 11, 12);
   narrow_bridge(0, 10, 0, 10);*/
   narrow_bridge(0, 10, 10, 0);
   pass();
}
 
void arrive_bridge(int direc,int prio){
   if(direc==1){
       struct semaphore* here=&left;
       while((here->value)<3){
           //
       }
       sema_down(&right);
   }
   else{
       struct semaphore* here=&right;
       while((here->value)<3){
           //
       }
       sema_down(&left);
   }
   sema_down(&for_lock_process);
   return;
}
void on_bridge(int direc,int prio){
   if(direc==0 && prio==0) printf("Started on Bridge from left to right(No Emergency)\n");
   else if(direc==0 && prio==1) printf("Started on Bridge from left to right(Having Emergency)\n");
   else if(direc==1 && prio==0) printf("Started on Bridge from right to left(No Emergency)\n");
   else if(direc==1 && prio==1) printf("Started on Bridge from right to left(Having Emergency)\n");
   return;
}
void over_bridge(int direc,int prio){
   if(direc==1){
       sema_up(&right);
   }
   else{
       sema_up(&left);
   }
   return;
}
void *onevehi(int direc,int prio){
   //printf("%d %d\n",direc,prio);
   arrive_bridge(direc,prio);
   on_bridge(direc,prio);
   over_bridge(direc,prio);
   sema_up(&for_lock_process);
}
 
void narrow_bridge(UNUSED unsigned int num_vehicles_left, UNUSED unsigned int num_vehicles_right,
       UNUSED unsigned int num_emergency_left, UNUSED unsigned int num_emergency_right)
{
   sema_init(&left,3);
   sema_init(&right,3);
   sema_init(&for_lock,1);
   sema_init(&for_lock_process,1);
   le=num_emergency_left;ln=num_vehicles_left;re=num_emergency_right;rn=num_vehicles_right;
   printf("%d %d %d %d",le,ln,re,rn);
   tid_t ri[re+rn],lp[le+ln];
   sema_down(&for_lock);
   for(int i=0;i<le;i++){
       //printf("123\n");
       lp[i]=thread_create("thisone",PRI_DEFAULT,onevehi,(0,1));
   }
   sema_up(&for_lock);
   sema_init(&left,3);
   sema_init(&right,3);
   sema_down(&for_lock);
   for(int i=0;i<re;i++){
       ri[i]=thread_create("thisone",PRI_DEFAULT,onevehi,(1,1));
   }
   sema_up(&for_lock);
   sema_init(&left,3);
   sema_init(&right,3);
   sema_down(&for_lock);
   for(int i=0;i<rn;i++){
       //printf("1234\n");
       ri[i+re]=thread_create("thisone",PRI_DEFAULT,onevehi,(1,0));
   }
   sema_up(&for_lock);
   sema_init(&left,3);
   sema_init(&right,3);
   sema_down(&for_lock);
   for(int i=0;i<ln;i++){
       lp[i+le]=thread_create("thisone",PRI_DEFAULT,onevehi,(0,0));
   }
   sema_up(&for_lock);
 
  
   /* FIXME implement */
}
