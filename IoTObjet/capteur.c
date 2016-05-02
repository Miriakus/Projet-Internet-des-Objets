#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LBUF 255
#define FicCPU "/proc/stat"
#define FicMem "/proc/meminfo"

typedef struct Ram Ram;
struct Ram {
	long total;
	long free;
	long buffers;
	long cached;
	long used;
	double pcentUsed;
};

typedef struct Swap Swap;
struct Swap {
	long total;
	long free;
	long cached;
	long used;
	double pcentUsed;
};

/* fonction qui accede a l'interface et qui calcule le pourcentage */
double cpuPcent(void)
{
FILE * fp;
char nom [10];
long v1, v2, v3, v4;
long vp1, vp2, vp3, vp4;
long d1, d2, d3, d4;
double res=0.;
   if ((fp = fopen(FicCPU, "r")) == NULL) return (double)-1;
   fscanf(fp,"%s %ld %ld %ld %ld", nom, &v1, &v2, &v3, &v4);
   //printf("%s %ld %ld %ld %ld\n",nom,v1,v2,v3,v4);
   /* on attend 1 sec */
   sleep(1);
   rewind(fp);
   if ((fp = fopen(FicCPU, "r")) == NULL) return (double)-1;
   fscanf(fp,"%s %ld %ld %ld %ld", nom, &vp1, &vp2, &vp3, &vp4);
   //printf("%s %ld %ld %ld %ld\n",nom,vp1,vp2,vp3,vp4);
   d1 = vp1 - v1;
   d2 = vp2 - v2;
   d3 = vp3 - v3;
   d4 = vp4 - v4;
   res = (double)((d1+d2+d3) * 100L) / (double)(d1+d2+d3+d4);
   return res;
}

Ram ramCheck(void)
{
FILE * fp;
char buf [LBUF];
Ram memory;

   	if ((fp = fopen(FicMem, "r")) == NULL) return;
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
   	
	printf("ram %ld %ld %ld %ld \n", memory.total, memory.free, memory.buffers, memory.cached);
	memory.used = memory.total - memory.free - memory.buffers - memory.cached;
	memory.pcentUsed = (double)((double)(memory.total - memory.free - memory.buffers - memory.cached) / memory.total) * 100;

   	return memory;
}

Swap swapCheck(void)
{
FILE * fp;
char buf [LBUF];
Swap memory;

   	if ((fp = fopen(FicMem, "r")) == NULL) return;
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

	printf("swap %ld %ld %ld \n", memory.total, memory.free, memory.cached);
	memory.used = memory.total - memory.free;
	memory.pcentUsed = (double)((double)(memory.total - memory.free) / memory.total) * 100;

	return memory;
}

int main(int N, char * P[])
{
double v;
Ram ram;
Swap swap;
    v = cpuPcent();
	ram = ramCheck();
	swap = swapCheck();
    printf("Le Pcentage CPU instantanne est %g %\n", v);
    printf("L'utilisation de la RAM est de %g % (%ld/%ld Mo)\n", ram.pcentUsed, ram.used/1024, ram.total/1024);
    printf("L'utilisation du Swap est de %g % (%ld/%ld Mo)\n", swap.pcentUsed, swap.used/1024, swap.total/1024);
    return 0;
}


