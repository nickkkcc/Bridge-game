package ru.poly.bridgeandroid;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SwitchCompat;

import android.app.ActivityManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.AddFriendToClient;
import ru.poly.bridgeandroid.model.menu.AddFriendToServer;
import ru.poly.bridgeandroid.model.menu.DeleteFriendToClient;
import ru.poly.bridgeandroid.model.menu.DeleteFriendToServer;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToClient;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.menu.InvitePlayersToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.Player;
import ru.poly.bridgeandroid.model.menu.PlayersCountLobbyToClient;
import ru.poly.bridgeandroid.model.menu.PlayersOnlineToClient;
import ru.poly.bridgeandroid.model.menu.StartGamePlayersToClient;
import ru.poly.bridgeandroid.model.menu.StartGamePlayersToServer;

public class CreateGameActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private Gson gson;
    private String token;
    private String lobbyId;
    private List<Player> friends;
    private List<Player> players;
    private ListView listView;
    private TextView playersCountTextView;
    private int playersCount;
    private boolean isFriends;
    private boolean isStartGame;
    private String login;
    private SharedPreferences sharedPreferences;
    private Intent gameIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

        ActivityManager am = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            am = (ActivityManager)this.getSystemService(Context.ACTIVITY_SERVICE);
        }

        int sizeStack =  am.getRunningTasks(2).size();

        for (int i = 0; i < sizeStack; i++) {

            int numActivities = am.getRunningTasks(2).get(i).numActivities;
            Log.d("CreateGameActivity", String.valueOf(numActivities));
        }

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");
        login = sharedPreferences.getString(LOGIN, "");

        Intent myIntent = getIntent();
        friends = myIntent.getParcelableArrayListExtra("friends");
        players = myIntent.getParcelableArrayListExtra("players");
