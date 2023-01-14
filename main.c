#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process
{
    int id;
    int ri;     // execution time
    int pi;     // priority
    int rt;     // remaining time
    int ex;     // executed if == 1
    int active; // 1 if active
} Process;

typedef struct Core
{
    Process *active_process;
} Core;

void readProcess(Process *process)
{
    int i = 0;
    scanf("%d", &i);
    process->id = i;
    scanf("%d", &i);
    process->pi = i;
    scanf("%d", &i);
    process->ri = i;

    process->rt = process->ri;
    process->ex = 0;
    process->active = 0;
}

void displayProcess(Process process)
{
    printf("Process id: %d execution time: %d priority: %d\n", process.id, process.ri, process.pi);
}

void FCFS(Core **Cores, int number_of_cores, Process *processes, int number_of_processes) // could be Core *Cores
{
    for (int i = 0; i < number_of_cores; i++)
    {
        if ((*Cores)[i].active_process == NULL)
        {
            for (int j = 0; j < number_of_processes; j++)
            {
                if (processes[j].active == 1 || processes[j].ex == 1)
                    continue;
                (*Cores)[i].active_process = &processes[j];
                (*Cores)[i].active_process->active = 1;
                break;
            }
        }
    }
}

void SRTF(Core **Cores, int number_of_cores, Process *processes, int number_of_processes)
{
    int before = 100;
    int index = -1;
    for (int i = 0; i < number_of_cores; i++)
    {
        if (Cores[0][i].active_process != NULL) // if core is processing, deactivate
        {
            Cores[0][i].active_process->active = 0;
            Cores[0][i].active_process = NULL;
        }
    }

    for (int i = 0; i < number_of_cores; i++)
    {
        for (int j = 0; j < number_of_processes; j++)
        {
            if (processes[j].rt < before && processes[j].ex != 1 && processes[j].active != 1)
            {
                before = processes[j].rt; // find process with shortest remaining time;
                index = j;
            }
        }
        if (index != -1)
        {
            Cores[0][i].active_process = &processes[index];
            Cores[0][i].active_process->active = 1;
        }
        index = -1;
        before = 100;
    }
}

void SPZW(Core **Cores, int number_of_cores, Process *processes, int number_of_processes)
{
    int before = 100;
    int index = -1;
    for (int i = 0; i < number_of_cores; i++)
    {
        if (Cores[0][i].active_process != NULL) // if core is processing, deactivate
        {
            Cores[0][i].active_process->active = 0;
            Cores[0][i].active_process = NULL;
        }
    }

    for (int i = 0; i < number_of_cores; i++)
    {
        for (int j = 0; j < number_of_processes; j++)
        {
            if (processes[j].pi < before && processes[j].ex != 1 && processes[j].active != 1)
            {
                before = processes[j].pi; // find process with highest priority(0);
                index = j;
            }
        }
        if (index != -1)
        {
            Cores[0][i].active_process = &processes[index];
            Cores[0][i].active_process->active = 1;
        }
        index = -1;
        before = 100;
    }
}

void makeAction(int time, Core **Cores, int number_of_cores)
{
    printf("Time moment:%d |", time);
    for (int i = 0; i < number_of_cores; i++)
    {
        if ((*Cores)[i].active_process == NULL)
        {
            printf(" core:%d id:%d |", i + 1, -1);
            continue;
        }

        printf(" core:%d id:%d |", i + 1, (*Cores)[i].active_process->id);
        (*Cores)[i].active_process->rt -= 1;
        if ((*Cores)[i].active_process->rt == 0)
        {
            (*Cores)[i].active_process->ex = 1;
            (*Cores)[i].active_process->active = 0;

            (*Cores)[i].active_process = NULL;
        }
    }
    printf("\n");
}

void makeActionSimpleDisplay(int time, Core **Cores, int number_of_cores)
{
    printf("%d ", time);
    for (int i = 0; i < number_of_cores; i++)
    {
        if ((*Cores)[i].active_process == NULL)
        {
            printf("%d ", -1);
            continue;
        }

        printf("%d ", (*Cores)[i].active_process->id);
        (*Cores)[i].active_process->rt -= 1;
        if ((*Cores)[i].active_process->rt == 0)
        {
            (*Cores)[i].active_process->ex = 1;
            (*Cores)[i].active_process->active = 0;

            (*Cores)[i].active_process = NULL;
        }
    }
    printf("\n");
}

int checkIfAllExecuted(Process *processes, int number_of_processes)
{
    int left_to_execute = 0;

    for (int i = 0; i < number_of_processes; i++)
    {
        if (processes[i].ex != 1)
        {
            left_to_execute = 1;
        }
    }
    return left_to_execute;
}

