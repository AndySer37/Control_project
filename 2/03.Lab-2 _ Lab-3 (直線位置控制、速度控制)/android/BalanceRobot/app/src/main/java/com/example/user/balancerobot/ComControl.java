package com.example.user.balancerobot;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.content.Intent;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by user on 2017/2/6.
 */

public class ComControl extends AppCompatActivity {

    Button btnDis;

    //bluetooth
    public String address = null;
    public ProgressDialog progress;
    public BluetoothAdapter myBluetooth = null;
    public BluetoothSocket btSocket = null;
    public boolean isBtConnected = false;
    public static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");//UUID:00001101-0000-1000-8000-00805F9B34FB
    //bluetooth-end

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_com_control);

        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS);

        ConnectBT bt = new ConnectBT();//user defined class
        bt.execute();

        GoToComControl();//user defined function
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>
    {
        private boolean ConnectSuccess = true;
        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(ComControl.this, "COnnecting...", "Please wait!!");
        }
        @Override
        protected Void doInBackground(Void... devices)
        {
            try
            {
                if(btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result)
        {
            super.onPostExecute(result);
            if(!ConnectSuccess)
            {
                msg("Connect Failed. Is it a SPP Bluetooth? Try again.");//user defined function
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }

    public void GoToComControl()
    {
        setContentView(R.layout.activity_com_control);//設定對應的layout

        btnDis = (Button)findViewById(R.id.button2);

        btnDis.setOnClickListener(new View.OnClickListener()//button的listener
        {
            @Override
            public void onClick(View v)
            {
                Disconnect();
            }
        });
    }

    private void Disconnect()
    {
        if(btSocket!=null)
        {
            try
            {
                btSocket.close();
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
        finish();//結束藍牙
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }
}
