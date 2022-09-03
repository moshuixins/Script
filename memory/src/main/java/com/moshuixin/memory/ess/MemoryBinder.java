package com.moshuixin.memory.ess;


import java.util.ArrayList;

import com.moshuixin.memory.IRootProcess;
import com.moshuixin.memory.data.Result;

public class MemoryBinder extends IRootProcess.Stub {


    @Override
    public int getPid() {
        return android.os.Process.myPid();
    }

    @Override
    public int getUid() {
        return android.os.Process.myUid();
    }

    @Override
    public void killGG() {
        MemoryToolsNative.killGG();
    }

    @Override
    public void killXs() {
        MemoryToolsNative.killXs();
    }


    @Override
    public void passGameSafe() {
        MemoryToolsNative.withPassGameSafe();
    }

    @Override
    public int getProcessPid(String packageName) {
        return MemoryToolsNative.getPid(packageName);
    }


    @Override
    public char getProcessState(String packageName) {
        return MemoryToolsNative.getProcessState(packageName);
    }

    @Override
    public void setRange(int range) {
        MemoryToolsNative.setRange(range);
    }

    @Override
    public int getRange() {
        return MemoryToolsNative.getRange();
    }

    @Override
    public long getModuleBase(int pid, String name, int index) {
        return MemoryToolsNative.getModuleBase(pid, name, index);
    }

    @Override
    public void searchMemory(int pid, String value, int type) {
        if (value.contains("~")) {
            var v = value.split("~");
            String fv = v[0];
            String tv = v[1];
            MemoryToolsNative.searchRangeMemory(pid, fv, tv, type);
        } else {
            MemoryToolsNative.searchMemory(pid, value, type);
        }
    }

    @Override
    public void refineMemory(int pid, String value, int type, long offset) {
        if (value.contains("~")) {
            var v = value.split("~");
            String fv = v[0];
            String tv = v[1];
            MemoryToolsNative.refineRangeMemory(pid, fv, tv, offset, type);
        } else {
            MemoryToolsNative.refineMemory(pid, value, type, offset);
        }
    }

    @Override
    public int getResultsCount() {
        return MemoryToolsNative.getResultsCount();
    }

    @Override
    public ArrayList<Result> getResults(int start, int end) {
        return MemoryToolsNative.getResults(start, end);
    }

    @Override
    public void clearResults() {
        MemoryToolsNative.clearResults();
    }

    @Override
    public String getMemory(int pid, long address, int type) {
        return MemoryToolsNative.getMemory(pid, address, type);
    }

    @Override
    public void setMemory(int pid, String value, long address, int type) {
        MemoryToolsNative.setMemory(pid, value, address, type);
    }

    @Override
    public void addFreezeItem(int pid, String value, long address, int type) {
        MemoryToolsNative.addFreezeItem(pid, value, address, type);
    }

    @Override
    public void clearFreezeItem(long address) {
        MemoryToolsNative.clearFreezeItem(address);
    }

    @Override
    public void clearAllFreezeItem() {
        MemoryToolsNative.clearAllFreezeItem();
    }

    @Override
    public void startFreeze(int pid) {
        MemoryToolsNative.startFreeze(pid);
    }

    @Override
    public void stopFreeze() {
        MemoryToolsNative.stopFreeze();
    }

    @Override
    public void setFreezeDelay(long delay) {
        MemoryToolsNative.setFreezeDelay(delay);
    }

}
