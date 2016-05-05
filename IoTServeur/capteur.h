#ifndef __CAPTEUR__
#define __CAPTEUR__

typedef struct Cpu Cpu;
struct Cpu {
    long user;
    long nice;
    long system;
    long idle;
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
};

Cpu cpuCheck(void);
Ram ramCheck(void);
Swap swapCheck(void);
Network networkCheck(void);

# endif /* __CAPTEUR__ */
