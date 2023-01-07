package ru.poly.bridgeandroid;

import android.app.ActivityManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import ru.poly.bridgeandroid.enums.GameEvent;
import ru.poly.bridgeandroid.enums.GamePhase;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.AcceptInvitePlayersToServer;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToClient;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.menu.StartGamePlayersToClient;

public class WaitGameActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private String token;
    private String lobbyId;
    private String login;
    private int playersCount;
    private boolean isStartGame;
    private Gson gson;
    private TextView playersCountTextView;
    private SharedPreferences sharedPreferences;
    private Intent gameIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wait_game);

        ActivityManager am = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            am = (ActivityManager)this.getSystemService(Context.ACTIVITY_SERVICE);
        }

        int sizeStack =  am.getRunningTasks(2).size();

        for (int i = 0; i < sizeStack; i++) {

            int numActivities = am.getRunningTasks(2).get(i).numActivities;
            Log.d("WaitGameActivity", String.valueOf(numActivities));
        }

        playersCountTextView = findViewById(R.id.create_game_players_count);
        final Button exitButton = findViewById(R.id.exit_to_menu_button);

        Intent myIntent = getIntent();
        playersCount = myIntent.getIntExtra("playersCount", 0);

        updatePlayersCountTextView();

        gson = new Gson();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");
        login = sharedPreferences.getString(LOGIN, "");

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

    @Override
    public void onBackPressed() {
        runOnUiThread(() -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(this, R.style.CustomDialogTheme);
            builder.setTitle("Выйти из лобби");
            builder.setMessage("Вы уверены, что хотите выйти из лобби?");
            View view = LayoutInflater.from(this).inflate(R.layout.fragment_invitation, findViewById(R.id.invite_layout));
            builder.setView(view);
            builder.setNegativeButton("Нет", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    // Ничего не делать
                }
            });
            builder.setPositiveButton("Да", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    ExitLobbyToServer exitGame = new ExitLobbyToServer(lobbyId, true);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(exitGame);
                    Message message = new Message(token, "exit_lobby", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
            });
            AlertDialog dialog = builder.create();
            dialog.setCanceledOnTouchOutside(false);
            dialog.show();
        });
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "players_count_lobby":
                PlayersCountLobbyToClient playersCountLobby = message.getData(PlayersCountLobbyToClient.class);
                if (playersCountLobby.getError().equals("")) {
                    playersCount = playersCountLobby.getPlayersInLobby();
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
                    isStartGame = true;
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
            case "update_game_state":
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "update_game_state error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                PlayerGameState gameState = updateGameState.getGameState();
                gameIntent = new Intent(WaitGameActivity.this, GameActivity.class);
                gameIntent.putExtra("gameState", gameState);
                if (!gameState.getPlayerTurn().equals(gameState.getPlayerPositionForLogin(login)) &&
                        !gameState.getGameEvent().equals(GameEvent.INITIALIZE)) {
                    startActivity(gameIntent);
                    finish();
                }
                break;
            case "notify_bid_turn":
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "notify_bid_turn error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                gameIntent.putExtra("notifyBidTurn", true);
                startActivity(gameIntent);
                finish();
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