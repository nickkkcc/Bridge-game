package ru.poly.bridgeandroid;

import android.app.Application;
import android.content.SharedPreferences;
import android.util.Log;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import ru.poly.bridgeandroid.client.BackgroundManager;
import ru.poly.bridgeandroid.client.SocketConnection;

public class BridgeApplication extends Application {

    private static final String KEY = "token";
    private static final String PREFERENCE = "preference";
    private SocketConnection socketConnection;

    @Override
    public void onCreate() {
        super.onCreate();

        socketConnection = new SocketConnection(this);
        BackgroundManager.get(this).registerListener(appActivityListener);

        EventBus.getDefault().register(this);
    }

    @Override
    public void onTerminate() {
        super.onTerminate();

        EventBus.getDefault().unregister(this);
    }

    public void closeSocketConnection() {
        deleteToken();
        socketConnection.closeConnection();
    }

    public void openSocketConnection() {
        socketConnection.openConnection();
    }

    public boolean isSocketConnected() {
        return socketConnection.isConnected();
    }

    public void reconnect() {
        socketConnection.openConnection();
    }

    @Subscribe
    public void sendMessage(String message) {
        socketConnection.sendMessage(message);
        Log.i("WebSocket", "Sending message --> " + message);
    }

    private void deleteToken() {
        SharedPreferences sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(KEY, null);
        editor.apply();
        Log.i("Application", "Token was deleted");
    }

    private final BackgroundManager.Listener appActivityListener = new BackgroundManager.Listener() {
        public void onBecameForeground() {
            openSocketConnection();
            Log.i("Websocket", "Became Foreground");
        }

        public void onBecameBackground() {
            closeSocketConnection();
            Log.i("Websocket", "Became Background");
        }
    };
}
