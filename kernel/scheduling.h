//
// Created by os on 2/1/22.
//

#ifndef XV6_OS2_SCHEDULING_H
#define XV6_OS2_SCHEDULING_H


struct proc;

extern short scheduleMode;

struct proc* get();
void put(struct proc* p);

void contextChange(struct proc* p);

#endif //XV6_OS2_SCHEDULING_H