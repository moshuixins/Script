package com.moshuixin.memory;

import android.annotation.SuppressLint;
import android.os.IBinder;

import java.util.ArrayList;

import com.moshuixin.memory.ess.MemoryBinder;
import com.moshuixin.proces.ProcessMain;

@SuppressLint("UnsafeDynamicallyLoadedCode")
public class Main extends ProcessMain<Main> {
    /*
    服务所对应的code
     */
    public static final int CODE_MEMORY = 0;
    public static final String PROCESS_NAME = "RootProcess";

    @Override
    public void run(ArrayList<String> args) {
        System.out.println("RootProcess ==> 载入成功");
        try {
            System.out.println("RootProcess ==> 开始初始化服务");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public IBinder getBinder(int code) {
        if (code == CODE_MEMORY) {
            return new MemoryBinder();
        }
        return super.getBinder(code);
    }
}