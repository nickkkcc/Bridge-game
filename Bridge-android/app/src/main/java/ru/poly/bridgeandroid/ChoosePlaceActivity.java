package ru.poly.bridgeandroid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import ru.poly.bridgeandroid.model.AcceptSelectTeamToClient;
import ru.poly.bridgeandroid.model.CreateLobbyToClient;
import ru.poly.bridgeandroid.model.LoginToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.PlayersOnlineToClient;
import ru.poly.bridgeandroid.model.RegistrationQuestionsToClient;
import ru.poly.bridgeandroid.model.SelectTeamToClient;
import ru.poly.bridgeandroid.model.SelectTeamToServer;
import ru.poly.bridgeandroid.ui.login.LoginActivity;
import ru.poly.bridgeandroid.ui.login.RegistrationActivity;

public class ChoosePlaceActivity extends AppCompatActivity {

    private static final String KEY = "token";
    private static final String LOBBY = "lobby";
    private static final String PREFERENCE = "preference";
    private Gson gson;
    private boolean firstTeam;
    private boolean secondTeam;
    private boolean isAdmin;
    private Button firstTeamButton;
    private Button secondTeamButton;
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_place);

        firstTeamButton = findViewById(R.id.NS);
        secondTeamButton = findViewById(R.id.EW);

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        String token = sharedPreferences.getString(KEY, "");
        String lobbyId = sharedPreferences.getString(LOBBY, "");

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
                SelectTeamToServer selectTeam = new SelectTeamToServer(lobbyId, 1);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                Message message = new Message(token, "select_team", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });
        secondTeamButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SelectTeamToServer selectTeam = new SelectTeamToServer(lobbyId, 2);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(selectTeam);
                Message message = new Message(token, "select_team", jsonObject);
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
                Intent intent = new Intent(ChoosePlaceActivity.this, CreateGameActivity.class);
                intent.putParcelableArrayListExtra("friends", playersOnline.getFriends());
                intent.putParcelableArrayListExtra("players", playersOnline.getPlayers());
                startActivity(intent);
                finish();
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
                    Intent intentToWaiting = new Intent(ChoosePlaceActivity.this, CreateGameActivity.class);
                    intentToWaiting.putExtra("playersCount", playersCountLobby.getPlayersInLobby());
                    startActivity(intentToWaiting);
                    finish();
                    break;
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), playersCountLobby.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}