package com.moshuixin.memory.tools;

import android.content.Context;
import android.os.RemoteException;
import android.util.Log;

import com.stericson.RootShell.RootShell;
import com.stericson.RootShell.exceptions.RootDeniedException;
import com.stericson.RootShell.execution.Command;
import com.stericson.RootShell.execution.Shell;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeoutException;

import com.moshuixin.memory.IRootProcess;
import com.moshuixin.memory.Main;
import com.moshuixin.memory.data.Result;
import com.moshuixin.memory.ess.MemoryToolsNative;
import com.moshuixin.proces.LaunchScript;
import com.moshuixin.proces.RootIPCReceiver;

public class MemoryTools {
    private static final String TAG = MemoryTools.class.getName();
    private static final Object lock = new Object();
    private static final MemoryTools tools = new MemoryTools();
    private static final ArrayList<IRootProcess> ipcList = new ArrayList<>();
    private static RootIPCReceiver<IRootProcess> receiver;

    private boolean root = false;
    private int SelectPid = 0;

    private MemoryTools() {
    }

    public static MemoryTools getInstance() {
        return tools;
    }

    public void selectAppByPid(int pid) {
        this.SelectPid = pid;
    }

    public void selectAppByPackage(String packageName) throws RemoteException {
        this.SelectPid = getProcessPid(packageName);
    }

    public int getSelectApp() {
        return SelectPid;
    }

