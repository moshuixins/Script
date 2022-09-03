package com.moshuixin.memory.ess;

import java.util.ArrayList;

import com.moshuixin.memory.data.Result;

public class MemoryToolsNative {
    public static native void killGG();

    public static native void killXs();

    public static native void withPassGameSafe();

    public static native int getPid(String packageName);

    public static native char getProcessState(String packageName);

    public static native void setRange(int range);

    public static native int getRange();

    public static native long getModuleBase(int pid, String name, int index);

    public static native void searchMemory(int pid, String value, int type);

    public static native void searchRangeMemory(int pid, String from_value, String to_value, int type);

    public static native void refineMemory(int pid, String value, int type, long offset);

    public static native void refineRangeMemory(int pid, String from_value, String to_value, long offset, int type);

    public static native int getResultsCount();

    public static native ArrayList<Result> getResults(int start, int end);

    public static native void clearResults();

    public static native String getMemory(int pid, long address, int type);

    public static native void setMemory(int pid, String value, long address, int type);

    public static native void addFreezeItem(int pid, String value, long address, int type);

    public static native void clearFreezeItem(long address);

    public static native void clearAllFreezeItem();

    public static native void startFreeze(int pid);

    public static native void stopFreeze();

    public static native void setFreezeDelay(long delay);

}
