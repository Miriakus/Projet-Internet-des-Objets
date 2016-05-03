#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FicCpu "/proc/stat"
#define FicCpuInfo "/proc/cpuinfo"
#define LBUF 500


double cpuPcent(void)
{
  FILE * fp;
  char nom [10];
  char cpuModel [60];
  char nom2 [60];
  char buffer [LBUF];
  char * bufferAfterTwoPoints;
  //v1=temps passé en utilisateur.v2=temps passé en priorité basse.v3=temps passé en système
  //v4=temps passé à ne rien faire.
  long v1, v2, v3, v4;
  //vp=deuxième mesure
  long vp1, vp2, vp3, vp4;
  //d=soustraction des deux mesures vp-v
  long d1, d2, d3, d4;
  double res=0.;

  if ((fp = fopen(FicCpu, "r")) == NULL) return (double)-1;
  fscanf(fp,"%s %ld %ld %ld %ld", nom, &v1, &v2, &v3, &v4);
  printf("%s %ld %ld %ld %ld\n",nom,v1,v2,v3,v4);

  sleep(1);
  rewind(fp);

  if ((fp = fopen(FicCpu, "r")) == NULL) return (double)-1;
  fscanf(fp,"%s %ld %ld %ld %ld", nom, &vp1, &vp2, &vp3, &vp4);
  printf("%s %ld %ld %ld %ld\n",nom,vp1,vp2,vp3,vp4);

  d1 = vp1 - v1;
  d2 = vp2 - v2;
  d3 = vp3 - v3;
  d4 = vp4 - v4;

  fclose(fp);

  if((fp = fopen(FicCpuInfo, "r")) == NULL) return (double)-1;
  while(fgets(buffer, LBUF, fp) != NULL){
    if(strcmp(nom2, "model")){
      bufferAfterTwoPoints = strchr(buffer, ':') +1;
      if(bufferAfterTwoPoints==NULL)continue;
      printf("%s : %s\n", nom2, bufferAfterTwoPoints);
    }
  }

  res = (double)((d1+d2+d3) * 100L) / (double)(d1+d2+d3+d4);
  printf("Votre CPU: \n");
  return res;
}

int main(int N, char * P[])
{
  double v;
  v = cpuPcent();
  printf("Le Pourcentage d'utilisation du CPU est de %g\n", v);
  return 0;
}


