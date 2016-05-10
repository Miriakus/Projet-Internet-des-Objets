#ifndef __CAPTEUR__
#define __CAPTEUR__

typedef struct Cpu Cpu;
struct Cpu {
    long user;              // tick
    long nice;              // tick
    long system;            // tick
    long idle;              // tick
    double pcentUsed;       // %
};

typedef struct Ram Ram;
struct Ram {
    long total;             // ko
    long free;              // ko
    long buffers;           // ko
    long cached;            // ko
    long used;              // ko
    double pcentUsed;       // %
};

typedef struct Swap Swap;
struct Swap {
    long total;             // ko
    long free;              // ko
    long cached;            // ko
    long used;              // ko
    double pcentUsed;       // %
};

typedef struct Disk Disk;
struct Disk {
    long totalRead;         // ko
    long totalWrite;        // ko
    long totalTimeActive;   // ms
    long debitRead;         // ko/s
    long debitWrite;        // ko/s
    double pcentActive;     // %
};

typedef struct Network Network;
struct Network {
    long totalDown;         // o
    long totalUp;           // o
    long debitDown;         // o/s
    long debitUp;           // o/s
};

typedef struct Time Time;
struct Time {
    long sec;
    long microsec;
};

typedef struct Capteur Capteur;
struct Capteur {
    Cpu cpu;
    Ram ram;
    Swap swap;
    Disk disk;
    Network network;
    Time time;
};

Cpu cpuCheck();
Ram ramCheck();
Swap swapCheck();
Disk diskCheck();
Network networkCheck();

void capteurCheck(Capteur*);
void calcCpuPcent(Cpu*, Cpu*);
void calcDiskDebit(Disk*, Disk*, unsigned int);
void calcNetworkDebit(Network*, Network*, unsigned int);
long calcDebit(long, long, unsigned int);
void printJSON(char*, Capteur*);

# endif /* __CAPTEUR__ */
