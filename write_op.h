#include <stdint.h>

struct write_op_s {
    int opType;
    int value;
    int startIndex;
    struct write_op_s* next;

};