//        playersCount = myIntent.getIntExtra("playersCount", 0);
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
        playersCountTextView = findViewById(R.id.create_game_players_count);

        final SwitchCompat switchOnOff = findViewById(R.id.switch_list);
        final TextView tvSwitchFriends = findViewById(R.id.tvSwitchFriends);
        final TextView tvSwitchPlayers = findViewById(R.id.tvSwitchPlayers);

        updatePlayersCountTextView();

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
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String loginToInvite = isFriends ? friends.get(position).getLogin() :
                        players.get(position).getLogin();
                InvitePlayersToServer invitePlayers = new InvitePlayersToServer(lobbyId, loginToInvite);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(invitePlayers);
                Message message = new Message(token, "invite_players", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));

                String alias = isFriends ? friends.get(position).getAlias() :
                        players.get(position).getAlias();
                Toast toast = Toast.makeText(getBaseContext(), alias + " приглашён в лобби", Toast.LENGTH_SHORT);
                toast.show();
            }
        });

        listView.setLongClickable(true);
        listView.setOnItemLongClickListener((parent, view, position, id) -> {
            String loginLongClick = isFriends ? friends.get(position).getLogin() :
                    players.get(position).getLogin();
            String alias = isFriends ? friends.get(position).getAlias() :
                    players.get(position).getAlias();
            AlertDialog.Builder builder = new AlertDialog.Builder(this, R.style.CustomDialogTheme);
            if (isFriends) {
                builder.setTitle("Удаление из друзей");
                builder.setMessage("Удалить из друзей " + alias + "?");
                View viewDeleteFriend = LayoutInflater.from(this).inflate(R.layout.fragment_invitation,
                        findViewById(R.id.invite_layout));
                builder.setView(viewDeleteFriend);
                builder.setNegativeButton("Нет", (dialog, idNegative) -> {
                    // Ничего не делать
                });
                builder.setPositiveButton("Да", (dialog, idPositive) -> {
                    DeleteFriendToServer deleteFriend = new DeleteFriendToServer(loginLongClick);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(deleteFriend);
                    Message message = new Message(token, "delete_friend", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                });
                AlertDialog dialog = builder.create();
                dialog.setCanceledOnTouchOutside(false);
                dialog.show();
            } else {
                builder.setTitle("Добавление в друзья");
                builder.setMessage("Добавить в друзья " + alias + "?");
                View viewAddFriend = LayoutInflater.from(this).inflate(R.layout.fragment_invitation,
                        findViewById(R.id.invite_layout));
                builder.setView(viewAddFriend);
                builder.setNegativeButton("Нет", (dialog, idNegative) -> {
                    // Ничего не делать
                });
                builder.setPositiveButton("Да", (dialog, idPositive) -> {
                    AddFriendToServer addFriend = new AddFriendToServer(loginLongClick);
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(addFriend);
                    Message message = new Message(token, "add_friend", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                });
                AlertDialog dialog = builder.create();
                dialog.setCanceledOnTouchOutside(false);
                dialog.show();
            }
            return true;
        });

        switchOnOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isFriends = !switchOnOff.isChecked();
                updateListViewAdapter(isFriends);
                if (isFriends) {
                    tvSwitchFriends.setTextColor(getResources().getColor(R.color.menu_yellow));
                    tvSwitchPlayers.setTextColor(getResources().getColor(R.color.white));
                } else {
                    tvSwitchFriends.setTextColor(getResources().getColor(R.color.white));
                    tvSwitchPlayers.setTextColor(getResources().getColor(R.color.menu_yellow));
                }
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
                    isStartGame = true;
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
                break;
            case "exit_lobby":
                ExitLobbyToClient exitLobby = message.getData(ExitLobbyToClient.class);
                if (exitLobby.isSuccessful()) {
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.remove(LOBBY);
                    editor.apply();

                    Intent intent = new Intent(CreateGameActivity.this, MenuActivity.class);
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
                gameIntent = new Intent(CreateGameActivity.this, GameActivity.class);
                gameIntent.putExtra("gameState", gameState);
                if (!gameState.getPlayerTurn().equals(gameState.getPlayerPositionForLogin(login))) {
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
            case "add_friend":
                AddFriendToClient addFriend = message.getData(AddFriendToClient.class);
                if (addFriend.isSuccessful()) {
                    runOnUiThread(() -> Toast.makeText(getBaseContext(), "Добавлен новый друг", Toast.LENGTH_SHORT).show());
                } else {
                    runOnUiThread(() -> Toast.makeText(getBaseContext(), addFriend.getError(), Toast.LENGTH_SHORT).show());
                }
                break;
            case "delete_friend":
                DeleteFriendToClient deleteFriend = message.getData(DeleteFriendToClient.class);
                if (deleteFriend.isSuccessful()) {
                    runOnUiThread(() -> Toast.makeText(getBaseContext(), "Друг удалён", Toast.LENGTH_SHORT).show());
                } else {
                    runOnUiThread(() -> Toast.makeText(getBaseContext(), deleteFriend.getError(), Toast.LENGTH_SHORT).show());
                }
                break;
            default:
                throw new RuntimeException();
        }
    }

    private void updateListViewAdapter(boolean isFriends) {
        List<String> playersList = new ArrayList<>();
        for (Player player : isFriends ? friends : players) {
            playersList.add(player.getAlias());
        }
        ArrayAdapter<String> playersAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, playersList) {

            public View getView(int position, View convertView, ViewGroup parent) {
                View view = super.getView(position, convertView, parent);

                TextView textView = (TextView) view.findViewById(android.R.id.text1);

                /*YOUR CHOICE OF COLOR*/
                textView.setTextColor(getResources().getColor(R.color.tab_indicator_text));

                return view;
            }
        };
        runOnUiThread(() -> {
            listView.setAdapter(playersAdapter);
        });
    }

    private void updatePlayersCountTextView() {
        String playersCountString = "Игроков в лобби: " + playersCount;
        runOnUiThread(() -> playersCountTextView.setText(playersCountString));
    }
}