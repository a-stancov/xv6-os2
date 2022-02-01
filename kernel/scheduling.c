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
short scheduleMode=0; //prima vrednosti 0 ili 1 , 0 za SJF bez preuzimanja, 1 za CFS
struct spinlock schedL;
int alpha = 50;

struct proc* get(){
    struct proc* res=0,*p;
    acquire(&schedL);
    for(p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if(p->state==RUNNABLE && (res==0||res->tn1>p->tn1||(res->tn1==p->tn1 && res->usedCnt>p->usedCnt))){
            res=p;
        }
        release(&p->lock);
    }
    release(&schedL);
    return res;
}

void put(struct proc* p){
    acquire(&schedL);
    if(p->state==USED){
        p->tn1=0;
        p->usedCnt=0;
    }
    else{
        p->tn=p->tn1;
        p->tn1 = (alpha*(ticks-p->tstart)+(100-alpha)*p->tn)/100;
    }
    p->state=RUNNABLE;
    release(&schedL);
}

void contextChange(struct proc* p){
    put(p);
    sched();
}