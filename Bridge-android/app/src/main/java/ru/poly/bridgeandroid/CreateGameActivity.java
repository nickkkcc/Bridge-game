package ru.poly.bridgeandroid;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.github.angads25.toggle.interfaces.OnToggledListener;
import com.github.angads25.toggle.model.ToggleableView;
import com.github.angads25.toggle.widget.LabeledSwitch;
import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import ru.poly.bridgeandroid.model.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.InvitePlayersToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.Player;
import ru.poly.bridgeandroid.model.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.PlayersOnlineToClient;
import ru.poly.bridgeandroid.model.StartGamePlayersToClient;
import ru.poly.bridgeandroid.model.StartGamePlayersToServer;

public class CreateGameActivity extends AppCompatActivity {

    private static final String KEY = "token";
    private static final String LOBBY = "lobby";
    private static final String PREFERENCE = "preference";
    private Gson gson;
    private List<Player> friends;
    private List<Player> players;
    private ListView listView;
    private TextView playersCountTextView;
    private int playersCount;
    private boolean isFriends;
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        String token = sharedPreferences.getString(KEY, "");
        String lobbyId = sharedPreferences.getString(LOBBY, "");

        Intent myIntent = getIntent();
        friends = myIntent.getParcelableArrayListExtra("friends");
        players = myIntent.getParcelableArrayListExtra("players");
//        players = Arrays.asList(new Player("login1","player1"), new Player("login1","player2"),
//                new Player("login3","player3"), new Player("login4","player4"),
//                new Player("login5","player5"), new Player("login6","player6"),
//                new Player("login7","player7"), new Player("login8","player8"),
//                new Player("login9","player9"), new Player("login10","player10"));
//        friends = Arrays.asList(new Player("login1","friend1"), new Player("login1","friend2"),
//                new Player("login3","friend3"), new Player("login4","friend4"),
//                new Player("login5","friend5"), new Player("login6","friend6"),
//                new Player("login7","friend7"), new Player("login8","friend8"),
//                new Player("login9","friend9"), new Player("login10","friend10"));

        gson = new Gson();

        listView = findViewById(R.id.listView);
        final Button startGameButton = findViewById(R.id.start_game);
        final Button exitGameButton = findViewById(R.id.exit_lobby);
        final LabeledSwitch labeledSwitch = findViewById(R.id.switch_list);
        playersCountTextView = findViewById(R.id.textView_players_count);

        isFriends = false;
        updateListViewAdapter(false);

        startGameButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                StartGamePlayersToServer startGame = new StartGamePlayersToServer(lobbyId, true);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(startGame);
                Message message = new Message(token, "start_game_players", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        exitGameButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ExitLobbyToServer exitGame = new ExitLobbyToServer(lobbyId, true);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(exitGame);
                Message message = new Message(token, "exit_lobby", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));

                SharedPreferences.Editor editor = sharedPreferences.edit();
                editor.remove(LOBBY);
                editor.apply();

                Intent intent = new Intent(CreateGameActivity.this, MenuActivity.class);
                startActivity(intent);
                finish();
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String login = isFriends ? friends.get(position).getLogin() :
                        players.get(position).getLogin();
                InvitePlayersToServer invitePlayers = new InvitePlayersToServer(login);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(invitePlayers);
                Message message = new Message(token, "invite_players", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));

                String alias = isFriends ? friends.get(position).getAlias() :
                        players.get(position).getAlias();
                Toast toast = Toast.makeText(getBaseContext(), alias + " пришлашён в лобби", Toast.LENGTH_SHORT);
                toast.show();
            }
        });

        labeledSwitch.setOnToggledListener(new OnToggledListener() {
            @Override
            public void onSwitched(ToggleableView toggleableView, boolean isOn) {
                isFriends = isOn;
                updateListViewAdapter(isFriends);
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
                    playersCount = playersCountLobby.getPlayersInLobby();
                    updatePlayersCountTextView();
                    break;
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
                    Intent intent = new Intent(CreateGameActivity.this, GameActivity.class);
                    startActivity(intent);
                    finish();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), startGame.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "players_online":
                PlayersOnlineToClient playersOnline = message.getData(PlayersOnlineToClient.class);
                friends = playersOnline.getFriends();
                players = playersOnline.getPlayers();
                updateListViewAdapter(isFriends);
            default:
                throw new RuntimeException();
        }
    }

    private void updateListViewAdapter(boolean isFriends) {
        List<String> playersList = new ArrayList<>();
        for (Player player : isFriends ? friends : players) {
            playersList.add(player.getAlias());
        }
        ArrayAdapter<String> playersAdapter = new ArrayAdapter<>(this,
                android.R.layout.simple_list_item_1, playersList);
        runOnUiThread(() -> {
            listView.setAdapter(playersAdapter);
        });
    }

    private void updatePlayersCountTextView() {
        String playersCountString = "Игроков в лобби: " + playersCount;
        runOnUiThread(() -> playersCountTextView.setText(playersCountString));
    }
}