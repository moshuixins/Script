package com.moshuixin.util;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.os.Build;
import android.media.AudioManager;
import android.os.PowerManager;
import android.os.Vibrator;
import java.util.UUID;

public class device {
    public static String buildId = Build.ID;
    public static String brand = Build.BRAND;
    public static String board = Build.BOARD;
    public static String device = Build.DEVICE;
    public static String product = Build.PRODUCT;
    public static String model = Build.MODEL;
    public static String bootloader = Build.BOOTLOADER;
    public static String hardware = Build.HARDWARE;
    public static String fingerprint = Build.FINGERPRINT;
    public static String serial = Build.SERIAL;
    public static String display = Build.DISPLAY;
    public static String manufacturer = Build.MANUFACTURER;

    public static int sdkInt = Build.VERSION.SDK_INT;
    public static String incremental = Build.VERSION.INCREMENTAL;
    public static String release = Build.VERSION.RELEASE;
    public static String codename = Build.VERSION.CODENAME;
    public static String baseOS = Build.VERSION.BASE_OS;
    public static String securityPatch = Build.VERSION.SECURITY_PATCH;

    public static int getWidth(Context context) {
        return context.getResources().getDisplayMetrics().widthPixels;
    }

    public static int getHeight(Context context) {
        return context.getResources().getDisplayMetrics().heightPixels;
    }

    public static String getAndroidId(Context context) {
        return android.provider.Settings.Secure.getString(context.getContentResolver(),
                android.provider.Settings.Secure.ANDROID_ID);
    }

    public static String getUniqueId(){
        String m_szDevIDShort = "35" + Build.BOARD.length() % 10
                + Build.BRAND.length() % 10 + Build.CPU_ABI.length() % 10
                + Build.DEVICE.length() % 10 + Build.DISPLAY.length() % 10
                + Build.HOST.length() % 10 + Build.ID.length() % 10
                + Build.MANUFACTURER.length() % 10 + Build.MODEL.length() % 10
                + Build.PRODUCT.length() % 10 + Build.TAGS.length() % 10
                + Build.TYPE.length() % 10 + Build.USER.length() % 10;
        String serial = "serial";
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                serial = Build.getSerial();
            }
            else {
                serial = Build.SERIAL;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return new UUID(m_szDevIDShort.hashCode(), serial.hashCode()).toString();
    }
    

    public static int getBrightness(Context context) {
        return android.provider.Settings.System.getInt(context.getContentResolver(),
                android.provider.Settings.System.SCREEN_BRIGHTNESS, 0);
    }

    public static int getBrightnessMode(Context context) {
        return android.provider.Settings.System.getInt(context.getContentResolver(),
                android.provider.Settings.System.SCREEN_BRIGHTNESS_MODE, 0);
    }

    public static boolean setBrightness(Context context, int b) {
        return android.provider.Settings.System.putInt(context.getContentResolver(),
                android.provider.Settings.System.SCREEN_BRIGHTNESS, b);
    }

    public static boolean setBrightnessMode(Context context, int mode) {
        return android.provider.Settings.System.putInt(context.getContentResolver(),
                android.provider.Settings.System.SCREEN_BRIGHTNESS_MODE, mode);
    }

    public static int getMusicVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
    }

    public static int getMusicMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
    }

    public static boolean setMusicVolume(Context context, int v) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, v, AudioManager.FLAG_PLAY_SOUND);
        return true;
    }

    public static int getNotificationVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_NOTIFICATION);
    }

    public static int getNotificationMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamMaxVolume(AudioManager.STREAM_NOTIFICATION);
    }

    public static boolean setNotificationVolume(Context context, int v) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_NOTIFICATION, v, AudioManager.FLAG_PLAY_SOUND);
        return true;
    }

    public static int getSystemVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_SYSTEM);
    }

    public static int getSystemMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamMaxVolume(AudioManager.STREAM_SYSTEM);
    }

    public static boolean setSystemVolume(Context context, int v) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_SYSTEM, v, AudioManager.FLAG_PLAY_SOUND);
        return true;
    }

    public static int getVoiceCallVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
    }

    public static int getVoiceCallMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL);
    }

    public static boolean setVoiceCallVolume(Context context, int v) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, v, AudioManager.FLAG_PLAY_SOUND);
        return true;
    }

    public static int getAlarmVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_ALARM);
    }

    public static int getAlarmMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamMaxVolume(AudioManager.STREAM_ALARM);
    }

    public static boolean setAlarmVolume(Context context, int v) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_ALARM, v, AudioManager.FLAG_PLAY_SOUND);
        return true;
    }

    public static float getBattery(Context context) {
        Intent batteryIntent = context.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        int level = batteryIntent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = batteryIntent.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
        return ((level / (float) scale) * 100);
    }

    public static boolean isCharging(Context context) {
        Intent batteryIntent = context.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        int status = batteryIntent.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
        return status == BatteryManager.BATTERY_STATUS_CHARGING || status == BatteryManager.BATTERY_STATUS_FULL;
    }

    public static long getTotalMem(Context context) {
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);
        return mi.totalMem;
    }

    public static long getAvailMem(Context context) {
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);
        return mi.availMem;
    }

    public static boolean isScreenOn(Context context) {
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        return pm.isScreenOn();
    }

    private static void wakeUp(Context context, String TAG) {
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
        wl.acquire();
    }

    public static void wakeUp(Context context) {
        wakeUp(context, "wakeUp");
    }

    public static void wakeUpIfNeeded(Context context) {
        if (!isScreenOn(context)) {
            wakeUp(context, "wakeUpIfNeeded");
        }
    }

    private static void keepScreenOn(Context context, int timeout, String TAG) {
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
        wl.acquire(timeout);
    }

    public static void keepScreenOn(Context context, int timeout) {
        keepScreenOn(context, timeout, "keepScreenOn");
    }

    private static void cancelKeepingAwake(Context context, String TAG) {
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
        wl.release();
    }

    public static void cancelKeepingAwake(Context context) {
        cancelKeepingAwake(context, "cancelKeepingAwake");
    }

    public static void vibrate(Context context, long milliseconds) {
        Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        vibrator.vibrate(milliseconds);
    }

    public static void cancelVibration(Context context) {
        Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        vibrator.cancel();
    }
}
