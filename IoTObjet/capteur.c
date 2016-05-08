/*
 * capteur.c : capteurs de l'objet connecté
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "capteur.h"

#define LBUF 255
#define FicCPU "/proc/stat"
#define FicMem "/proc/meminfo"
#define FicDisk "/sys/block/sda/stat"
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

Disk diskCheck()
{
    FILE *fp;
    char buf[LBUF];
    Disk disk;

    if ((fp = fopen(FicDisk, "r")) == NULL) return disk;
    fscanf(fp, "%s %s %ld %s %s %s %ld %s %s %ld",
        buf, buf, &disk.totalRead, buf, buf, buf, &disk.totalWrite, buf, buf, &disk.totalTimeActive);
    fclose(fp);

    //printf("disk %ld %ld %ld\n", disk.totalRead, disk.totalWrite, disk.totalTimeActive);
    disk.totalRead /= 2;
    disk.totalWrite /= 2;

    return disk;
}

Network networkCheck()
{
    FILE *fp;
    char buf[LBUF];
    Network eth;
	int i;

    if ((fp = fopen(FicNet, "r")) == NULL) return eth;
	fgets(buf, LBUF, fp);
	fgets(buf, LBUF, fp);
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
    capteur->disk = diskCheck();
    capteur->network = networkCheck();
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

void calcDiskDebit(Disk *disk, Disk *diskOld, unsigned int frequence)
{
    disk->debitRead = calcDebit(disk->totalRead, diskOld->totalRead, frequence);
    disk->debitWrite = calcDebit(disk->totalWrite, diskOld->totalWrite, frequence);
    disk->pcentActive = (double) ((disk->totalTimeActive - diskOld->totalTimeActive) * 100) / (double) frequence;
}

void calcNetworkDebit(Network *network, Network *networkOld, unsigned int frequence)
{
    network->debitDown = calcDebit(network->totalDown, networkOld->totalDown, frequence);
    network->debitUp = calcDebit(network->totalUp, networkOld->totalUp, frequence);
}

long calcDebit(long total, long totalOld, unsigned int frequence)
{
    return (long) ((double) (total - totalOld) / (double) ((double) frequence / 1000));
}
