package ru.poly.bridgeandroid;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;

import ru.poly.bridgeandroid.model.menu.AcceptSelectTeamToClient;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToClient;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.menu.Player;
import ru.poly.bridgeandroid.model.menu.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.menu.PlayersOnlineToClient;
import ru.poly.bridgeandroid.model.menu.SelectTeamAdminToServer;
import ru.poly.bridgeandroid.model.menu.SelectTeamToClient;
import ru.poly.bridgeandroid.model.menu.SelectTeamToServer;

public class ChoosePlaceActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String PREFERENCE = "preference";
    private Gson gson;
    private String token;
    private String lobbyId;
    private boolean firstTeam;
    private boolean secondTeam;
    private boolean isAdmin;
    private boolean isStartedNewActivity;
    private Button firstTeamButton;
    private Button secondTeamButton;
    private ArrayList<Player> players;
    private ArrayList<Player> friends;
    private Integer playersCount;
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_place);

        firstTeamButton = findViewById(R.id.NS);
        secondTeamButton = findViewById(R.id.EW);

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");

        Intent myIntent = getIntent();
        firstTeam = myIntent.getBooleanExtra("firstTeam", true);
        secondTeam = myIntent.getBooleanExtra("secondTeam", true);
        isAdmin = myIntent.getBooleanExtra("isAdmin", true);

        if (!firstTeam) {
            firstTeamButton.setVisibility(View.INVISIBLE);
        }
        if (!secondTeam) {
            secondTeamButton.setVisibility(View.INVISIBLE);
        }

        gson = new Gson();

        firstTeamButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isAdmin) {
                    SelectTeamAdminToServer selectTeam = new SelectTeamAdminToServer(lobbyId, 0);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                    Message message = new Message(token, "select_team_admin", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                } else {
                    SelectTeamToServer selectTeam = new SelectTeamToServer(lobbyId, 0);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                    Message message = new Message(token, "select_team", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
            }
        });
        secondTeamButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isAdmin) {
                    SelectTeamAdminToServer selectTeam = new SelectTeamAdminToServer(lobbyId, 1);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                    Message message = new Message(token, "select_team_admin", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                } else {
                    SelectTeamToServer selectTeam = new SelectTeamToServer(lobbyId, 1);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                    Message message = new Message(token, "select_team", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
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
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.remove(LOBBY);
        editor.apply();

        Intent intent = new Intent(ChoosePlaceActivity.this, MenuActivity.class);
        startActivity(intent);
        finish();
    }

    @Subscribe
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "accept_select_team":
                AcceptSelectTeamToClient acceptSelectTeam = message.getData(AcceptSelectTeamToClient.class);
                if (acceptSelectTeam.isSuccessful()) {
                    if (!isAdmin) {
                        // TODO: обсудить что будет при валидном случае
                    }
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), acceptSelectTeam.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "players_online":
                PlayersOnlineToClient playersOnline = message.getData(PlayersOnlineToClient.class);
                friends = playersOnline.getFriends();
                players = playersOnline.getPlayers();
                if (playersCount != null && !isStartedNewActivity) {
                    isStartedNewActivity = true;
                    Intent intent = new Intent(ChoosePlaceActivity.this, CreateGameActivity.class);
                    intent.putParcelableArrayListExtra("friends", friends);
                    intent.putParcelableArrayListExtra("players", players);
                    intent.putExtra("playersCount", playersCount);
                    startActivity(intent);
                    finish();
                }
                break;
            case "select_team":
                SelectTeamToClient selectTeam = message.getData(SelectTeamToClient.class);
                if (selectTeam.getFirstTeam() == 2 && selectTeam.getSecondTeam() == 2) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "Все команды уже заняты", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                    // TODO: как обрабатывать логику
                }

                firstTeam = selectTeam.getFirstTeam() < 2;
                secondTeam = selectTeam.getSecondTeam() < 2;

                runOnUiThread(() -> {
                    firstTeamButton.setVisibility(firstTeam ? View.VISIBLE : View.INVISIBLE);
                    secondTeamButton.setVisibility(secondTeam ? View.VISIBLE : View.INVISIBLE);
                });
                break;
            case "players_count_lobby":
                PlayersCountLobbyToClient playersCountLobby = message.getData(PlayersCountLobbyToClient.class);
                if (playersCountLobby.getError().equals("")) {
                    playersCount = playersCountLobby.getPlayersInLobby();
                    if (!isAdmin && !isStartedNewActivity) {
                        isStartedNewActivity = true;
                        Intent intent = new Intent(ChoosePlaceActivity.this, WaitGameActivity.class);
                        intent.putExtra("playersCount", playersCount);
                        startActivity(intent);
                        finish();
                    }
                    if (friends != null && players != null && !isStartedNewActivity) {
                        isStartedNewActivity = true;
                        Intent intent = new Intent(ChoosePlaceActivity.this, CreateGameActivity.class);
                        intent.putParcelableArrayListExtra("friends", friends);
                        intent.putParcelableArrayListExtra("players", players);
                        intent.putExtra("playersCount", playersCount);
                        startActivity(intent);
                        finish();
                    }
                    break;
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), playersCountLobby.getError(), Toast.LENGTH_SHORT);
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

                    Intent intent = new Intent(ChoosePlaceActivity.this, MenuActivity.class);
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
}