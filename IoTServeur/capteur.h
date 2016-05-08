#ifndef __CAPTEUR__
#define __CAPTEUR__

typedef struct Cpu Cpu;
struct Cpu {
    long user;
    long nice;
    long system;
    long idle;
    double pcentUsed;
};

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

typedef struct Network Network;
struct Network {
    long totalDown;
    long totalUp;
    long debitDown;
    long debitUp;
};

typedef struct Capteur Capteur;
struct Capteur {
    Cpu cpu;
    Ram ram;
    Swap swap;
    Network net;
    long time;
};

Cpu cpuCheck();
Ram ramCheck();
Swap swapCheck();
Network networkCheck();
void capteurCheck(Capteur*);
void calcCpuPcent(Cpu*, Cpu*);

# endif /* __CAPTEUR__ */
