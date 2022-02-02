//
// Created by os on 2/1/22.
//

#ifndef XV6_OS2_SCHEDULING_H
#define XV6_OS2_SCHEDULING_H


struct proc;

extern short scheduleMode;
extern int alpha;
struct proc* get();
void put(struct proc* p);

void contextChange(struct proc* p);
int schedulePeek();
#endif //XV6_OS2_SCHEDULING_H
