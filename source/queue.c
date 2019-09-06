#include <stdbool.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "queue.h"

void queue_empty(queue_t *const q){
    q->readPointer= 0;   
    q->writePointer=0;
    q->cont=0;
}

bool queue_is_empty(const queue_t *const q){
  //return (q->writePointer==q->readPointer);   //TODO: He de comprobar si esta full???
  return ((q->cont)==0);
}

bool queue_is_full(const queue_t *const q){
  
  return (q->cont==MIDA_BUFF-1);
}

void queue_enqueue(queue_t *const q, uint8_t v){
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    if (!queue_is_full(q)) {
      
      q->queue[q->writePointer]=v;
      /*Si al incrementar el punter sobrepasem la capacitat torna un zero*/
      //++(q->writePointer)==MIDA_BUFF? 0: ++(q->writePointer);
      int a=(q->writePointer)+1;
      a==MIDA_BUFF ? (q->writePointer)=0: ((q->writePointer)+=1);
      (q->cont)+=1;
    }
  }
}

void queue_dequeue(queue_t *const q){
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    if (!queue_is_empty(q)) {
      /*Si al incrementar el punter sobrepasem la capacitat torna un zero*/
      //++(q->readPointer)==MIDA_BUFF? 0: ++(q->readPointer);
      int b=(q->readPointer)+1;
      b==MIDA_BUFF ? (q->readPointer)=0: ((q->readPointer)+=1);
      (q->cont)-=1;
    }
  }
}

uint8_t queue_front(const queue_t *const q){
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    if (queue_is_empty(q)) 
      return 0;
    else
      return q->queue[q->readPointer];
  }
}

