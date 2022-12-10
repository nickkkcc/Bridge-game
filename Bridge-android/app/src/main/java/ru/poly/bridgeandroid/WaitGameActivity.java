package ru.poly.bridgeandroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import ru.poly.bridgeandroid.model.ExitLobbyToClient;
import ru.poly.bridgeandroid.model.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.StartGamePlayersToClient;

public class WaitGameActivity extends AppCompatActivity {

    private static final String KEY = "token";
    private static final String LOBBY = "lobby";
    private static final String PREFERENCE = "preference";
    private String token;
    private String lobbyId;
    private int playersCount;
    private Gson gson;
    private TextView playersCountTextView;
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wait_game);

        playersCountTextView = findViewById(R.id.create_game_players_count);
        final Button exitButton = findViewById(R.id.exit_to_menu_button);

        Intent myIntent = getIntent();
        playersCount = myIntent.getIntExtra("playersCount", 0);

        updatePlayersCountTextView();

        gson = new Gson();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(KEY, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");

        exitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ExitLobbyToServer exitGame = new ExitLobbyToServer(lobbyId, true);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(exitGame);
                Message message = new Message(token, "exit_lobby", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    @Subscribe
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "players_count_lobby":
                PlayersCountLobbyToClient playersCountLobby = message.getData(PlayersCountLobbyToClient.class);
                if (playersCountLobby.getError().equals("")) {
                    updatePlayersCountTextView();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), playersCountLobby.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "start_game_players":
                StartGamePlayersToClient startGame = message.getData(StartGamePlayersToClient.class);
                if (startGame.isSuccessful()) {
                    Intent intent = new Intent(WaitGameActivity.this, GameActivity.class);
                    startActivity(intent);
                    finish();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), startGame.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "exit_lobby":
                ExitLobbyToClient exitLobby = message.getData(ExitLobbyToClient.class);
                if (exitLobby.isSuccessful()) {
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.remove(LOBBY);
                    editor.apply();

                    Intent intent = new Intent(WaitGameActivity.this, MenuActivity.class);
                    startActivity(intent);
                    finish();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), exitLobby.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }

    private void updatePlayersCountTextView() {
        String playersCountString = "Ожидание игроков\n Игроков в лобби: " + playersCount;
        runOnUiThread(() -> playersCountTextView.setText(playersCountString));
    }
}