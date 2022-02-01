//
// Created by os on 2/1/22.
//
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "scheduling.h"

extern struct proc proc[NPROC];
short scheduleMode=0; //prima vrednosti 0,1 ili 2
struct spinlock schedL;

struct proc* get(){
    struct proc* res=0,*p;
    acquire(&schedL);
    for(p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if(p->state==RUNNABLE && (res==0)){
            res=p;
            release(&p->lock);
            break;
        }
        release(&p->lock);
    }
    release(&schedL);
    return res;
}

void put(struct proc* p){
    acquire(&schedL);
    p->state=RUNNABLE;
    release(&schedL);
}

void contextChange(struct proc* p){
    put(p);
    sched();
}