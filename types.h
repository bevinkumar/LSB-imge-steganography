#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;
typedef unsigned U;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
