#include <stdio.h>
#include "taskchain.h"

TaskResualt FailsSometimes()
{
    static int f=0;
    puts(__func__);

    if (f)
    {
        f = 0;
        return (TaskResualt){.resault = TaskResualt_Fail};
    }
    else
    {
        f = 1;
        return (TaskResualt){.resault = TaskResualt_Ok};
    }
}

TaskResualt NeverFails()
{
    puts(__func__);

    return (TaskResualt){.resault = TaskResualt_Ok};
}

void TaskFailedMethod()
{
    puts(__func__);
}

int main(int argc, char *argv[])
{
    TskChn demotask = {
        .task = NeverFails,
        .Okey = &(TskChn){
            .task = FailsSometimes,
            .Fail = TaskFailedMethod,
            .Okey = &(TskChn){
                .task = FailsSometimes,
                .Fail = TaskFailedMethod}}};

    RunChain(&demotask);
}