void Simulation(int strategy, int t, Core **pointer_to_cores, int num_of_cores, Process *processes, int number_of_processes, char display)
{
    if (strategy == 0)
        FCFS(pointer_to_cores, num_of_cores, processes, number_of_processes);
    else if (strategy == 2)
        SRTF(pointer_to_cores, num_of_cores, processes, number_of_processes);
    else if (strategy == 4)
        SPZW(pointer_to_cores, num_of_cores, processes, number_of_processes);
    if (display == 'c')
        makeAction(t, pointer_to_cores, num_of_cores);
    else
        makeActionSimpleDisplay(t, pointer_to_cores, num_of_cores);
}

int main(int argc, char *argv[])
{
    int strategy = -1, num_of_cores = 1, preemption_time = 1;
    char display;
    Core *cores;
    Core **pointer_to_cores;
    switch (argc)
    {
    case 1:
        printf("Missing arguments\n");
        return -1;
        break;
    case 2:
        printf("Missing arguments\n");
        return -1;
        break;
    case 3:
        strategy = atoi(argv[1]);
        display = argv[2][0];
        if (strategy != 0 && strategy != 2 && strategy != 4) // allowed identifiers
        {
            printf("Unexpected startegy id %d\n", strategy);
            return -1;
        }
        if (display != 's' && display != 'c')
        {
            printf("Wrong display parameter %d\n", num_of_cores);
            return -1;
        }
        cores = malloc(num_of_cores * sizeof(Core));
        for (int b = 0; b < num_of_cores; b++)
        {
            cores[b].active_process = NULL;
        }
        break;
    case 4:
        strategy = atoi(argv[1]);
        num_of_cores = atoi(argv[2]);
        display = argv[3][0];
        if (strategy != 0 && strategy != 2 && strategy != 4) // allowed identifiers
        {
            printf("Unexpected startegy id %d\n", strategy);
            return -1;
        }
        if (num_of_cores < 1)
        {
            printf("Unexpected number of cores %d\n", num_of_cores);
            return -1;
        }
        if (display != 's' && display != 'c')
        {
            printf("Wrong display parameter %d\n", num_of_cores);
            return -1;
        }
        if (display != 's' && display != 'c')
        {
            printf("Wrong display parameter %d\n", num_of_cores);
            return -1;
        }
        cores = malloc(num_of_cores * sizeof(Core));
        for (int b = 0; b < num_of_cores; b++)
        {
            cores[b].active_process = NULL;
        }
        break;
    case 5:
        strategy = atoi(argv[1]);
        num_of_cores = atoi(argv[2]);
        preemption_time = atoi(argv[3]);
        display = argv[4][0];
        if (strategy != 0 && strategy != 2 && strategy != 4) // allowed identifiers
        {
            printf("Unexpected startegy id %d\n", strategy);
            return -1;
        }
        if (num_of_cores < 1)
        {
            printf("Unexpected number of cores %d\n", num_of_cores);
            return -1;
        }
        if (preemption_time < 1)
        {
            printf("Unexpected preemption_time %d\n", preemption_time);
            return -1;
        }
        cores = malloc(num_of_cores * sizeof(Core));
        for (int b = 0; b < num_of_cores; b++)
        {
            cores[b].active_process = NULL;
        }
        break;
    default:
        printf("Too much arguments\n");
        return -1;
        break;
    }

    int t = 0, i = 0, j = 0, number_of_processes = 0, time = 0;
    char c;
    Process processes[100];
    pointer_to_cores = &cores;

    while (1)
    {
        scanf("%d%c", &i, &c);
        t = i;
        if (c == '\n')
        {
            Simulation(strategy, t, pointer_to_cores, num_of_cores, processes, number_of_processes, display);
            continue;
        }
        if (c == '-') // end reading lines
            break;

        while (c != '\n')
        {
            number_of_processes++;
            readProcess(&processes[number_of_processes - 1]);
            scanf("%c", &c);
        } // readed all of the processes passed in time t
        Simulation(strategy, t, pointer_to_cores, num_of_cores, processes, number_of_processes, display);
    }

    int left;
    left = checkIfAllExecuted(processes, number_of_processes);
    while (left != 0)
    {
        Simulation(strategy, t, pointer_to_cores, num_of_cores, processes, number_of_processes, display);
        t++;
        left = checkIfAllExecuted(processes, number_of_processes);
    }
    if (display == 'c')
        makeAction(t, pointer_to_cores, num_of_cores);
    else
        makeActionSimpleDisplay(t, pointer_to_cores, num_of_cores);

    free(cores);
}