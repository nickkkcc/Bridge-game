package ru.poly.bridgeandroid;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import ru.poly.bridgeandroid.model.AcceptInvitePlayersToClient;
import ru.poly.bridgeandroid.model.AcceptInvitePlayersToServer;
import ru.poly.bridgeandroid.model.CreateLobbyToClient;
import ru.poly.bridgeandroid.model.ExitLobbyToServer;
import ru.poly.bridgeandroid.model.InvitePlayersToClient;
import ru.poly.bridgeandroid.model.JoinToServer;
import ru.poly.bridgeandroid.model.LoginToClient;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.RegistrationQuestionsToClient;
import ru.poly.bridgeandroid.model.SelectTeamToClient;
import ru.poly.bridgeandroid.model.SelectTeamToServer;
import ru.poly.bridgeandroid.ui.login.LoginActivity;
import ru.poly.bridgeandroid.ui.login.RegistrationActivity;

public class MenuActivity extends AppCompatActivity {

    private static final String KEY = "token";
    private static final String LOBBY = "lobby";
    private static final String PREFERENCE = "preference";
    private String token;
    private Gson gson;
    private TextView loadingTextView;
    private ProgressBar loadingProgressBar;
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        final Button createButton = findViewById(R.id.create_button);
        final Button joinButton = findViewById(R.id.join_button);
        final Button exitButton = findViewById(R.id.exit_account_button);
        loadingTextView = findViewById(R.id.joining_text);
        loadingProgressBar = findViewById(R.id.loading_menu);

        gson = new Gson();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(KEY, "");

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
                boolean readyToJoin;
                if (loadingTextView.getVisibility() == View.VISIBLE) {
                    //loadingTextView.setVisibility(View.INVISIBLE);
                    loadingProgressBar.setVisibility(View.INVISIBLE);
                    joinButton.setText("Присоединиться");
                    readyToJoin = false;
                } else {
                    //loadingTextView.setVisibility(View.VISIBLE);
                    loadingProgressBar.setVisibility(View.VISIBLE);
                    joinButton.setText("Отменить поиск");
                    readyToJoin = true;
                    Toast.makeText(getBaseContext(), "Ожидание приглашения", Toast.LENGTH_LONG)
                            .show();
                }

                JoinToServer join = new JoinToServer(readyToJoin);
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(join);
                Message message = new Message(token, "join", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
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

    @Subscribe
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
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Игрок " + invitePlayers.getAlias() + " отправил Вам приглашение.");
                    builder.setMessage("Присоединиться в лобби?");
                    builder.setNegativeButton("Нет", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            AcceptInvitePlayersToServer acceptInvite =
                                    new AcceptInvitePlayersToServer(invitePlayers.getLobbyId(), false);
                            JsonObject jsonObject = (JsonObject) gson.toJsonTree(acceptInvite);
                            Message message = new Message(token, "accept_invite_players", jsonObject);
                            EventBus.getDefault().post(gson.toJson(message));
                        }
                    });
                    builder.setPositiveButton("Да", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            AcceptInvitePlayersToServer acceptInvite =
                                    new AcceptInvitePlayersToServer(invitePlayers.getLobbyId(), true);
                            JsonObject jsonObject = (JsonObject) gson.toJsonTree(acceptInvite);
                            Message message = new Message(token, "accept_invite_players", jsonObject);
                            EventBus.getDefault().post(gson.toJson(message));
                        }
                    });
                    AlertDialog dialog = builder.create();
                    dialog.show();
                });
                break;
            case "accept_invite_players":
                AcceptInvitePlayersToClient acceptInvite = message.getData(AcceptInvitePlayersToClient.class);
                if (acceptInvite.isSuccessful()) {
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
            default:
                throw new RuntimeException();
        }
    }
}