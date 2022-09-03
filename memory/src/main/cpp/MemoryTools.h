#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <cstring>

struct MAPS {
    long addr;
    long taddr;
    struct MAPS *next;
};

struct RESULT {
    long int addr;
    struct RESULT *next;
};

struct FREEZE {
    long int addr;
    char *value;
    int type;
    struct FREEZE *next;
};

#define LEN sizeof(struct MAPS)
#define FRE sizeof(struct FREEZE)
typedef struct MAPS *PMAPS;
typedef struct RESULT *PRES;
typedef struct FREEZE *PFREEZE;

typedef int TYPE;
typedef int RANGE;
typedef int COUNT;
typedef int COLOR;
typedef long int OFFSET;
typedef long int ADDRESS;
typedef char PACKAGENAME;

enum type {
    DWORD,
    FLOAT,
    DOUBLE,
    WORD,
    BYTE,
    QWORD,
};

enum Range {
    ALL,
    B_BAD,
    C_ALLOC,
    C_BSS,
    CODE_APP,
    C_DATA,
    C_HEAP,
    JAVA_HEAP,
    A_ANONMYOUS,
    CODE_SYSTEM,
    STACK,
    ASHMEM
};

enum Color {
    COLOR_SILVERY,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_DARK_BLUE,
    COLOR_PINK,
    COLOR_SKY_BLUE,
    COLOR_WHITE
};

PMAPS Res = NULL;

PFREEZE Pfreeze = NULL;
PFREEZE pEnd = NULL;
PFREEZE pNew = NULL;
int FreezeCount = 0;
int Freeze = 0;
pthread_t pth;
char Fbm[64];
long int delay = 30000;

int ResCount = 0;
int MemorySearchRange = 0;

int SetTextColor(int color);

int getPID(char bm[64]);

int SetSearchRange(int type);

PMAPS readmaps(int pid, int type);

PMAPS readmaps_all(int pid);

PMAPS readmaps_bad(int pid);

PMAPS readmaps_c_alloc(int pid);

PMAPS readmaps_c_bss(int pid);

PMAPS readmaps_code_app(int pid);

PMAPS readmaps_c_data(int pid);

PMAPS readmaps_c_heap(int pid);

PMAPS readmaps_java_heap(int pid);

PMAPS readmaps_a_anonmyous(int pid);

PMAPS readmaps_code_system(int pid);

PMAPS readmaps_stack(int pid);

PMAPS readmaps_ashmem(int pid);

ADDRESS
GetModuleBase(int pid, char *name, int index);

void RangeMemorySearch(int pid, char *from_value, char *to_value, int *gs, int type);

PMAPS RangeMemorySearch_DWORD(int pid, int from_value, int to_value, int *gs, PMAPS pMap);

PMAPS RangeMemorySearch_FLOAT(int pid, float from_value, float to_value, int *gs, PMAPS pMap);

PMAPS RangeMemorySearch_DOUBLE(int pid, double from_value, double to_value, int *gs, PMAPS pMap);

PMAPS RangeMemorySearch_WORD(int pid, short from_value, short to_value, int *gs, PMAPS pMap);

PMAPS RangeMemorySearch_BYTE(int pid, char from_value, char to_value, int *gs, PMAPS pMap);

PMAPS
RangeMemorySearch_QWORD(int pid, long long from_value, long long to_value, int *gs, PMAPS pMap);

void MemorySearch(int pid, char *value, int *gs, int TYPE);

PMAPS MemorySearch_DWORD(int pid, int value, int *gs, PMAPS pMap);

PMAPS MemorySearch_FLOAT(int pid, float value, int *gs, PMAPS pMap);

PMAPS MemorySearch_DOUBLE(int pid, double value, int *gs, PMAPS pMap);

PMAPS MemorySearch_WORD(int pid, short int value, int *gs, PMAPS pMap);

PMAPS MemorySearch_BYTE(int pid, char value, int *gs, PMAPS pMap);

PMAPS MemorySearch_QWORD(int pid, long long value, int *gs, PMAPS pMap);

void MemoryOffset(int pid, char *value, long int offset, int *gs, int type);

PMAPS MemoryOffset_DWORD(int pid, int value, long int offset, PMAPS pBuff, int *gs);

PMAPS MemoryOffset_FLOAT(int pid, float value, long int offset, PMAPS pBuff, int *gs);

PMAPS MemoryOffset_DOUBLE(int pid, double value, long int offset, PMAPS pBuff, int *gs);

PMAPS MemoryOffset_WORD(int pid, short int value, long int offset, PMAPS pBuff, int *gs);

PMAPS MemoryOffset_BYTE(int pid, char value, long int offset, PMAPS pBuff, int *gs);

