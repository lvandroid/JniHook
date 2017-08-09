package com.bsty.jnihook;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private ElfHooker elfHooker;
    Executor executor = Executors.newSingleThreadExecutor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method

        Button hookBtn = (Button) findViewById(R.id.hook);
        hookBtn.setOnClickListener(this);
        Button httpBtn = (Button) findViewById(R.id.http);
        httpBtn.setOnClickListener(this);
        Button socketBtn = (Button) findViewById(R.id.socket);
        socketBtn.setOnClickListener(this);

        elfHooker = new ElfHooker();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.hook:
                elfHooker.setHook();
                break;
            case R.id.http:
                executor.execute(new Runnable() {
                    @Override
                    public void run() {
                        NetUtils.okhttpRequest();
//                        NetUtils.okhttpAsyncRequest();
                    }
                });
                break;
            case R.id.socket:
                executor.execute(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Socket socket = new Socket("www.baidu.com", 56545);
                            socket.isConnected();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
            default:
                break;
        }
    }
}
