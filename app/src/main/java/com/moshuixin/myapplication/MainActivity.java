package com.moshuixin.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.material.textfield.TextInputEditText;

import com.moshuixin.memory.tools.*;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        MemoryTools.getInstance().init(this, true);
        TextView text = (TextView) findViewById(R.id.text1);
        Button button = (Button) findViewById(R.id.button);

        button.setOnClickListener(v -> {
            try {
                MemoryTools.getInstance().selectAppByPackage("gg.pointers");
                long add = MemoryTools.getInstance().getModuleBase("libgame.so") + 0x12008;
                long a = (long) MemoryTools.getInstance().getMemory(add, 5);
                printIn(Long.toHexString(a));
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        });
    }

    public static void log(String msg) {
        Log.e(MainActivity.class.getName(), msg);
    }

    public void printIn(String a) {
        TextInputEditText b = (TextInputEditText) findViewById(R.id.edit1);
        b.setText(b.getText() + "\n" + a);
    }
}