PMAPS MemoryOffset_QWORD(int pid, long long value, long int offset, PMAPS pBuff, int *gs);

void RangeMemoryOffset(int pid, char *from_value, char *to_value, long int offset, int *gs,
                       int type);

PMAPS RangeMemoryOffset_DWORD(int pid, int from_value, int to_value, long int offset, PMAPS pBuff,
                              int *gs);

PMAPS
RangeMemoryOffset_FLOAT(int pid, float from_value, float to_value, long int offset, PMAPS pBuff,
                        int *gs);

PMAPS RangeMemoryOffset_DOUBLE(int pid, double from_value, double to_value, long int offset,
                               PMAPS pBuff, int *gs);

PMAPS RangeMemoryOffset_WORD(int pid, short int from_value, short int to_value, long int offset,
                             PMAPS pBuff, int *gs);

PMAPS RangeMemoryOffset_BYTE(int pid, char from_value, char to_value, long int offset, PMAPS pBuff,
                             int *gs);

PMAPS RangeMemoryOffset_QWORD(int pid, long long from_value, long long to_value, long int offset,
                              PMAPS pBuff, int *gs);

char *MemoryRead(int pid, long address, int type);

int MemoryRead_DWORD(int pid, long address);

float MemoryRead_FLOAT(int pid, long address);

double MemoryRead_DOUBLE(int pid, long address);

short int MemoryRead_WORD(int pid, long address);

char MemoryRead_BYTE(int pid, long address);

long long MemoryRead_QWORD(int pid, long address);

void MemoryWrite(int pid, char *value, long address, int type);

int MemoryWrite_DWORD(int pid, int value, long address);

int MemoryWrite_FLOAT(int pid, float value, long address);

int MemoryWrite_DOUBLE(int pid, double value, long address);

int MemoryWrite_WORD(int pid, short int value, long address);

int MemoryWrite_BYTE(int pid, char value, long address);

int MemoryWrite_QWORD(int pid, long long value, long address);

void ClearResults();

int killProcessByOs(char *bm);

char GetProcessState(char *bm);

int killXs();

void AddFreezeItem(int pid, long int addr, char *value, int type);

void AddFreezeItem_DWORD(int pid, long int addr, char *value);

void AddFreezeItem_FLOAT(int pid, long int addr, char *value);

void AddFreezeItem_DOUBLE(int pid, long int addr, char *value);

void AddFreezeItem_WORD(int pid, long int addr, char *value);

void AddFreezeItem_BYTE(int pid, long int addr, char *value);

void AddFreezeItem_QWORD(int pid, long int addr, char *value);

int RemoveFreezeItem(long int addr);

int RemoveFreezeItem_All();

int StartFreeze(int pid);

int StopFreeze();

int SetFreezeDelay(long int De);

char *intToChar(int i);

char *longToChar(long i);

char *floatToChar(float i);

char *doubleToChar(double i);

char *longlongToChar(long long i);

char *intToChar(int i) {
    char *str = (char *) malloc(10);
    sprintf(str, "%d", i);
    return str;
}

char *longToChar(long i) {
    char *str = (char *) malloc(20);
    sprintf(str, "%lx", i);
    return str;
}

char *floatToChar(float i) {
    char *str = (char *) malloc(20);
    sprintf(str, "%f", i);
    return str;
}

char *doubleToChar(double i) {
    char *str = (char *) malloc(20);
    sprintf(str, "%lf", i);
    return str;
}

char *longlongToChar(long long i) {
    char *str = (char *) malloc(20);
    sprintf(str, "%lld", i);
    return str;
}

int SetTextColor(COLOR color) {
    switch (color) {
        case COLOR_SILVERY:
            printf("\033[30;1m");
            break;
        case COLOR_RED:
            printf("\033[31;1m");
            break;
        case COLOR_GREEN:
            printf("\033[32;1m");
            break;
        case COLOR_YELLOW:
            printf("\033[33;1m");
            break;
        case COLOR_DARK_BLUE:
            printf("\033[34;1m");
            break;
        case COLOR_PINK:
            printf("\033[35;1m");
            break;
        case COLOR_SKY_BLUE:
            printf("\033[36;1m");
            break;
        case COLOR_WHITE:
            printf("\033[37;1m");
            break;
        default:
            printf("\033[37;1m");
            break;
    }
    return 0;
}

