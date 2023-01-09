package ru.poly.bridgeandroid;

import android.app.Application;
import android.content.SharedPreferences;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import ru.poly.bridgeandroid.client.BackgroundManager;
import ru.poly.bridgeandroid.client.SocketConnection;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.LoginToServer;

public class BridgeApplication extends Application {

    private static final String KEY = "token";
    private static final String LOGIN = "login";
    private static final String PASSWORD = "password";
    private static final String RESTART = "restart";
    private static final String RESTART_GAME = "restartGame";
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

            while (!isSocketConnected()) {
                // Ничего не делать
            }

            SharedPreferences sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
            boolean isRestart = sharedPreferences.getBoolean(RESTART, false);
            boolean isRestartGame = sharedPreferences.getBoolean(RESTART_GAME, false);

            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putBoolean(RESTART, false);
            editor.putBoolean(RESTART_GAME, false);
            editor.apply();

            if (isRestart || isRestartGame) {
                Gson gson = new Gson();
                String login = sharedPreferences.getString(LOGIN, "");
                String password = sharedPreferences.getString(PASSWORD, "");

                LoginToServer loginToServer = new LoginToServer(login, password);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(loginToServer);
                Message message = new Message("0", "login", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        }

        public void onBecameBackground() {
            closeSocketConnection();
            Log.i("Websocket", "Became Background");
        }
    };
}