#ifndef TASKCHAIN_H
#define TASKCHAIN_H

typedef struct TaskResualt
{
    enum
    {
        TaskResualt_Ok,
        TaskResualt_Fail
    } resault;
} TaskResualt;

typedef struct TaskChain
{
    struct TaskChain *Okey;

    void (*Fail)();        /* the method to run when the work fails */
    TaskResualt (*task)(); /* the work that may fail or be ok */
} TskChn;

static inline void RunChain(TskChn *chain)
{
    /* if there is no chain, bale, or if there is no task to run bail */
    if (!chain || !chain->task)
        return;

    /* actualy do the work the user wants */
    TaskResualt res = chain->task(); 

    if (res.resault == TaskResualt_Fail)
    {
        chain->Fail();
        return;
    }

    /* This was the last task in the chain */
    if (!chain->Okey) 
        return;

    /* we have more things todo, lets go run them */
    RunChain(chain->Okey);
}

#endif