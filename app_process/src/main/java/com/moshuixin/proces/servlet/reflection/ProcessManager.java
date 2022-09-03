package com.moshuixin.proces.servlet.reflection;

import android.annotation.SuppressLint;

import java.lang.reflect.Method;

@SuppressLint("PrivateApi")
public class ProcessManager {
    public static void setAppName(String name, int userId) {
        try {
            Class<?> cDdmHandleAppName = Class.forName("android.ddm.DdmHandleAppName");
            Method m = cDdmHandleAppName.getDeclaredMethod(
                    "setAppName",
                    String.class,
                    Integer.class
            );
            m.invoke(null, name, userId);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
