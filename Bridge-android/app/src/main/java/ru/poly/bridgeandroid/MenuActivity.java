package ru.poly.bridgeandroid;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import ru.poly.bridgeandroid.enums.GameEvent;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.AcceptInvitePlayersToClient;
import ru.poly.bridgeandroid.model.menu.AcceptInvitePlayersToServer;
import ru.poly.bridgeandroid.model.menu.CreateLobbyToClient;
import ru.poly.bridgeandroid.model.menu.InvitePlayersToClient;
import ru.poly.bridgeandroid.model.menu.JoinToClient;
import ru.poly.bridgeandroid.model.menu.JoinToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.RequestHistoryToClient;
import ru.poly.bridgeandroid.model.menu.RequestScoreToClient;
import ru.poly.bridgeandroid.model.menu.SelectTeamToClient;
import ru.poly.bridgeandroid.ui.login.LoginActivity;

public class MenuActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private String token;
    private String login;
    private boolean isAccepted;
    private boolean readyToJoin;
    private boolean isStartGame;
    private Gson gson;
    private ProgressBar loadingProgressBar;
    private SharedPreferences sharedPreferences;
    private Intent gameIntent;
    private Intent historyIntent;

    private long backPressedTime;
    private Toast backToast;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        ActivityManager am = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            am = (ActivityManager)this.getSystemService(Context.ACTIVITY_SERVICE);
        }

        int sizeStack =  am.getRunningTasks(2).size();

        for (int i = 0; i < sizeStack; i++) {

            int numActivities = am.getRunningTasks(2).get(i).numActivities;
            Log.d("MenuActivity", String.valueOf(numActivities));
        }

        final Button createButton = findViewById(R.id.create_button);
        final Button joinButton = findViewById(R.id.join_button);
        final Button previousGames = findViewById(R.id.previous_games);
        final Button exitButton = findViewById(R.id.exit_account_button);
        loadingProgressBar = findViewById(R.id.loading_menu);

        gson = new Gson();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        login = sharedPreferences.getString(LOGIN, "");

        createButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Message message = new Message(token, "create_lobby");
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        joinButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (readyToJoin) {
                    joinButton.setText("Присоединиться");
                    readyToJoin = false;
                } else {
                    joinButton.setText("Отменить поиск");
                    readyToJoin = true;
                }

                JoinToServer join = new JoinToServer(readyToJoin);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(join);
                Message message = new Message(token, "join", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        previousGames.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Message messageScore = new Message(token, "request_score");
                EventBus.getDefault().post(gson.toJson(messageScore));

                Message messageHistory = new Message(token, "request_history_list");
                EventBus.getDefault().post(gson.toJson(messageHistory));
            }
        });

        exitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MenuActivity.this, LoginActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
                finish();
                Runtime.getRuntime().exit(0);
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
        if (backPressedTime + 2000 > System.currentTimeMillis()) {
            backToast.cancel();
            super.onBackPressed();
            return;
        } else {
            backToast = Toast.makeText(getBaseContext(), "Нажмите ещё раз, чтобы выйти", Toast.LENGTH_SHORT);
            backToast.show();
        }
        backPressedTime = System.currentTimeMillis();
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "create_lobby":
                CreateLobbyToClient createLobby = message.getData(CreateLobbyToClient.class);
                if (createLobby.isSuccessful()) {
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString(LOBBY, createLobby.getLobbyId());
                    editor.apply();

                    Intent intent = new Intent(MenuActivity.this, ChoosePlaceActivity.class);
                    intent.putExtra("team1", true);
                    intent.putExtra("team2", true);
                    intent.putExtra("isAdmin", true);
                    startActivity(intent);
                    finish();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), createLobby.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "invite_players":
                InvitePlayersToClient invitePlayers = message.getData(InvitePlayersToClient.class);

                runOnUiThread(() -> {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this, R.style.CustomDialogTheme);
                    builder.setTitle("Игрок " + invitePlayers.getAlias() + " отправил Вам приглашение.");
                    builder.setMessage("Присоединиться в лобби?");
                    View view = LayoutInflater.from(this).inflate(R.layout.fragment_invitation, findViewById(R.id.invite_layout));
                    builder.setView(view);
                    builder.setNegativeButton("Нет", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            isAccepted = false;
                            AcceptInvitePlayersToServer acceptInvite =
                                    new AcceptInvitePlayersToServer(invitePlayers.getLobbyId(), isAccepted);
                            JsonObject jsonObject = (JsonObject) gson.toJsonTree(acceptInvite);
                            Message message = new Message(token, "accept_invite_players", jsonObject);
                            EventBus.getDefault().post(gson.toJson(message));
                        }
                    });
                    builder.setPositiveButton("Да", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            isAccepted = true;
                            AcceptInvitePlayersToServer acceptInvite =
                                    new AcceptInvitePlayersToServer(invitePlayers.getLobbyId(), isAccepted);
                            JsonObject jsonObject = (JsonObject) gson.toJsonTree(acceptInvite);
                            Message message = new Message(token, "accept_invite_players", jsonObject);
                            EventBus.getDefault().post(gson.toJson(message));
                        }
                    });
                    AlertDialog dialog = builder.create();
                    dialog.setCanceledOnTouchOutside(false);
                    dialog.show();
                });
                break;
            case "accept_invite_players":
                AcceptInvitePlayersToClient acceptInvite = message.getData(AcceptInvitePlayersToClient.class);
                if (acceptInvite.isSuccessful() && isAccepted) {
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString(LOBBY, acceptInvite.getLobbyId());
                    editor.apply();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), acceptInvite.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "select_team":
                SelectTeamToClient selectTeam = message.getData(SelectTeamToClient.class);
                if (selectTeam.getFirstTeam() == 2 && selectTeam.getSecondTeam() == 2) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "Все команды уже заняты", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                    return;
                    // TODO: непонятно что делать, если изначально все места заняты
                }

                Intent intent = new Intent(MenuActivity.this, ChoosePlaceActivity.class);
                intent.putExtra("team1", selectTeam.getFirstTeam() != 2);
                intent.putExtra("team2", selectTeam.getSecondTeam() != 2);
                intent.putExtra("isAdmin", false);
                startActivity(intent);
                finish();
                break;
            case "join":
                JoinToClient join = message.getData(JoinToClient.class);
                if (join.isSuccessful()) {
                    runOnUiThread(() -> Toast.makeText(getBaseContext(), "Ожидание приглашения", Toast.LENGTH_LONG)
                            .show());
                } else {
                    readyToJoin = !readyToJoin;
                    runOnUiThread(() -> {
                        String error;
                        if (readyToJoin) {
                            error = "Не удалось отменить поиск, попробуйте ещё раз.";
                        } else {
                            error = "Не удалось начать поиск, попробуйте ещё раз.";
                        }
                        Toast toast = Toast.makeText(getBaseContext(), error, Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                switchLoadingVisibility();
                break;
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                PlayerGameState gameState = updateGameState.getGameState();
                if (gameState.getGameEvent().equals(GameEvent.PLAY_CONTINUES)) {
                    isStartGame = true;
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "Игра продолжается.", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                    return;
                }
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "update_game_state error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                gameIntent = new Intent(MenuActivity.this, GameActivity.class);
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
            case "request_score":
                RequestScoreToClient requestScore = message.getData(RequestScoreToClient.class);
                historyIntent = new Intent(MenuActivity.this, HistoryActivity.class);
                historyIntent.putExtra("score", requestScore);
                break;
            case "request_history_list":
                RequestHistoryToClient requestHistory = message.getData(RequestHistoryToClient.class);
                historyIntent.putParcelableArrayListExtra("historyList", requestHistory.getHistoryList());
                startActivity(historyIntent);
                break;
            default:
                throw new RuntimeException();
        }
    }

    private void switchLoadingVisibility() {
        if (readyToJoin) {
            runOnUiThread(() -> {
                loadingProgressBar.setVisibility(View.VISIBLE);
            });
        } else {
            runOnUiThread(() -> {
                loadingProgressBar.setVisibility(View.INVISIBLE);
            });
        }
    }
}