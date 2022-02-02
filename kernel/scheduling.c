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

struct proc* procesi[NPROC];
short scheduleMode=2; //prima vrednosti 0 ili 1 ili 2 , 0 za SJF bez preuzimanja, 1 za SJF sa preuzimanjem , 2 za CFS
struct spinlock schedL;
int alpha = 50;
int brProcesa=0;

struct proc* getSJF(){
    struct proc* res=0;
    if(brProcesa>0){
        res=procesi[--brProcesa];
    }
    return res;
}

struct proc* getCFS(){
    struct proc* res=0;
    if(brProcesa==0) return res;
    res=procesi[--brProcesa];
    res->timeSlice=(brProcesa==0)?2:(ticks-res->lastdur-res->tstart); //2 ako nema sta drugo
    if(res->timeSlice<1)res->timeSlice=1; //ako mu je dodeljen neki bangav timeslice

    res->lastdur+=res->timeSlice;

    return res;
}

struct proc* get(){
    struct proc* res=0;
    acquire(&schedL);
    if(scheduleMode==2)
        res=getCFS();
    else
        res=getSJF();
    release(&schedL);
    return res;
}


void putSJF(struct proc* p){
    if(p->state==USED){ //ovde radim inicijalizaciju procesa
        p->tn1=0;
        p->usedCnt=0;
        p->lastdur=0;
        p->taken=0;
    }
    else{
        if(!p->taken){
            p->tn=p->tn1;
            p->tn1 = (alpha*(p->lastdur+ticks-p->tstart)+(100-alpha)*p->tn)/100;
            p->lastdur=0;
        }else{
            p->lastdur+=ticks-p->tstart;
            p->taken=0;
        }

    }
    p->state=RUNNABLE;

    int curr;
    if(brProcesa==0){
        procesi[0]=p;
    }else{
        for(curr=brProcesa;curr>0;curr--){
            if(procesi[curr-1]->tn1-procesi[curr-1]->lastdur>p->tn1-p->lastdur)
                break;

            procesi[curr]=procesi[curr-1];
        }
        procesi[curr]=p;
    }

    brProcesa++;
}
void putCFS(struct proc* p){
    if(p->state==USED||p->state==SLEEPING){ //ovde radim inicijalizaciju procesa
        p->tstart=ticks;
        p->lastdur=0;
    }
    p->state=RUNNABLE;

    int curr;
    if(brProcesa==0){
        procesi[0]=p;
    }else{
        for(curr=brProcesa;curr>0;curr--){
            if(procesi[curr-1]->lastdur>p->lastdur)
                break;

            procesi[curr]=procesi[curr-1];
        }
        procesi[curr]=p;
    }

    brProcesa++;
}

void put(struct proc* p){
    acquire(&schedL);
    if(scheduleMode==2)
        putCFS(p);
    else
        putSJF(p);
    release(&schedL);
}


int schedulePeek(){
    return procesi[brProcesa-1]->tn1-procesi[brProcesa-1]->lastdur;
}


void contextChange(struct proc* p){ //koristi se u trap.c, kada dodje prekid od timera
    acquire(&schedL);
    if(scheduleMode==1){ //SJF sa preotimanjem
        if(brProcesa!=0 && schedulePeek()<p->tn1-p->lastdur-(ticks-p->tstart)){
            p->taken=1;
            release(&schedL);
            yield();
            return;
        }
    }
    else if(scheduleMode==2){ //CFS
        if(--p->timeSlice==0){
            release(&schedL);
            yield();
            return;
        }
    }
    release(&schedL);
}