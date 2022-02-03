//
// Created by os on 2/3/22.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
    if(argc < 3){
        fprintf(2, "usage: changeAlgo num num ...\n");
        exit(1);
    }
    changeSchAlgo(atoi(argv[1]),atoi(argv[2]));
    exit(0);
}
