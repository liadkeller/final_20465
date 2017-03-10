#include "lim.h"

struct opcode {
        char *name;
        int group;
} op[] = {{"mov", 2},
          {"cmp", 2},
          {"add", 2},
          {"sub", 2},
          {"not", 1},
          {"clr", 1},
          {"lea", 2},
          {"inc", 1},
          {"dec", 1},
          {"jmp", 1},
          {"bne", 1},
          {"red", 1},
          {"prn", 1},
          {"jsr", 1},
          {"rts", 0},
          {"sto", 0}};
          
int getOpcode(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, op[i].name, op_name_size) == 0)
                        return i;
        // error
        return -1;
}

int getGroup(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, op[i].name, op_name_size3) == 0)
                        return op[i].group;
        // error
        return -1;
}          