    public void killGG() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().killGG();
                }
            }
        } else {
            MemoryToolsNative.killGG();
        }

    }

    public void killXs() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().killXs();
                }
            }
        } else {
            MemoryToolsNative.killXs();
        }
    }

    public void byPassGameSafe() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().passGameSafe();
                }
            }
        } else {
            MemoryToolsNative.withPassGameSafe();
        }
    }

    public int getProcessPid(String packageName) throws RemoteException {
        int pid = 0;
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    pid = getIpc().getProcessPid(packageName);
                }
            }
        } else {
            pid = MemoryToolsNative.getPid(packageName);
        }
        //if (pid == 0) throw new MemoryExt.NotFoundPidException();
        return pid;
    }

    public ProcessState getProcessState(String packageName) throws RemoteException {
        char state;
        if (root) {
            state = getIpc().getProcessState(packageName);
        } else {
            state = MemoryToolsNative.getProcessState(packageName);
        }
        if (state == (char) 0) { // 程序没有找到导致无法获取状态
            throw new MemoryExt.UnknownException();
        }
        switch (state) {
            case 'D':
            case 'd':
                return ProcessState.DORMANT;
            case 'R':
            case 'r':
                return ProcessState.RUNNING;
            case 'S':
                return ProcessState.SLEEP;
            case 'T':
            case 't':
                return ProcessState.TRACK;
            case 'W':
            case 'w':
                return ProcessState.WAKING;
            case 'X':
            case 'x':
                return ProcessState.DEAD;
            case 'Z':
            case 'z':
                return ProcessState.ZOMBIE;
            case '<':
                return ProcessState.NOBLE;
            case 'N':
            case 'n':
                return ProcessState.LOW;
            case 'L':
                return ProcessState.LOCK;
            case 's':
                return ProcessState.LEADER;
            case 'l':
                return ProcessState.MULTI;
            case '+':
                return ProcessState.BACKSTAGE;
            default:
                throw new IllegalStateException("unknown state type");
        }
    }

    public void setRange(int range) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().setRange(range);
                }
            }
        } else {
            MemoryToolsNative.setRange(range);
        }
    }

    public int getRange() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    return getIpc().getRange();
                }
            }
        } else {
            MemoryToolsNative.getRange();
        }
        return 0;
    }

    public long getModuleBase(String name, int index) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    return getIpc().getModuleBase(SelectPid, name, index);
                }
            }
        } else {
            return MemoryToolsNative.getModuleBase(SelectPid, name, index);
        }
        return 0;
    }

    public void searchMemory(String value, int type) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().searchMemory(SelectPid, value, type);
                }
            }
        } else {
            if (value.contains("~")) {
                var v = value.split("~");
                String fv = v[0];
                String tv = v[1];
                MemoryToolsNative.searchRangeMemory(SelectPid, fv, tv, type);
            } else {
                MemoryToolsNative.searchMemory(SelectPid, value, type);
            }
        }
    }

    public void refineMemory(String value, int type, long offset) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().refineMemory(SelectPid, value, type, offset);
                }
            }
        } else {
            if (value.contains("~")) {
                var v = value.split("~");
                String fv = v[0];
                String tv = v[1];
                MemoryToolsNative.refineRangeMemory(SelectPid, fv, tv, offset, type);
            } else {
                MemoryToolsNative.refineMemory(SelectPid, value, type, offset);
            }
        }
    }

    public int getResultsCount() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    return getIpc().getResultsCount();
                }
            }
        } else {
           return MemoryToolsNative.getResultsCount();
        }
        return 0;
    }

    public List<Result> getResults(int start, int end) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    return getIpc().getResults(start, end);
                }
            }
        } else {
            MemoryToolsNative.getResults(start, end);
        }
        return null;
    }

    public void clearResults() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().clearResults();
                }
            }
        } else {
            MemoryToolsNative.clearResults();
        }
    }

    public String getMemory(long address, int type) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    return getIpc().getMemory(SelectPid, address, type);
                }
            }
        } else {
            return MemoryToolsNative.getMemory(SelectPid, address, type);
        }
        return null;
    }

    public void setMemory(String value, long address, int type) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().setMemory(SelectPid, value, address, type);
                }
            }
        } else {
            MemoryToolsNative.setMemory(SelectPid, value, address, type);
        }
    }

    public void addFreezeItem(String value, long address, int type) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().addFreezeItem(SelectPid, value, address, type);
                }
            }
        } else {
            MemoryToolsNative.addFreezeItem(SelectPid, value, address, type);
        }
    }

    public void clearFreezeItem(long address) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().clearFreezeItem(address);
                }
            }
        } else {
            MemoryToolsNative.clearFreezeItem(address);
        }
    }

    public void clearAllFreezeItem() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().clearAllFreezeItem();
                }
            }
        } else {
            MemoryToolsNative.clearAllFreezeItem();
        }
    }

    public void startFreeze(int pid) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().startFreeze(SelectPid);
                }
            }
        } else {
            MemoryToolsNative.startFreeze(SelectPid);
        }
    }

    public void stopFreeze() throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().stopFreeze();
                }
            }
        } else {
            MemoryToolsNative.stopFreeze();
        }
    }

    public void setFreezeDelay(long delay) throws RemoteException {
        if (root) {
            synchronized (lock) {
                if (!ipcList.isEmpty()) {
                    getIpc().setFreezeDelay(delay);
                }
            }
        } else {
            MemoryToolsNative.setFreezeDelay(delay);
        }
    }

    /*
    下面的方法，与你无关
     */
    private IRootProcess getIpc() {
        return receiver.getIPC();
    }

    public final boolean init(Context ctx, boolean isRoot) {
        root = isRoot;
        if (isRoot) {
            if (RootShell.isAccessGiven(5 * 1000, 3)) {
                createIpc(ctx);
                return true;
            }
            return false;
        } else {
            System.loadLibrary("memory");
            return true;
        }
    }

    public final void createIpc(Context ctx) {
        synchronized (lock) {
            if (receiver == null) {
                receiver = new RootIPCReceiver<IRootProcess>(ctx, Main.CODE_MEMORY) {
                    @Override
                    public void onConnect(@NotNull IRootProcess ipc) {
                        ipcList.add(ipc);
                        Log.e(TAG, "onConnect: " + ipcList);
                    }

                    @Override
                    public void onDisconnect(@NotNull IRootProcess ipc) {
                        ipcList.remove(ipc);
                        if (ipcList.isEmpty()) {
                            createIpc(ctx); // 尝试重连
                        }
                    }
                };
            }
            String libPath = MemoryExt.getLibraryPath(ctx, "memory");
            String script = LaunchScript.getLaunchString(ctx, Main.class, Main.PROCESS_NAME, Main.CODE_MEMORY, new String[]{libPath});
            Shell shell = null;
            try {
                shell = RootShell.getShell(true);
                shell.add(new Command(0, "cd " + ctx.getFilesDir().getAbsolutePath()));
                shell.add(new Command(1, script));
            } catch (IOException | TimeoutException | RootDeniedException e) {
                Log.e(TAG, "createIpc: ", e);
            }
        }
    }
}
