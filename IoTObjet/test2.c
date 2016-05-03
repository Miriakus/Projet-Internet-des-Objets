/* exemple simple de recuperation des informations du CPU en allant
   consulter l'interface /proc/stat
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LBUF 500

#define FicCpu "/proc/stat"
#define FicCpuInfo "/proc/cpuinfo"
#define FicDisk "/proc/diskstats"
#define FicMem "/proc/meminfo"

/* fonction qui accede a l'interface et qui calcule le pourcentage */
void cpuPcent(void)
{
  FILE * fp;
  char nom [10];
  long v1, v2, v3, v4;
  long vp1, vp2, vp3, vp4;
  long d1, d2, d3, d4;
  double res=0.;

  if ((fp = fopen(FicCpu, "r")) == NULL) return;
  fscanf(fp,"%s %ld %ld %ld %ld", nom, &v1, &v2, &v3, &v4);
  /* on attend 1 sec */
  sleep(1);
  rewind(fp);
  if ((fp = fopen(FicCpu, "r")) == NULL) return;
  fscanf(fp,"%s %ld %ld %ld %ld", nom, &vp1, &vp2, &vp3, &vp4);
  d1 = vp1 - v1;
  d2 = vp2 - v2;
  d3 = vp3 - v3;
  d4 = vp4 - v4;
  res = (double)((d1+d2+d3) * 100L) / (double)(d1+d2+d3+d4);
  printf("Utilisation du processeur : %g%%\n", res);
  fclose(fp);
}


void modelCpu(void)
{
  FILE * fp;
  char nom [100];
  char nom2[200];
  char reste[200];
  char buffer[LBUF], *b;
  int n, s1,s2;

  if ((fp = fopen(FicCpuInfo, "r")) == NULL) return;
  while (fgets(buffer, LBUF, fp) != NULL) {
    n=strlen(buffer);
    buffer[n-1]='\0';
    s1 = sscanf(buffer,"%s %s :", nom, nom2);
    b = strchr(buffer,':');
    if (b==NULL) continue;
    s2 = sscanf(b+1," %s", reste);
    if (strcmp(nom, "model")==0){
      if (strcmp(nom2, "name")==0){
        printf("Votre processeur : %s\n", b+1);
        break;
      }
    }
  }
  fclose(fp);
}
/* Tests pour la récupération des données disques
void diskStatus(void){
  FILE * fp;
  char buffer[LBUF], *b, name[20];
  int majorNum, minorNum, reads, readsMerged, sectReads, timeRead, writes, writesMerged, sectWrite, timeWrites, iosCurrent, timeIos, weighterTimeIos;
  int majorNum2, minorNum2, reads2, readsMerged2, sectReads2, timeRead2, writes2, writesMerged2, sectWrite2, timeWrites2, iosCurrent2, timeIos2, weighterTimeIos2;

  if((fp = fopen(FicDisk, "r")) == NULL) return;
  while (fgets(buffer, LBUF, fp)!=NULL){
    fscanf(fp, "%d %d %s %d %d %d %d %d %d %d %d %d %d %d", & majorNum, & minorNum, name, & reads, & readsMerged, & sectReads, & timeRead, & writes, & writesMerged, & sectWrite, & timeWrites, & iosCurrent, & timeIos, & weighterTimeIos);
    if(strcmp(name, "sda")==0){
      sleep(1);
      rewind(fp);
      if((fp = fopen(FicDisk, "r")) == NULL) return;
      while (fgets(buffer, LBUF, fp)!=NULL){
        fscanf(fp, "%d %d %s %d %d %d %d %d %d %d %d %d %d %d", & majorNum2, & minorNum2, name, & reads2, & readsMerged2, & sectReads2, & timeRead2, & writes2, & writesMerged2, & sectWrite2, & timeWrites2, & iosCurrent2, & timeIos2, & weighterTimeIos2);
        if(strcmp(name, "sda")==0){
          printf("Le disque dur \"%s\" utilisation en écriture : %d%%.\n", name, timeWrites);
        }
      }
    }
  }
}
*/

void ramPcent(void){
  FILE * fp;
  char buffer[LBUF], name[100];
  int n;
  int matchMemTotal, matchMemFree, matchMemBuffer, matchMemCache;
  matchMemFree = 0;
  matchMemTotal = 0;
  long value, ramPcent, memTotal, memFree, memBuffers, memCache;
  while(1){
    if((fp = fopen(FicMem, "r"))==NULL) return;
    while(fgets(buffer, LBUF, fp)!=NULL){
      n = strlen(buffer);
      buffer[n-3]='\0';
      sscanf(buffer, "%s %ld", name, & value);
      if(strcmp(name, "MemTotal:")==0){
        memTotal = value;
        matchMemTotal = 1;
      }
      if(strcmp(name, "MemFree:")==0){
        memFree = value;
        matchMemFree = 1;
      }
      if(strcmp(name, "Buffers:")==0){
        memBuffers = value;
        matchMemBuffer = 1;
      }
      if(strcmp(name, "Cached:")==0){
        memCache = value;
        matchMemCache = 1;
      }
      if(matchMemFree==1 && matchMemTotal==1 && matchMemCache==1 && matchMemBuffer==1){
        ramPcent = (memFree-memBuffers-memFree)*100L / memTotal;
        printf("Mémoire vive utilisée à %ld%%\n", ramPcent);
        break;
      }
    }
    sleep(1);
    rewind(fp);
    fclose(fp);
  }
}

int main(int N, char * P[])
{
  modelCpu();
  cpuPcent();
  ramPcent();
  return 0;
}


