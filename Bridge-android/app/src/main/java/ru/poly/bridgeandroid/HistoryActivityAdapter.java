package ru.poly.bridgeandroid;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.core.content.res.ResourcesCompat;
import androidx.recyclerview.widget.RecyclerView;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TimeZone;

import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.AddFriendToServer;
import ru.poly.bridgeandroid.model.menu.HistoryElement;

import static android.content.Context.MODE_PRIVATE;

public class HistoryActivityAdapter extends RecyclerView.Adapter<HistoryActivityAdapter.HistoryActivityViewHolder> {

    private static final String TOKEN = "token";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private String token;
    private String login;

    private Context context;
    private Activity activity;
    private List<HistoryElement> historyList;

    private Gson gson;
    private SharedPreferences sharedPreferences;

    private SimpleDateFormat inputDateFormat;
    private SimpleDateFormat outputDateFormat;

    HistoryActivityAdapter(List<HistoryElement> historyList, Context context) {
        this.historyList = historyList;
        this.context = context;
        this.activity = (Activity) context;

        gson = new Gson();

        sharedPreferences = context.getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        login = sharedPreferences.getString(LOGIN, "");

        inputDateFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssX", Locale.ENGLISH);
        inputDateFormat.setTimeZone(TimeZone.getTimeZone("UTC"));

        outputDateFormat = new SimpleDateFormat("HH:mm dd.MM.yyyy", Locale.ROOT);
        outputDateFormat.setTimeZone(TimeZone.getTimeZone("Europe/Moscow"));
    }

    @NonNull
    @Override
    public HistoryActivityViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(parent.getContext());

        View view = inflater.inflate(R.layout.history_element, parent, false);

        return new HistoryActivityViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull final HistoryActivityViewHolder holder, final int position) {
        HistoryElement historyElement = historyList.get(position);

        String startTime = "";
        String endTime = "";
        try {
            Date startDate = inputDateFormat.parse(historyElement.getGameStartTime());
            Date endDate = inputDateFormat.parse(historyElement.getGameEndTime());
            startTime = outputDateFormat.format(startDate);
            endTime = outputDateFormat.format(endDate);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        String time = startTime + " - " + endTime;
        holder.time.setText(time);

        String owner;
        if (historyElement.getOwnerLogin().equals(login)) {
            owner = "Создатель лобби: Вы";
        } else {
            owner = "Создатель лобби: " + historyElement.getOwnerAlias();
        }
        Spannable spannableOwner = new SpannableString(owner);
//        spannableOwner.setSpan(new ForegroundColorSpan(ResourcesCompat.getColor(context.getResources(),
//                R.color.yellow, null)), 17, owner.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        holder.lobbyOwner.setText(spannableOwner, TextView.BufferType.SPANNABLE);

        String winnerTeam;
        if (historyElement.getWinnerTeam() == 0) {
            winnerTeam = "Команда-победитель: Север-Юг";
        } else {
            winnerTeam = "Команда-победитель: Запад-Восток";
        }
        Spannable spannableWinnerTeam = new SpannableString(winnerTeam);
//        spannableWinnerTeam.setSpan(new ForegroundColorSpan(ResourcesCompat.getColor(context.getResources(),
//                R.color.yellow, null)), 18, winnerTeam.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        holder.winnerTeam.setText(spannableWinnerTeam, TextView.BufferType.SPANNABLE);

        String team1 = "Север-Юг: " + historyElement.getTotalScoreNorthSouth();
        Spannable spannableNorthSouth = new SpannableString(team1);
        spannableNorthSouth.setSpan(new ForegroundColorSpan(ResourcesCompat.getColor(context.getResources(),
                R.color.yellow, null)), 10, team1.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        holder.team1.setText(spannableNorthSouth, TextView.BufferType.SPANNABLE);

        String team2 = "Запад-Восток: " + historyElement.getTotalScoreEastWest();
        Spannable spannableEastWest = new SpannableString(team2);
        spannableEastWest.setSpan(new ForegroundColorSpan(ResourcesCompat.getColor(context.getResources(),
                R.color.yellow, null)), 14, team2.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        holder.team2.setText(spannableEastWest, TextView.BufferType.SPANNABLE);

        Map<String, Button> loginButtonsMap = new HashMap<String, Button>() {{
            put(historyElement.getPlayerNorthLogin(), holder.northButton);
            put(historyElement.getPlayerSouthLogin(), holder.southButton);
            put(historyElement.getPlayerEastLogin(), holder.eastButton);
            put(historyElement.getPlayerWestLogin(), holder.westButton);
        }};

        Map<String, String> loginAliasMap = new HashMap<String, String>() {{
            put(historyElement.getPlayerNorthLogin(), historyElement.getPlayerNorthAlias());
            put(historyElement.getPlayerSouthLogin(), historyElement.getPlayerSouthAlias());
            put(historyElement.getPlayerEastLogin(), historyElement.getPlayerEastAlias());
            put(historyElement.getPlayerWestLogin(), historyElement.getPlayerWestAlias());
        }};

        for (Map.Entry<String, Button> entry : loginButtonsMap.entrySet()) {
            if (entry.getKey().equals(login)) {
                activity.runOnUiThread(() -> entry.getValue().setText("Вы"));
                continue;
            }
            activity.runOnUiThread(() -> entry.getValue().setText(loginAliasMap.get(entry.getKey())));
            entry.getValue().setOnClickListener(v -> activity.runOnUiThread(() -> {
                AlertDialog.Builder builder = new AlertDialog.Builder(context, R.style.CustomDialogTheme);
                builder.setTitle("Добавление в друзья");
                builder.setMessage("Добавить в друзья " + loginAliasMap.get(entry.getKey()) + "?");
                View view = LayoutInflater.from(context).inflate(R.layout.fragment_invitation,
                        activity.findViewById(R.id.invite_layout));
                builder.setView(view);
                builder.setNegativeButton("Нет", (dialog, id) -> {
                    // Ничего не делать
                });
                builder.setPositiveButton("Да", (dialog, id) -> {
                    AddFriendToServer addFriend = new AddFriendToServer(entry.getKey());
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(addFriend);
                    Message message = new Message(token, "add_friend", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                });
                AlertDialog dialog = builder.create();
                dialog.setCanceledOnTouchOutside(false);
                dialog.show();
            }));
        }
    }

    @Override
    public int getItemCount() {
        return historyList.size();
    }

    static class HistoryActivityViewHolder extends RecyclerView.ViewHolder {
        private final TextView time;
        private final TextView lobbyOwner;
        private final TextView winnerTeam;
        private final TextView team1;
        private final TextView team2;

        private final Button northButton;
        private final Button southButton;
        private final Button eastButton;
        private final Button westButton;

        public HistoryActivityViewHolder(@NonNull View itemView) {
            super(itemView);
            this.time = itemView.findViewById(R.id.history_element_time);
            this.lobbyOwner = itemView.findViewById(R.id.history_element_owner);
            this.winnerTeam = itemView.findViewById(R.id.history_element_winner_team);
            this.team1 = itemView.findViewById(R.id.history_element_team1);
            this.team2 = itemView.findViewById(R.id.history_element_team2);
            this.northButton = itemView.findViewById(R.id.history_element_north);
            this.southButton = itemView.findViewById(R.id.history_element_south);
            this.eastButton = itemView.findViewById(R.id.history_element_east);
            this.westButton = itemView.findViewById(R.id.history_element_west);
        }
    }
}
