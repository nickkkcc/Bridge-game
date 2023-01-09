package ru.poly.bridgeandroid;

import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.gson.Gson;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.List;

import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.AddFriendToClient;
import ru.poly.bridgeandroid.model.menu.HistoryElement;
import ru.poly.bridgeandroid.model.menu.RequestScoreToClient;
import ru.poly.bridgeandroid.ui.login.LoginActivity;

public class HistoryActivity extends AppCompatActivity {

    private RequestScoreToClient requestScore;
    private List<HistoryElement> historyList;

    private static final String RESTART = "restart";
    private static final String PREFERENCE = "preference";
    private SharedPreferences sharedPreferences;

    private boolean isInactive;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_history);

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);

        final TextView allGames = findViewById(R.id.history_all_games);
        final TextView winGames = findViewById(R.id.history_win_games);
        final TextView winRate = findViewById(R.id.history_win_rate);
        final RecyclerView recyclerView = findViewById(R.id.history_recycler_view);

        Intent myIntent = getIntent();
        requestScore = myIntent.getParcelableExtra("score");
        historyList = myIntent.getParcelableArrayListExtra("historyList");

        allGames.setText(String.valueOf(requestScore.getAllGamesCount()));
        winGames.setText(String.valueOf(requestScore.getWinGamesCount()));
        winRate.setText(String.valueOf(requestScore.getWinRate()));

        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);

        recyclerView.setHasFixedSize(true);
        HistoryActivityAdapter historyActivityAdapter = new HistoryActivityAdapter(historyList, this);
        recyclerView.setAdapter(historyActivityAdapter);
    }

    @Override
    public void onStart() {
        super.onStart();
        if (isInactive) {
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putBoolean(RESTART, true);
            editor.apply();

            Intent intent = new Intent(HistoryActivity.this, LoginActivity.class);
            startActivity(intent);
            finishAffinity();
        } else {
            EventBus.getDefault().register(this);
        }
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        isInactive = true;
        super.onStop();
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "add_friend":
                AddFriendToClient addFriend = message.getData(AddFriendToClient.class);
                if (addFriend.isSuccessful()) {
                    Toast.makeText(getBaseContext(), "Добавлен новый друг", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(getBaseContext(), addFriend.getError(), Toast.LENGTH_SHORT).show();
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}