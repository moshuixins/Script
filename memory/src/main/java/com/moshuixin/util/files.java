package com.moshuixin.util;

import android.content.Context;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;

import com.stardust.pio.PFileInterface;
import com.stardust.pio.PFiles;
import com.stardust.pio.UncheckedIOException;
import com.stardust.util.Func1;

import java.io.File;
import java.io.IOException;

/**
 * Created by Stardust on 2018/1/23.
 */

public class files {

    static public String readAssets(Context context, String path, String encoding) {
        try {
            return PFiles.read(context.getAssets().open(path), encoding);
        } catch (IOException e) {
            throw new UncheckedIOException(e);
        }
    }

    static public String readAssets(Context context, String path) {
        return readAssets(context, path, "UTF-8");
    }


    static public PFileInterface open(String path, String mode, String encoding, int bufferSize) {
        return PFiles.open(path, mode, encoding, bufferSize);
    }


    static public Object open(String path, String mode, String encoding) {
        return PFiles.open(path, mode, encoding);
    }


    static public Object open(String path, String mode) {
        return PFiles.open(path, mode);
    }


    static public Object open(String path) {
        return PFiles.open(path);
    }


    static public boolean create(String path) {
        return PFiles.create(path);
    }


    static public boolean createIfNotExists(String path) {
        return PFiles.createIfNotExists(path);
    }


    static public boolean createWithDirs(String path) {
        return PFiles.createWithDirs(path);
    }


    static public boolean exists(String path) {
        return PFiles.exists(path);
    }


    static public boolean ensureDir(String path) {
        return PFiles.ensureDir(path);
    }


    static public String read(String path, String encoding) {
        return PFiles.read(path, encoding);
    }


    static public String read(String path) {
        return PFiles.read(path);
    }


    static public byte[] readBytes(String path) {
        return PFiles.readBytes(path);
    }


    static public void write(String path, String text) {
        PFiles.write(path, text);
    }


    static public void write(String path, String text, String encoding) {
        PFiles.write(path, text, encoding);
    }


    static public void append(String path, String text) {
        PFiles.append(path, text);
    }


    static public void append(String path, String text, String encoding) {
        PFiles.append(path, text, encoding);
    }


    static public void appendBytes(String path, byte[] bytes) {
        PFiles.appendBytes(path, bytes);
    }


    static public void writeBytes(String path, byte[] bytes) {
        PFiles.writeBytes(path, bytes);
    }


    static public boolean copy(String pathFrom, String pathTo) {
        return PFiles.copy(pathFrom, pathTo);
    }


    static public boolean renameWithoutExtension(String path, String newName) {
        return PFiles.renameWithoutExtension(path, newName);
    }


    static public boolean rename(String path, String newName) {
        return PFiles.rename(path, newName);
    }


    static public boolean move(String path, String newPath) {
        return PFiles.move(path, newPath);
    }


    static public String getExtension(String fileName) {
        return PFiles.getExtension(fileName);
    }


    static public String getName(String filePath) {
        return PFiles.getName(filePath);
    }


    static public String getNameWithoutExtension(String filePath) {
        return PFiles.getNameWithoutExtension(filePath);
    }


    static public boolean remove(String path) {
        return PFiles.remove(path);
    }


    static public boolean removeDir(String path) {
        return PFiles.removeDir(path);
    }


    static public String getSdcardPath() {
        return PFiles.getSdcardPath();
    }


    static public String getExternalCacheDirectory(Context context, String type) {
        File appCacheDir = null;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            if (TextUtils.isEmpty(type)) {
                appCacheDir = context.getExternalCacheDir();
            } else {
                appCacheDir = context.getExternalFilesDir(type);
            }

            if (appCacheDir == null) {// 有些手机需要通过自定义目录
                appCacheDir = new File(Environment.getExternalStorageDirectory(), "Android/data/" + context.getPackageName() + "/cache/" + type);
            }

            if (!appCacheDir.exists() && !appCacheDir.mkdirs()) {
                Log.e("getExternalDirectory", "getExternalDirectory fail ,the reason is make directory fail !");
            }
        } else {
            Log.e("getExternalDirectory", "getExternalDirectory fail ,the reason is sdCard nonexistence or sdCard mount fail !");
        }
        return String.valueOf(appCacheDir);
    }


    static public String getInternalCacheDirectory(Context context, String type) {
        File appCacheDir = null;
        if (TextUtils.isEmpty(type)) {
            appCacheDir = context.getCacheDir();// /data/data/app_package_name/cache
        } else {
            appCacheDir = new File(context.getFilesDir(), type);// /data/data/app_package_name/files/type
        }

        if (!appCacheDir.exists() && !appCacheDir.mkdirs()) {
            Log.e("getInternalDirectory", "getInternalDirectory fail ,the reason is make directory fail !");
        }
        return String.valueOf(appCacheDir);
    }

    static public String[] listDir(String path) {
        return PFiles.listDir(path);
    }


    static public String[] listDir(String path, Func1<String, Boolean> filter) {
        return PFiles.listDir(path, filter);
    }


    static public boolean isFile(String path) {
        return PFiles.isFile(path);
    }


    static public boolean isDir(String path) {
        return PFiles.isDir(path);
    }


    static public boolean isEmptyDir(String path) {
        return PFiles.isEmptyDir(path);
    }


    static public String join(String parent, String child) {
        return PFiles.join(parent, child);
    }


    static public String getHumanReadableSize(long bytes) {
        return PFiles.getHumanReadableSize(bytes);
    }


    static public String getSimplifiedPath(String path) {
        return PFiles.getSimplifiedPath(path);
    }

}

