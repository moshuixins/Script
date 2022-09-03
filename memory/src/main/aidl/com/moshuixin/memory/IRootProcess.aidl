// IRootProcess.aidl
package com.moshuixin.memory;
import com.moshuixin.memory.data.Result;
interface IRootProcess {
    int getPid();
    int getUid();

    void killGG();
    void killXs();
    void passGameSafe();

    int getProcessPid(String packageName);
    char getProcessState(String packageName);


    void setRange(int range);
    int getRange();
    long getModuleBase(int pid, String name, int index);
    void searchMemory(int pid, String value,int type);
    void refineMemory(int pid, String value, int type, long offset);
    int getResultsCount();
    List<Result> getResults(int start, int end);
    void clearResults();
    String getMemory(int pid,long address,int type);
    void setMemory(int pid, String value, long address, int type);
    void addFreezeItem(int pid,String value, long address, int type);
    void clearFreezeItem(long address);
    void clearAllFreezeItem();
    void startFreeze(int pid);
    void stopFreeze();
    void setFreezeDelay(long delay);
}