int getPID(PACKAGENAME *PackageName) {
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(PackageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

ADDRESS GetModuleBase(int pid, char *name, int index) {
    int i = 0;
    long start = 0, end = 0;
    char line[1024] = {0};
    char fname[128];
    sprintf(fname, "/proc/%d/maps", pid);
    FILE *p = fopen(fname, "r");
    if (p) {
        while (fgets(line, sizeof(line), p)) {
            if (strstr(line, name) != NULL) {
                i++;
                if (i == index) {
                    sscanf(line, "%lx-%lx", &start, &end);
                    break;
                }
            }
        }
        fclose(p);
    }
    return start;
}

int SetSearchRange(TYPE type) {
    switch (type) {
        case ALL:
            MemorySearchRange = 0;
            break;
        case B_BAD:
            MemorySearchRange = 1;
            break;
        case C_ALLOC:
            MemorySearchRange = 2;
            break;
        case C_BSS:
            MemorySearchRange = 3;
            break;
        case CODE_APP:
            MemorySearchRange = 4;
            break;
        case C_DATA:
            MemorySearchRange = 5;
            break;
        case C_HEAP:
            MemorySearchRange = 6;
            break;
        case JAVA_HEAP:
            MemorySearchRange = 7;
            break;
        case A_ANONMYOUS:
            MemorySearchRange = 8;
            break;
        case CODE_SYSTEM:
            MemorySearchRange = 9;
            break;
        case STACK:
            MemorySearchRange = 10;
            break;
        case ASHMEM:
            MemorySearchRange = 11;
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    return 0;
}

PMAPS readmaps(int pid, TYPE type) {
    PMAPS pMap = NULL;
    switch (type) {
        case ALL:
            pMap = readmaps_all(pid);
            break;
        case B_BAD:
            pMap = readmaps_bad(pid);
            break;
        case C_ALLOC:
            pMap = readmaps_c_alloc(pid);
            break;
        case C_BSS:
            pMap = readmaps_c_bss(pid);
            break;
        case CODE_APP:
            pMap = readmaps_code_app(pid);
            break;
        case C_DATA:
            pMap = readmaps_c_data(pid);
            break;
        case C_HEAP:
            pMap = readmaps_c_heap(pid);
            break;
        case JAVA_HEAP:
            pMap = readmaps_java_heap(pid);
            break;
        case A_ANONMYOUS:
            pMap = readmaps_a_anonmyous(pid);
            break;
        case CODE_SYSTEM:
            pMap = readmaps_code_system(pid);
            break;
        case STACK:
            pMap = readmaps_stack(pid);
            break;
        case ASHMEM:
            pMap = readmaps_ashmem(pid);
            break;

        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pMap == NULL) {
        return 0;
    }
    return pMap;
}

PMAPS readmaps_all(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew;
    PMAPS pEnd;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp)) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_bad(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "kgsl-3d0")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_c_alloc(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[anon:libc_malloc]")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_c_bss(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[anon:.bss]")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_code_app(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (!feof(fp) && strstr(buff, "/data/")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_c_data(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/data/app/")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_c_heap(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[heap]")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_java_heap(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/dev/ashmem/")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_a_anonmyous(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp)) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_code_system(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/system")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_stack(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[stack]")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

PMAPS readmaps_ashmem(int pid) {
    PMAPS pHead = NULL;
    PMAPS pNew = NULL;
    PMAPS pEnd = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        puts("分析失败");
        return NULL;
    }
    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/dev/ashmem/") &&
            !strstr(buff, "dalvik")) {
            sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);

            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 1) {
            i++;
            if (i == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
        }
    }
    free(pNew);
    fclose(fp);
    return pHead;
}

void ClearResults() {
    PMAPS pHead = Res;
    PMAPS pTemp = pHead;
    for (int i = 0; i < ResCount; i++) {
        pTemp = pHead;
        pHead = pHead->next;
        free(pTemp);
    }
    ResCount = 0;
}

void RangeMemorySearch(int pid, char *from_value, char *to_value, COUNT *gs, TYPE type) {
    PMAPS pHead = NULL;
    PMAPS pMap = NULL;
    switch (MemorySearchRange) {
        case ALL:
            pMap = readmaps(pid, ALL);
            break;
        case B_BAD:
            pMap = readmaps(pid, B_BAD);
            break;
        case C_ALLOC:
            pMap = readmaps(pid, C_ALLOC);
            break;
        case C_BSS:
            pMap = readmaps(pid, C_BSS);
            break;
        case C_DATA:
            pMap = readmaps(pid, C_DATA);
            break;
        case CODE_APP:
            pMap = readmaps_code_app(pid);
            break;
        case C_HEAP:
            pMap = readmaps(pid, C_HEAP);
            break;
        case JAVA_HEAP:
            pMap = readmaps(pid, JAVA_HEAP);
            break;
        case A_ANONMYOUS:
            pMap = readmaps(pid, A_ANONMYOUS);
            break;
        case CODE_SYSTEM:
            pMap = readmaps(pid, CODE_SYSTEM);
            break;
        case STACK:
            pMap = readmaps(pid, STACK);
            break;
        case ASHMEM:
            pMap = readmaps(pid, ASHMEM);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pMap == NULL) {
        puts("map error");
        return (void) 0;
    }
    switch (type) {
        case DWORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemorySearch_DWORD(pid, atoi(to_value), atoi(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_DWORD(pid, atoi(from_value), atoi(to_value), gs, pMap);
            break;
        case FLOAT:
            if (atof(from_value) > atof(to_value))
                pHead = RangeMemorySearch_FLOAT(pid, atof(to_value), atof(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_FLOAT(pid, atof(from_value), atof(to_value), gs, pMap);
            break;
        case DOUBLE:
            if (atof(from_value) > atof(to_value))
                pHead = RangeMemorySearch_DOUBLE(pid, atof(to_value), atof(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_DOUBLE(pid, atof(from_value), atof(to_value), gs, pMap);
            break;
        case WORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemorySearch_WORD(pid, atoi(to_value), atoi(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_WORD(pid, atoi(from_value), atoi(to_value), gs, pMap);
            break;
        case BYTE:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemorySearch_BYTE(pid, atoi(to_value), atoi(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_BYTE(pid, atoi(from_value), atoi(to_value), gs, pMap);
            break;
        case QWORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemorySearch_QWORD(pid, atoll(to_value), atoll(from_value), gs, pMap);
            else
                pHead = RangeMemorySearch_QWORD(pid, atoll(from_value), atoll(to_value), gs, pMap);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pHead == NULL) {
        puts("RangeSearch Error");
        return (void) 0;
    }
    ClearResults();
    ResCount = *gs;
    Res = pHead;
}

PMAPS
RangeMemorySearch_DWORD(int pid, int from_value, int to_value, COUNT *gs,
                        PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    int buff[1024] = {0};
    memset(buff, 0, 4);
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j++) {
            pread64(handle, buff, 0x1000, pTemp->addr + j * 4096);
            for (int i = 0; i < 1024; i++) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 4);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS RangeMemorySearch_FLOAT(int pid, float from_value, float to_value, COUNT *gs,
                              PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    float buff[1024] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 1024; i += 1) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 4);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS RangeMemorySearch_DOUBLE(int pid, double from_value, double to_value, int *gs, PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    double buff[512] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 512; i += 1) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 8);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS RangeMemorySearch_WORD(int pid, short from_value, short to_value, int *gs, PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    short buff[2048] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 2048; i += 1) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 2);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS RangeMemorySearch_BYTE(int pid, char from_value, char to_value, int *gs, PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    char buff[4096] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 4096; i += 1) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 1);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS
RangeMemorySearch_QWORD(int pid, long long from_value, long long to_value, int *gs, PMAPS pMap) {
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    long long buff[512] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 512; i += 1) {
                if (buff[i] >= from_value && buff[i] <= to_value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 8);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

void MemorySearch(int pid, char *value, int *gs, TYPE type) {
    PMAPS pHead = NULL;
    PMAPS pMap = NULL;
    switch (MemorySearchRange) {
        case ALL:
            pMap = readmaps(pid, ALL);
            break;
        case B_BAD:
            pMap = readmaps(pid, B_BAD);
            break;
        case C_ALLOC:
            pMap = readmaps(pid, C_ALLOC);
            break;
        case C_BSS:
            pMap = readmaps(pid, C_BSS);
            break;
        case C_DATA:
            pMap = readmaps(pid, C_DATA);
            break;
        case CODE_APP:
            pMap = readmaps_code_app(pid);
            break;
        case C_HEAP:
            pMap = readmaps(pid, C_HEAP);
            break;
        case JAVA_HEAP:
            pMap = readmaps(pid, JAVA_HEAP);
            break;
        case A_ANONMYOUS:
            pMap = readmaps(pid, A_ANONMYOUS);
            break;
        case CODE_SYSTEM:
            pMap = readmaps(pid, CODE_SYSTEM);
            break;
        case STACK:
            pMap = readmaps(pid, STACK);
            break;
        case ASHMEM:
            pMap = readmaps(pid, ASHMEM);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pMap == NULL) {
        puts("map error");
        return (void)0;
    }
    switch (type) {
        case DWORD:
            pHead = MemorySearch_DWORD(pid, atoi(value), gs, pMap);
            break;
        case FLOAT:
            pHead = MemorySearch_FLOAT(pid, atof(value), gs, pMap);
            break;
        case DOUBLE:
            pHead = MemorySearch_DOUBLE(pid, atof(value), gs, pMap);
            break;
        case WORD:
            pHead = MemorySearch_WORD(pid, atoi(value), gs, pMap);
            break;
        case BYTE:
            pHead = MemorySearch_BYTE(pid, (char) atoi(value), gs, pMap);
            break;
        case QWORD:
            pHead = MemorySearch_QWORD(pid, atoll(value), gs, pMap);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pHead == NULL) {
        puts("search error");
        return (void)0;
    }
    ClearResults();
    ResCount = *gs;
    Res = pHead;
}

PMAPS MemorySearch_DWORD(int pid, int value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    int buff[1024] = {0};
    memset(buff, 0, 4);
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j++) {
            pread64(handle, buff, 0x1000, pTemp->addr + j * 4096);
            for (int i = 0; i < 1024; i++) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 4);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS MemorySearch_FLOAT(int pid, float value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    float buff[1024] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 1024; i += 1) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 4);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS MemorySearch_DOUBLE(int pid, double value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    double buff[512] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 512; i += 1) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 8);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS MemorySearch_WORD(int pid, short value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    short buff[2048] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 2048; i += 1) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 2);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS MemorySearch_BYTE(int pid, char value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    char buff[4096] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 4096; i += 1) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 1);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

PMAPS MemorySearch_QWORD(int pid, long long value, COUNT *gs, PMAPS pMap) {
    if (pid == 0) {
        puts("can not get pid");
        return NULL;
    }
    *gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS) malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int handle;
    int iCount = 0;
    int c;
    char lj[64];
    long long buff[512] = {0};
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp->next != NULL) {
        c = (pTemp->taddr - pTemp->addr) / 4096;
        for (int j = 0; j < c; j += 1) {
            pread64(handle, buff, 0x1000, pTemp->addr + (j * 4096));
            for (int i = 0; i < 512; i += 1) {
                if (buff[i] == value) {
                    iCount++;
                    *gs += 1;
                    n->addr = (pTemp->addr) + (j * 4096) + (i * 8);
                    if (iCount == 1) {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    } else {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS) malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
    }
    free(n);
    close(handle);
    return pBuff;
}

void MemoryOffset(int pid, char *value, OFFSET offset, COUNT *gs, TYPE type) {
    PMAPS pHead = NULL;
    switch (type) {
        case DWORD:
            pHead = MemoryOffset_DWORD(pid, atoi(value), offset, Res, gs);
            break;
        case FLOAT:
            pHead = MemoryOffset_FLOAT(pid, atof(value), offset, Res, gs);
            break;
        case DOUBLE:
            pHead = MemoryOffset_DOUBLE(pid, atof(value), offset, Res, gs);
            break;
        case WORD:
            pHead = MemoryOffset_WORD(pid, atoi(value), offset, Res, gs);
            break;
        case BYTE:
            pHead = MemoryOffset_BYTE(pid, value[0], offset, Res, gs);
            break;
        case QWORD:
            pHead = MemoryOffset_QWORD(pid, atoll(value), offset, Res, gs);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pHead == NULL) {
        puts("offset error");
        return (void) 0;
    }
    ClearResults();
    ResCount = *gs;
    Res = pHead;
}

PMAPS MemoryOffset_DWORD(int pid, int value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    int *buf = (int *) malloc(sizeof(int));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 4, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS MemoryOffset_FLOAT(int pid, float value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    float *buf = (float *) malloc(sizeof(float));
    float jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 4, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS MemoryOffset_DOUBLE(int pid, double value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    double *buf = (double *) malloc(sizeof(double));
    double jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 8, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS MemoryOffset_BYTE(int pid, char value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    char *buf = (char *) malloc(sizeof(char));
    char jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 1, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS MemoryOffset_WORD(int pid, short value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    short *buf = (short *) malloc(sizeof(short));
    short jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 2, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS MemoryOffset_QWORD(int pid, long long value, OFFSET offset, PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    long long *buf = (long long *) malloc(sizeof(long long));
    long long jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 8, all);
        jg = *buf;
        if (jg == value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }
        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

void RangeMemoryOffset(int pid, char *from_value, char *to_value, OFFSET offset, COUNT *gs,
                       TYPE type) {
    PMAPS pHead = NULL;
    switch (type) {
        case DWORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemoryOffset_DWORD(pid, atoi(to_value), atoi(from_value), offset, Res,
                                                gs);
            else
                pHead = RangeMemoryOffset_DWORD(pid, atoi(from_value), atoi(to_value), offset, Res,
                                                gs);
            break;
        case FLOAT:
            if (atof(from_value) > atof(to_value))
                pHead = RangeMemoryOffset_FLOAT(pid, atof(to_value), atof(from_value), offset, Res,
                                                gs);
            else
                pHead = RangeMemoryOffset_FLOAT(pid, atof(from_value), atof(to_value), offset, Res,
                                                gs);
            break;
        case DOUBLE:
            if (atof(from_value) > atof(to_value))
                pHead = RangeMemoryOffset_DOUBLE(pid, atof(to_value), atof(from_value), offset, Res,
                                                 gs);
            else
                pHead = RangeMemoryOffset_DOUBLE(pid, atof(from_value), atof(to_value), offset, Res,
                                                 gs);
            break;
        case WORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemoryOffset_WORD(pid, atoi(to_value), atoi(from_value), offset, Res,
                                               gs);
            else
                pHead = RangeMemoryOffset_WORD(pid, atoi(from_value), atoi(to_value), offset, Res,
                                               gs);
            break;
        case BYTE:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemoryOffset_BYTE(pid, atoi(to_value), atoi(from_value), offset, Res,
                                               gs);
            else
                pHead = RangeMemoryOffset_BYTE(pid, atoi(from_value), atoi(to_value), offset, Res,
                                               gs);
            break;
        case QWORD:
            if (atoi(from_value) > atoi(to_value))
                pHead = RangeMemoryOffset_QWORD(pid, atoll(to_value), atoll(from_value), offset,
                                                Res,
                                                gs);
            else
                pHead = RangeMemoryOffset_QWORD(pid, atoll(from_value), atoll(to_value), offset,
                                                Res,
                                                gs);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
    if (pHead == NULL) {
        puts("RangeOffset error");
        return (void) 0;
    }
    ClearResults();
    ResCount = *gs;
    Res = pHead;
}

PMAPS
RangeMemoryOffset_DWORD(int pid, int from_value, int to_value, OFFSET offset, PMAPS pBuff,
                        COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    int *buf = (int *) malloc(sizeof(int));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 4, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;
            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS RangeMemoryOffset_FLOAT(int pid, float from_value, float to_value, OFFSET offset,
                              PMAPS pBuff, COUNT *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    float *buf = (float *) malloc(sizeof(float));

    float jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 4, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;

            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS RangeMemoryOffset_DOUBLE(int pid, double from_value, double to_value, long int offset,
                               PMAPS pBuff, int *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    double *buf = (double *) malloc(sizeof(double));
    double jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 8, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;

            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS RangeMemoryOffset_WORD(int pid, short int from_value, short int to_value, long int offset,
                             PMAPS pBuff, int *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    short *buf = (short *) malloc(sizeof(short));
    short jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 2, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;

            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;

}

PMAPS RangeMemoryOffset_BYTE(int pid, char from_value, char to_value, long int offset, PMAPS pBuff,
                             int *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    char *buf = (char *) malloc(sizeof(char));
    char jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 1, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;

            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

PMAPS RangeMemoryOffset_QWORD(int pid, long long from_value, long long to_value, long int offset,
                              PMAPS pBuff, int *gs) {
    *gs = 0;
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS) malloc(LEN);
    BUFF = pNew;
    int iCount = 0, handle;
    char lj[64];
    long int all;
    long long *buf = (long long *) malloc(sizeof(long long));
    long long jg;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    while (pTemp != NULL) {
        all = pTemp->addr + offset;
        pread64(handle, buf, 8, all);
        jg = *buf;
        if (jg >= from_value && jg <= to_value) {
            iCount++;
            *gs += 1;

            pNew->addr = pTemp->addr;
            if (iCount == 1) {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            } else {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS) malloc(LEN);
            if (ResCount == 1) {
                free(pNew);
                close(handle);
                return BUFF;
            }
        }

        pTemp = pTemp->next;
    }
    free(pNew);
    close(handle);
    return BUFF;
}

char *MemoryRead(int pid, long address, int type) {
    switch (type) {
        case DWORD:
            return intToChar(MemoryRead_DWORD(pid, address));
            break;
        case FLOAT:
            return floatToChar(MemoryRead_FLOAT(pid, address));
            break;
        case DOUBLE:
            return doubleToChar(MemoryRead_DOUBLE(pid, address));
            break;
        case WORD:
            return intToChar(MemoryRead_WORD(pid, address));
            break;
        case BYTE:
            return intToChar(MemoryRead_BYTE(pid, address));
            break;
        case QWORD:
            return longlongToChar(MemoryRead_QWORD(pid, address));
        default:
            return NULL;
            break;
    }
}

int MemoryRead_DWORD(int pid, long address) {
    char lj[64];
    int *buf = (int *) malloc(sizeof(int));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 4, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

float MemoryRead_FLOAT(int pid, long address) {
    char lj[64];
    float *buf = (float *) malloc(4);
    float jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 4, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

double MemoryRead_DOUBLE(int pid, long address) {
    char lj[64];
    double *buf = (double *) malloc(8);
    double jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 8, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

short int MemoryRead_WORD(int pid, long address) {
    char lj[64];
    short *buf = (short *) malloc(sizeof(short));
    short int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 2, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

char MemoryRead_BYTE(int pid, long address) {
    char lj[64];
    char *buf = (char *) malloc(sizeof(char));
    char jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 1, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

long long MemoryRead_QWORD(int pid, long address) {
    char lj[64];
    long long *buf = (long long *) malloc(sizeof(long long));
    long long jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pread64(handle, buf, 8, address);
    jg = *buf;
    free(buf);
    close(handle);
    return jg;
}

void MemoryWrite(int pid, char *value, long address, TYPE type) {
    switch (type) {
        case DWORD:
            MemoryWrite_DWORD(pid, atoi(value), address);
            break;
        case FLOAT:
            MemoryWrite_FLOAT(pid, atof(value), address);
            break;
        case DOUBLE:
            MemoryWrite_DOUBLE(pid, atof(value), address);
            break;
        case WORD:
            MemoryWrite_WORD(pid, atoi(value), address);
            break;
        case BYTE:
            MemoryWrite_BYTE(pid, (char) atoi(value), address);
            break;
        case QWORD:
            MemoryWrite_QWORD(pid, atoll(value), address);
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
    }
}

int MemoryWrite_DWORD(int pid, int value, long address) {
    char lj[64];
    int *buf = (int *) malloc(sizeof(int));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 4, address);
    free(buf);
    close(handle);
    return jg;
}

int MemoryWrite_FLOAT(int pid, float value, long address) {
    char lj[64];
    float *buf = (float *) malloc(sizeof(float));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 4, address);
    free(buf);
    close(handle);
    return jg;
}

int MemoryWrite_DOUBLE(int pid, double value, long address) {
    char lj[64];
    double *buf = (double *) malloc(sizeof(double));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 8, address);
    free(buf);
    close(handle);
    return jg;
}

int MemoryWrite_WORD(int pid, short int value, long address) {
    char lj[64];
    short *buf = (short *) malloc(sizeof(short));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 2, address);
    free(buf);
    close(handle);
    return jg;
}

int MemoryWrite_BYTE(int pid, char value, long address) {
    char lj[64];
    char *buf = (char *) malloc(sizeof(char));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 1, address);
    free(buf);
    close(handle);
    return jg;
}

int MemoryWrite_QWORD(int pid, long long value, long address) {
    char lj[64];
    long long *buf = (long long *) malloc(sizeof(long long));
    int jg;
    sprintf(lj, "/proc/%d/mem", pid);
    int handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    *buf = value;
    jg = pwrite64(handle, buf, 8, address);
    free(buf);
    close(handle);
    return jg;
}

int killProcessByOs(PACKAGENAME *bm) {
    int pid = getPID(bm);
    if (pid == 0) {
        return -1;
    }
    char ml[32];
    sprintf(ml, "kill %d", pid);
    system(ml);
    return 0;
}

char GetProcessState(PACKAGENAME *bm) {

    int pid = getPID(bm);
    if (pid == 0) {
        return 0;
    }
    FILE *fp;
    char lj[64];
    char buff[64];
    char zt;
    char zt1[16];
    sprintf(lj, "/proc/%d/status", pid);
    fp = fopen(lj, "r");
    if (fp == NULL) {
        return 0;
    }

    while (!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        if (strstr(buff, "State")) {
            sscanf(buff, "State: %c", &zt);

            break;
        }
    }
    fclose(fp);
    return zt;
}

int killXs() {
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    char filepath[256];
    char filetext[128];
    dir = opendir("/data/data");
    FILE *fp = NULL;
    if (NULL != dir) {
        while ((ptr = readdir(dir)) != NULL) {

            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (ptr->d_type != DT_DIR)
                continue;

            sprintf(filepath, "/data/data/%s/lib/libxscript.so", ptr->d_name);
            fp = fopen(filepath, "r");
            if (fp == NULL)
                continue;
            else {
                killProcessByOs(ptr->d_name);
            }
        }
    }
    closedir(dir);
    return 0;
}

void *FreezeThread(void *a) {
    int handle;
    char lj[64];
    int buf_i;
    float buf_f;
    double buf_d;
    short buf_s;
    char buf_c;
    long long buf_ll;
    int pid = *(int *) a;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    if (handle == -1) {
        puts("Error -2");
        return (void *) 0;
    }
    lseek(handle, 0, SEEK_SET);
    PFREEZE pTemp = Pfreeze;
    while (Freeze == 1) {
        for (int i = 0; i < FreezeCount; i++) {
            switch (pTemp->type) {
                case DWORD:
                    buf_i = atoi(pTemp->value);
                    pwrite64(handle, &buf_i, 4, pTemp->addr);
                    break;
                case FLOAT:
                    buf_f = atof(pTemp->value);
                    pwrite64(handle, &buf_f, 4, pTemp->addr);
                    break;
                case DOUBLE:
                    buf_d = atof(pTemp->value);
                    pwrite64(handle, &buf_d, 8, pTemp->addr);
                case WORD:
                    buf_s = atoi(pTemp->value);
                    pwrite64(handle, &buf_s, 2, pTemp->addr);
                case BYTE:
                    buf_c = (char) atoi(pTemp->value);
                    pwrite64(handle, &buf_c, 1, pTemp->addr);
                case QWORD:
                    buf_ll = atoll(pTemp->value);
                    pwrite64(handle, &buf_ll, 8, pTemp->addr);
                default:
                    break;
            }
            pTemp = pTemp->next;
        }
        pTemp = Pfreeze;
        usleep(delay);
    }
    return NULL;
}

void AddFreezeItem(int pid, ADDRESS addr, char *value, TYPE type) {
    switch (type) {
        case DWORD:
            AddFreezeItem_DWORD(pid, addr, value);
            break;
        case FLOAT:
            AddFreezeItem_FLOAT(pid, addr, value);
            break;
        case DOUBLE:
            AddFreezeItem_DOUBLE(pid, addr, value);
            break;
        case WORD:
            AddFreezeItem_WORD(pid, addr, value);
            break;
        case BYTE:
            AddFreezeItem_BYTE(pid, addr, value);
            break;
        case QWORD:
            AddFreezeItem_QWORD(pid, addr, value);
            break;
        default:
            SetTextColor(COLOR_SKY_BLUE);
            puts("You Choose a NULL type");
            break;
    }
}

void AddFreezeItem_DWORD(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = DWORD;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = DWORD;
        pNew->value = value;
        FreezeCount += 1;
    }
}

void AddFreezeItem_FLOAT(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = FLOAT;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = FLOAT;
        pNew->value = value;
        FreezeCount += 1;
    }
}

void AddFreezeItem_DOUBLE(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = DOUBLE;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = DOUBLE;
        pNew->value = value;
        FreezeCount += 1;
    }
}

void AddFreezeItem_WORD(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = WORD;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = WORD;
        pNew->value = value;
        FreezeCount += 1;
    }
}

void AddFreezeItem_BYTE(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = BYTE;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = BYTE;
        pNew->value = value;
        FreezeCount += 1;
    }
}

void AddFreezeItem_QWORD(int pid, ADDRESS addr, char *value) {
    if (FreezeCount == 0) {
        Pfreeze = pEnd = pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd = pNew;
        Pfreeze = pNew;
        pNew->addr = addr;
        pNew->type = QWORD;
        pNew->value = value;
        FreezeCount += 1;
    } else {
        pNew = (PFREEZE) malloc(FRE);
        pNew->next = NULL;
        pEnd->next = pNew;
        pEnd = pNew;
        pNew->addr = addr;
        pNew->type = QWORD;
        pNew->value = value;
        FreezeCount += 1;
    }
}

int RemoveFreezeItem(ADDRESS addr) {
    PFREEZE pTemp = Pfreeze;
    PFREEZE p1 = NULL;
    PFREEZE p2 = NULL;
    for (int i = 0; i < FreezeCount; i++) {
        p1 = pTemp;
        p2 = pTemp->next;
        if (pTemp->addr == addr) {
            p1->next = p2;
            free(pTemp);
            FreezeCount -= 1;
        }
        pTemp = p2;
    }
    return 0;
}

int RemoveFreezeItem_All() {
    PFREEZE pHead = Pfreeze;
    PFREEZE pTemp = pHead;
    int i;
    for (i = 0; i < FreezeCount; i++) {
        pTemp = pHead;
        pHead = pHead->next;
        free(pTemp);
        FreezeCount -= 1;
    }
    free(Pfreeze);
    FreezeCount -= 1;
    return 0;
}

int StartFreeze(int pid) {
    if (Freeze == 1) {
        return -1;
    }
    int a;
    Freeze = 1;
    pthread_create(&pth, NULL, FreezeThread, (void *) &pid);
    return 0;
}

int StopFreeze() {
    Freeze = 0;
    return 0;
}

int SetFreezeDelay(long int De) {
    delay = De;
    return 0;
}
