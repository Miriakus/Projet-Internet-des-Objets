#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "capteur.h"

#define LBUF 255
#define FicCPU "/proc/stat"
#define FicMem "/proc/meminfo"
#define FicNet "/proc/net/dev"

Cpu cpuCheck()
{
    FILE *fp;
	Cpu proc;

    if ((fp = fopen(FicCPU, "r")) == NULL) return proc;
    fscanf(fp, "cpu\t%ld %ld %ld %ld", &proc.user, &proc.nice, &proc.system, &proc.idle);
    fclose(fp);

    return proc;
}

Ram ramCheck()
{
    FILE *fp;
    char buf[LBUF];
    Ram memory;

    if ((fp = fopen(FicMem, "r")) == NULL) return memory;
    while (fscanf(fp, "%s", buf) != 0) {
        if (strcmp(buf, "MemTotal:") == 0)
            fscanf(fp, "%ld", &memory.total);
        else if (strcmp(buf, "MemFree:") == 0)
            fscanf(fp, "%ld", &memory.free);
        else if (strcmp(buf, "Buffers:") == 0)
            fscanf(fp, "%ld", &memory.buffers);
        else if (strcmp(buf, "Cached:") == 0) {
            fscanf(fp, "%ld", &memory.cached);
            break;
        }
    }
    fclose(fp);

    //printf("ram %ld %ld %ld %ld \n", memory.total, memory.free, memory.buffers, memory.cached);
    memory.used = memory.total - memory.free - memory.buffers - memory.cached;
    memory.pcentUsed = (double) ((double) (memory.total - memory.free - memory.buffers - memory.cached) / memory.total) * 100;

    return memory;
}

Swap swapCheck()
{
    FILE *fp;
    char buf[LBUF];
    Swap memory;

    if ((fp = fopen(FicMem, "r")) == NULL) return memory;
    while (fscanf(fp, "%s", buf) != 0) {
        if (strcmp(buf, "SwapCached:") == 0)
            fscanf(fp, "%ld", &memory.cached);
        else if (strcmp(buf, "SwapTotal:") == 0)
            fscanf(fp, "%ld", &memory.total);
        else if (strcmp(buf, "SwapFree:") == 0) {
            fscanf(fp, "%ld", &memory.free);
            break;
        }
    }
    fclose(fp);

    //printf("swap %ld %ld %ld \n", memory.total, memory.free, memory.cached);
    memory.used = memory.total - memory.free;
    memory.pcentUsed = (double) ((double) (memory.total - memory.free) / memory.total) * 100;

    return memory;
}

Network networkCheck()
{
    FILE *fp;
    char buf[LBUF];
    Network eth;
	int i;

    if ((fp = fopen(FicNet, "r")) == NULL) return eth;
	fgets(buf, LBUF-1, fp);
	fgets(buf, LBUF-1, fp);
    fscanf(fp, "%s %ld", buf, &eth.totalDown);
	for (i=0; i<7; i++)
		fscanf(fp, "%s", buf);
    fscanf(fp, "%ld", &eth.totalUp);
    fclose(fp);

    //printf("net %ld %ld %ld %ld \n", eth.totalDown, eth.totalUp, eth.debitDown, eth.debitUp);

    return eth;
}

void capteurCheck(Capteur *capteur)
{
    capteur->cpu = cpuCheck();
    capteur->ram = ramCheck();
    capteur->swap = swapCheck();
    capteur->net = networkCheck();
    capteur->time = time(NULL);
}

void calcCpuPcent(Cpu *cpu, Cpu *cpuOld)
{
    long diffUser, diffNice, diffSystem, diffIdle;

    diffUser = cpu->user - cpuOld->user;
    diffNice = cpu->nice - cpuOld->nice;
    diffSystem = cpu->system - cpuOld->system;
    diffIdle = cpu->idle - cpuOld->idle;

    cpu->pcentUsed = (double) ((diffUser + diffNice + diffSystem) * 100L)
        / (double) (diffUser + diffNice + diffSystem + diffIdle);
}

