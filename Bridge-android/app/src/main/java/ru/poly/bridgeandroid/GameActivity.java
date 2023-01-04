package ru.poly.bridgeandroid;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.material.navigation.NavigationView;
import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringJoiner;

import ru.poly.bridgeandroid.enums.BidCall;
import ru.poly.bridgeandroid.enums.CardSuit;
import ru.poly.bridgeandroid.enums.GamePhase;
import ru.poly.bridgeandroid.enums.PlayerPosition;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.Bid;
import ru.poly.bridgeandroid.model.game.BidSelected;
import ru.poly.bridgeandroid.model.game.Card;
import ru.poly.bridgeandroid.model.game.MoveSelected;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.AcceptInvitePlayersToServer;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToClient;
import ru.poly.bridgeandroid.model.menu.ExitLobbyToServer;

public class GameActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";

    private static final int NORTH_SOUTH_ROWS_COUNT = 2;
    private static final int NORTH_SOUTH_COLUMNS_COUNT = 7;
    private static final int WEST_EAST_ROWS_COUNT = 7;
    private static final int WEST_EAST_COLUMNS_COUNT = 2;
    private static final int PLAYERS_COUNT = 4;

    private Gson gson;
    private String token;
    private String lobbyId;
    private String login;
    private SharedPreferences sharedPreferences;

    private PlayerGameState gameState;
    private PlayerPosition mainPlayerPosition;
    private boolean notifyBidTurn;
    private boolean notifyMoveTurn;
    private boolean isBiddingPhase;
    private boolean isMatchEnded;
    private int tricksToWin;
    private int enemyTricksToWin;
    private int ourTeamIndex;
    private int theyTeamIndex;
    private HashMap<PlayerPosition, Integer> newPlayerPositions = new HashMap<>();

    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ActionBarDrawerToggle actionBarDrawerToggleMenu;
    private final List<ImageView> playerCardsImageView = new ArrayList<>(14);
    private final List<ImageView> teammateCardsImageView = new ArrayList<>(14);
    private final List<ImageView> leftEnemyCardsImageView = new ArrayList<>(14);
    private final List<ImageView> rightEnemyCardsImageView = new ArrayList<>(14);
    private ImageView contractBidImageView;
    private TextView localScoreTextView;
    private TextView globalScoreTextView;

    private List<Card> playerCards = new ArrayList<>(13);
    private List<Card> teammateCards = new ArrayList<>(13);

    AlertDialog dialog;

    private final Map<Integer, String> suitDrawableNames = new HashMap<Integer, String>() {{
        put(0, "c");
        put(1, "d");
        put(2, "h");
        put(3, "s");
    }};
    private final Map<Integer, String> rankDrawableNames = new HashMap<Integer, String>() {{
        put(1, "2");
        put(2, "3");
        put(3, "4");
        put(4, "5");
        put(5, "6");
        put(6, "7");
        put(7, "8");
        put(8, "9");
        put(9, "10");
        put(10, "j");
        put(11, "q");
        put(12, "k");
        put(13, "a");
    }};
    private final Map<CardSuit, String> suitBidNames = new HashMap<CardSuit, String>() {{
        put(CardSuit.CLUBS, "c");
        put(CardSuit.DIAMONDS, "d");
        put(CardSuit.HEARTS, "h");
        put(CardSuit.SPADES, "s");
        put(CardSuit.NONE, "nt");
    }};

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_game);

        hideSystemBars();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");
        login = sharedPreferences.getString(LOGIN, "");

        gson = new Gson();

        Intent myIntent = getIntent();
        gameState = myIntent.getParcelableExtra("gameState");
        notifyBidTurn = myIntent.getBooleanExtra("notifyBidTurn", false);
        isBiddingPhase = true;

        drawerLayout = findViewById(R.id.game_drawer_layout);
        navigationView = findViewById(R.id.game_navigation_view);
        actionBarDrawerToggleMenu = new ActionBarDrawerToggle(this, drawerLayout,
                R.string.nav_open, R.string.nav_close);

        contractBidImageView = findViewById(R.id.game_table_contract_bid);
        localScoreTextView = findViewById(R.id.local_score);
        globalScoreTextView = findViewById(R.id.global_score);

        // Player
        for (int i = 0; i < NORTH_SOUTH_ROWS_COUNT; i++) {
            for (int j = 0; j < NORTH_SOUTH_COLUMNS_COUNT; j++) {
                playerCardsImageView.add(findViewById(getResources().getIdentifier("game_table_row" + (i + 1)
                        + "_player_card" + (j + 1), "id", getPackageName())));
            }
        }

        // Left enemy
        for (int i = 0; i < WEST_EAST_ROWS_COUNT; i++) {
            for (int j = 0; j < WEST_EAST_COLUMNS_COUNT; j++) {
                leftEnemyCardsImageView.add(findViewById(getResources().getIdentifier("game_table_row" + (i + 1)
                        + "_enemy_left_card" + (j + 1), "id", GameActivity.this.getPackageName())));
            }
        }

        // Teammate
        for (int i = 0; i < NORTH_SOUTH_ROWS_COUNT; i++) {
            for (int j = 0; j < NORTH_SOUTH_COLUMNS_COUNT; j++) {
                teammateCardsImageView.add(findViewById(getResources().getIdentifier("game_table_row" + (i + 1)
                        + "_teammate_card" + (j + 1), "id", GameActivity.this.getPackageName())));
            }
        }

        // Right enemy
        for (int i = 0; i < WEST_EAST_ROWS_COUNT; i++) {
            for (int j = 0; j < WEST_EAST_COLUMNS_COUNT; j++) {
                rightEnemyCardsImageView.add(findViewById(getResources().getIdentifier("game_table_row" + (i + 1)
                        + "_enemy_right_card" + (j + 1), "id", GameActivity.this.getPackageName())));
            }
        }

        loadFragment(BargainingFragment.newInstance(gameState, notifyBidTurn));

        drawerLayout.addDrawerListener(actionBarDrawerToggleMenu);
        actionBarDrawerToggleMenu.syncState();
        navigationView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                switch (item.getItemId()) {
                    case R.id.nav_scores:
                        // TODO: таблица очков
                        showScoresDialog();
                        return true;
                    case R.id.nav_settings:
                        // TODO: настройки или вообще убрать
                        Log.i("onOptionsItemSelected", "Settings");
                        return true;
                    case R.id.nav_exit:
                        ExitLobbyToServer exitGame = new ExitLobbyToServer(lobbyId, true);
                        JsonObject jsonObject = (JsonObject) gson.toJsonTree(exitGame);
                        Message message = new Message(token, "exit_lobby", jsonObject);
                        EventBus.getDefault().post(gson.toJson(message));
                        return true;
                }
                return true;
            }
        });

        Toolbar toolbar = findViewById(R.id.game_toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeAsUpIndicator(R.drawable.ic_baseline_menu_24);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setDisplayShowTitleEnabled(false);

        for (int i = 0; i < playerCardsImageView.size(); i++) {
            ImageView cardImageView = playerCardsImageView.get(i);
            int cardIndex = i;
            cardImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Card card = playerCards.get(cardIndex);
                    MoveSelected moveSelected = new MoveSelected(gson.fromJson(gson.toJson(card), JsonObject.class));
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(moveSelected);
                    Message message = new Message(token, "move_send", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
            });
            cardImageView.setClickable(false);
        }

        for (int i = 0; i < teammateCardsImageView.size(); i++) {
            ImageView cardImageView = teammateCardsImageView.get(i);
            int cardIndex = i;
            cardImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Card card = teammateCards.get(cardIndex);
                    MoveSelected moveSelected = new MoveSelected(gson.fromJson(gson.toJson(card), JsonObject.class));
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(moveSelected);
                    Message message = new Message(token, "move_send", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
            });
            cardImageView.setClickable(false);
        }

        initializeNewPlayerPositions();
        ourTeamIndex = mainPlayerPosition.equals(PlayerPosition.NORTH) ||
                mainPlayerPosition.equals(PlayerPosition.SOUTH) ? 0 : 1;
        theyTeamIndex = 1 - ourTeamIndex;

        updateGameUI();

        int ourTeamScores = gameState.getScore().getAllScores(ourTeamIndex);
        int theyTeamScores = gameState.getScore().getAllScores(theyTeamIndex);
        changeGlobalScoreTextView(ourTeamScores, theyTeamScores, gameState.getDealNumber(),
                gameState.getGameNumber(), gameState.getRubberNumber());
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (actionBarDrawerToggleMenu.onOptionsItemSelected(item)) {
            return true;
        }
        return super.onOptionsItemSelected(item);
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

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                gameState = updateGameState.getGameState();
                notifyBidTurn = false;
                notifyMoveTurn = false;
                updateGameUI();
                break;
            case "notify_bid_turn":
                notifyBidTurn = true;
                break;
            case "notify_bid_rejected":
                break;
            case "notify_move_turn":
                notifyMoveTurn = true;
                if (gameState.getHandToPlay().equals(mainPlayerPosition)) {
                    playerCards = getSortedCards(true);
                } else {
                    teammateCards = getSortedCards(false);
                }
                setClickableCards(!gameState.getHandToPlay().equals(mainPlayerPosition), true);
                break;
            case "notify_move_rejected":
                break;
            case "exit_lobby":
                ExitLobbyToClient exitLobby = message.getData(ExitLobbyToClient.class);
                if (exitLobby.isSuccessful()) {
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.remove(LOBBY);
                    editor.apply();

                    if (isMatchEnded) {
                        showScoresGameEndDialog();
                    } else {
                        runOnUiThread(() -> {
                            Toast toast = Toast.makeText(getBaseContext(),
                                    "Один из игроков покинул игру.", Toast.LENGTH_SHORT);
                            toast.show();
                            if (dialog != null) {
                                dialog.dismiss();
                            }
                        });

                        Intent intent = new Intent(GameActivity.this, MenuActivity.class);
                        startActivity(intent);
                        finish();
                    }
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

    private void updateGameUI() {
        switch (gameState.getGameEvent()) {
            case BID_START:
                if (!isBiddingPhase) {
                    loadFragment(BargainingFragment.newInstance(gameState,
                            gameState.getPlayerTurn().equals(gameState.getPlayerPositionForLogin(login))));
                }
                for (PlayerPosition playerPosition : PlayerPosition.values()) {
                    changeCardsVisibility(playerPosition, gameState.getPlayerCardCountMap().get(playerPosition));
                }
                highlightPlayer(gameState.getPlayerTurn());
                showCards(true, true);
                break;
            case BID_RESTART:
                for (PlayerPosition playerPosition : PlayerPosition.values()) {
                    changeCardsVisibility(playerPosition, gameState.getPlayerCardCountMap().get(playerPosition));
                }
                highlightPlayer(gameState.getPlayerTurn());
                showCards(true, true);
                break;
            case PLAYER_BID:
                highlightPlayer(gameState.getPlayerTurn());
                break;
            case BID_END:
                runOnUiThread(() -> {
                    contractBidImageView.setImageResource(getBidDrawableId(
                            gameState.getContractBid().getCardSuit(),
                            gameState.getContractBid().getTricksAbove()));
                    Toast.makeText(this, "Торги закончены", Toast.LENGTH_SHORT).show();
                });
                isBiddingPhase = false;
                break;
            case PLAY_START:
                if (newPlayerPositions.get(gameState.getContractBid().getPlayerPosition()) == 0 ||
                        newPlayerPositions.get(gameState.getContractBid().getPlayerPosition()) == 2) {
                    tricksToWin = 6 + gameState.getContractBid().getTricksAbove();
                    enemyTricksToWin = 14 - tricksToWin;
                } else {
                    enemyTricksToWin = 6 + gameState.getContractBid().getTricksAbove();
                    tricksToWin = 14 - enemyTricksToWin;
                }
                changeLocalScoreTextView(0, tricksToWin, 0, enemyTricksToWin);
                loadFragment(DrawFragment.newInstance(gameState, newPlayerPositions));
                break;
            case TRICK_START:
            case PLAYER_MOVED:
                for (PlayerPosition playerPosition : PlayerPosition.values()) {
                    changeCardsVisibility(playerPosition, gameState.getPlayerCardCountMap().get(playerPosition));
                }
                highlightPlayer(gameState.getHandToPlay());
                showCards(true, true);
                showCards(false, true);
                setClickableCards(false, false);
                setClickableCards(true, false);
                // TODO: тоже самое, что и для TRICK_START и PLAYER_MOVED
                break;
            case TRICK_END:
                for (PlayerPosition playerPosition : PlayerPosition.values()) {
                    changeCardsVisibility(playerPosition, gameState.getPlayerCardCountMap().get(playerPosition));
                }
                highlightPlayer(gameState.getHandToPlay()); // TODO: Нужно ли подсвечивать при TRICK_END
                showCards(true, true);
                showCards(false, true);
                setClickableCards(false, false);
                setClickableCards(true, false);
                Map<PlayerPosition, Integer> tricksWon = gameState.getTricksWon();
                int ourScores = 0;
                int theyScores = 0;
                for (int i = 0; i < newPlayerPositions.size(); i++) {
                    if (i % 2 == 0) {
                        ourScores += tricksWon.get(getKey(newPlayerPositions, i));
                    } else {
                        theyScores += tricksWon.get(getKey(newPlayerPositions, i));
                    }
                }
                changeLocalScoreTextView(ourScores, tricksToWin, theyScores, enemyTricksToWin);
                break;
            case PLAY_END:
                int ourTeamScores = gameState.getScore().getAllScores(ourTeamIndex);
                int theyTeamScores = gameState.getScore().getAllScores(theyTeamIndex);
                changeGlobalScoreTextView(ourTeamScores, theyTeamScores, gameState.getDealNumber(),
                        gameState.getGameNumber(), gameState.getRubberNumber());
                showCards(false, false);
                setClickableCards(false, false);
                setClickableCards(true, false);
                runOnUiThread(() -> contractBidImageView.setImageResource(0));
                localScoreTextView.setText("");
                break;
            case MATCH_END:
                isMatchEnded = true;
                break;
            case RUBBER_COMPLETED:
                break;
            case PLAY_STOP:
                runOnUiThread(() -> {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Потеряно соединение с одним из игроков.");
                    builder.setMessage("Вы можете подождать игрока некоторое время или выйти из игры.");
                    builder.setPositiveButton("Выйти из игры", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            ExitLobbyToServer exitGame = new ExitLobbyToServer(lobbyId, true);
                            JsonObject jsonObject = (JsonObject) gson.toJsonTree(exitGame);
                            Message message = new Message(token, "exit_lobby", jsonObject);
                            EventBus.getDefault().post(gson.toJson(message));
                        }
                    });
                    dialog = builder.create();
                    dialog.setCanceledOnTouchOutside(false);
                    dialog.show();
                });
                break;
            case PLAY_CONTINUES:
                runOnUiThread(() -> {
                    Toast toast = Toast.makeText(getBaseContext(),
                            "Игрок снова присоединился в игру.", Toast.LENGTH_SHORT);
                    toast.show();
                    if (dialog != null) {
                        dialog.dismiss();
                    }
                });
                break;
        }
        // TODO: изменение всего UI игры
    }

    private void highlightPlayer(PlayerPosition playerPosition) {
        int clientPlayerPosition = newPlayerPositions.get(playerPosition);
        for (int i = 0; i < PLAYERS_COUNT; i++) {
            switch (i) {
                case 0:
                    if (i == clientPlayerPosition) {
                        for (ImageView cardImageView : playerCardsImageView) {
                            cardImageView.clearColorFilter();
                        }
                    } else {
                        for (ImageView cardImageView : playerCardsImageView) {
                            cardImageView.setColorFilter(ContextCompat.getColor(GameActivity.this, R.color.black_overlay_cards));
                        }
                    }
                    break;
                case 1:
                    if (i == clientPlayerPosition) {
                        for (ImageView cardImageView : leftEnemyCardsImageView) {
                            cardImageView.clearColorFilter();
                        }
                    } else {
                        for (ImageView cardImageView : leftEnemyCardsImageView) {
                            cardImageView.setColorFilter(ContextCompat.getColor(GameActivity.this, R.color.black_overlay_cards));
                        }
                    }
                    break;
                case 2:
                    if (i == clientPlayerPosition) {
                        for (ImageView cardImageView : teammateCardsImageView) {
                            cardImageView.clearColorFilter();
                        }
                    } else {
                        for (ImageView cardImageView : teammateCardsImageView) {
                            cardImageView.setColorFilter(ContextCompat.getColor(GameActivity.this, R.color.black_overlay_cards));
                        }
                    }
                    break;
                case 3:
                    if (i == clientPlayerPosition) {
                        for (ImageView cardImageView : rightEnemyCardsImageView) {
                            cardImageView.clearColorFilter();
                        }
                    } else {
                        for (ImageView cardImageView : rightEnemyCardsImageView) {
                            cardImageView.setColorFilter(ContextCompat.getColor(GameActivity.this, R.color.black_overlay_cards));
                        }
                    }
                    break;
            }
        }
    }

    private void changeCardsVisibility(PlayerPosition playerPosition, int cardsCount) {
        int clientPlayerPosition = newPlayerPositions.get(playerPosition);
        switch (clientPlayerPosition) {
            case 0:
                for (int i = 0; i < cardsCount; i++) {
                    playerCardsImageView.get(i).setVisibility(View.VISIBLE);
                }
                for (int i = cardsCount; i < playerCardsImageView.size(); i++) {
                    playerCardsImageView.get(i).setVisibility(View.INVISIBLE);
                }
                break;
            case 1:
                for (int i = 0; i < cardsCount; i++) {
                    leftEnemyCardsImageView.get(i).setVisibility(View.VISIBLE);
                }
                for (int i = cardsCount; i < leftEnemyCardsImageView.size(); i++) {
                    leftEnemyCardsImageView.get(i).setVisibility(View.INVISIBLE);
                }
                break;
            case 2:
                for (int i = 0; i < cardsCount; i++) {
                    teammateCardsImageView.get(i).setVisibility(View.VISIBLE);
                }
                for (int i = cardsCount; i < teammateCardsImageView.size(); i++) {
                    teammateCardsImageView.get(i).setVisibility(View.INVISIBLE);
                }
                break;
            case 3:
                for (int i = 0; i < cardsCount; i++) {
                    rightEnemyCardsImageView.get(i).setVisibility(View.VISIBLE);
                }
                for (int i = cardsCount; i < rightEnemyCardsImageView.size(); i++) {
                    rightEnemyCardsImageView.get(i).setVisibility(View.INVISIBLE);
                }
                break;
        }
    }

    private void showCards(boolean isPlayerHand, boolean isVisible) {
        PlayerPosition teammatePosition = gameState.getDeclarer().getPositionIndex() < 2 ?
                PlayerPosition.getPlayerPosition(gameState.getDeclarer().getPositionIndex() + 2) :
                PlayerPosition.getPlayerPosition(gameState.getDeclarer().getPositionIndex() - 2);
        int playerPosition = isPlayerHand ? newPlayerPositions.get(mainPlayerPosition) :
                newPlayerPositions.get(teammatePosition);
        List<Card> cards = getSortedCards(isPlayerHand);
        int cardsSize = isVisible ? cards.size() : 13;

        switch (playerPosition) {
            case 0:
                for (int i = 0; i < cardsSize; i++) {
                    ImageView cardImageView = playerCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    if (isVisible) {
                        cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                    } else {
                        cardImageView.setImageResource(getResources().getIdentifier("back", "drawable",
                                getPackageName()));
                    }
                }
                break;
            case 1:
                for (int i = 0; i < cardsSize; i++) {
                    ImageView cardImageView = leftEnemyCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    if (isVisible) {
                        cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                    } else {
                        cardImageView.setImageResource(getResources().getIdentifier("back", "drawable",
                                getPackageName()));
                    }
                }
                break;
            case 2:
                for (int i = 0; i < cardsSize; i++) {
                    ImageView cardImageView = teammateCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    if (isVisible) {
                        cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                    } else {
                        cardImageView.setImageResource(getResources().getIdentifier("back", "drawable",
                                getPackageName()));
                    }
                }
                break;
            case 3:
                for (int i = 0; i < cardsSize; i++) {
                    ImageView cardImageView = rightEnemyCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    if (isVisible) {
                        cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                    } else {
                        cardImageView.setImageResource(getResources().getIdentifier("back", "drawable",
                                getPackageName()));
                    }
                }
                break;
        }
    }

    private List<Card> getSortedCards(boolean isPlayerHand) {
        List<Card> cards = isPlayerHand ? gameState.getPlayerHand().getCards() :
                gameState.getDummyHand().getCards();

        Collections.sort(cards, (card1, card2) -> {
            int compareSuit = Integer.compare(card1.getSuit().getSuitIndex(),
                    card2.getSuit().getSuitIndex());
            return compareSuit != 0 ? compareSuit :
                    Integer.compare(card1.getRank().getRankIndex(), card2.getRank().getRankIndex());
        });
        return cards;
    }

    private int getCardDrawableId(Card card) {
        String suitName = suitDrawableNames.get(card.getSuit().getSuitIndex());
        String rankName = rankDrawableNames.get(card.getRank().getRankIndex());
        return getResources().getIdentifier(suitName + "_" + rankName, "drawable",
                getPackageName());
    }

    private void initializeNewPlayerPositions() {
        List<Integer> newPositions = Arrays.asList(0, 1, 2, 3);
        mainPlayerPosition = gameState.getPlayerPositionForLogin(login);
        cyclicShiftLeft(newPositions, mainPlayerPosition.getPositionIndex());
        for (int i = 0; i < 4; i++) {
            newPlayerPositions.put(PlayerPosition.getPlayerPosition(newPositions.get(i)), i);
        }
    }

    private void cyclicShiftLeft(List<Integer> list, int shift) {
        int[] temp = new int[list.size()];
        int k = 0;
        for (int i = shift; i < list.size(); i++) {
            temp[k] = list.get(i);
            k++;
        }

        for (int i = 0; i < shift; i++) {
            temp[k] = list.get(i);
            k++;
        }

        for (int i = 0; i < list.size(); i++) {
            list.set(i, temp[i]);
        }
    }

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }

    private void loadFragment(Fragment fragment) {
        // create a FragmentManager
        FragmentManager fm = getSupportFragmentManager();
        // create a FragmentTransaction to begin the transaction and replace the Fragment
        FragmentTransaction fragmentTransaction = fm.beginTransaction();
        // replace the FrameLayout with new Fragment
        fragmentTransaction.replace(R.id.game_fragment, fragment);
        fragmentTransaction.commit(); // save the changes
    }

    private int getBidDrawableId(CardSuit cardSuit, int tricksAbove) {
        String suitName = suitBidNames.get(cardSuit);
        return getResources().getIdentifier("bargaining_" + suitName + "_" + tricksAbove, "drawable",
                getPackageName());
    }

    private void changeLocalScoreTextView(int ourScores, int ourMaxScores,
                                            int theyScores, int theyMaxScores) {
        String scoreText = "Мы: " + ourScores + "/" + ourMaxScores + "\nОни: " +
                theyScores + "/" + theyMaxScores;
        runOnUiThread(() -> {
            localScoreTextView.setVisibility(View.VISIBLE);
            localScoreTextView.setText(scoreText);
        });
    }
    //ScoreText
    private void changeGlobalScoreTextView(int ourScores, int theyScores, int dealNumber,
                                           int gameNumber, int rubberNumber) {
        String scoreText = "Мы: " + ourScores+ "\nОни: " + theyScores + "\nСделка: " + dealNumber +
                "\nГейм: " + gameNumber + "\nРоббер: " + rubberNumber;
        runOnUiThread(() -> {
            globalScoreTextView.setVisibility(View.VISIBLE);
            globalScoreTextView.setText(scoreText);
        });
    }

    private void setClickableCards(boolean isDummy, boolean isClickable) {
        if (isDummy) {
            for (int i = 0; i < teammateCardsImageView.size(); i++) {
                int cardIndex = i;
                runOnUiThread(() -> {
                    teammateCardsImageView.get(cardIndex).setClickable(isClickable);
                });
            }
        } else {
            for (int i = 0; i < playerCardsImageView.size(); i++) {
                int cardIndex = i;
                runOnUiThread(() -> {
                    playerCardsImageView.get(cardIndex).setClickable(isClickable);
                });
            }
        }
    }
    //Alerts
    private void showScoresDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        View view = LayoutInflater.from(this).inflate(R.layout.fragment_scores, findViewById(R.id.score_layout));
        builder.setView(view);

        TextView bonusesTextView1 = view.findViewById(R.id.score_bonuses_team1);
        TextView bonusesTextView2 = view.findViewById(R.id.score_bonuses_team2);
        TextView contractTextView1 = view.findViewById(R.id.score_contract_team1);
        TextView contractTextView2 = view.findViewById(R.id.score_contract_team2);
        TextView allTextView1 = view.findViewById(R.id.score_all_team1);
        TextView allTextView2 = view.findViewById(R.id.score_all_team2);

        bonusesTextView1.setText(String.valueOf(gameState.getScore().getBonusScores(ourTeamIndex)));
        bonusesTextView2.setText(String.valueOf(gameState.getScore().getBonusScores(theyTeamIndex)));

        int[] ourContractScoresArray = gameState.getScore().getContractPoints()[ourTeamIndex];
        int[] theyContractScoresArray = gameState.getScore().getContractPoints()[theyTeamIndex];
        StringBuilder ourContractScores = new StringBuilder();
        StringBuilder theyContractScores = new StringBuilder();

        for (int score : ourContractScoresArray) {
            ourContractScores.append(score).append("\n");
        }
        for (int score : theyContractScoresArray) {
            theyContractScores.append(score).append("\n");
        }
        contractTextView1.setText(ourContractScores.substring(0, ourContractScores.length() - 1));
        contractTextView2.setText(theyContractScores.substring(0, theyContractScores.length() - 1));

        allTextView1.setText(String.valueOf(gameState.getScore().getAllScores(ourTeamIndex)));
        allTextView2.setText(String.valueOf(gameState.getScore().getAllScores(theyTeamIndex)));

        final AlertDialog alertDialog = builder.create();
        alertDialog.show();
    }

    private void showScoresGameEndDialog() {
        TextView textview = new TextView(GameActivity.this);
        textview.setText("\nИгра окончена!");
        textview.setTextSize(26);
        textview.setTypeface(null, Typeface.BOLD);
        textview.setGravity(Gravity.CENTER);
        textview.setTextColor(getResources().getColor(R.color.menu_yellow));

        AlertDialog.Builder builder = new AlertDialog.Builder(GameActivity.this, R.style.CustomDialogTheme);
        //builder.setTitle("Игра окончена!");
        builder.setCustomTitle(textview);
        View view = LayoutInflater.from(this).inflate(R.layout.fragment_scores, findViewById(R.id.score_layout));
        builder.setView(view);

        TextView bonusesTextView1 = view.findViewById(R.id.score_bonuses_team1);
        TextView bonusesTextView2 = view.findViewById(R.id.score_bonuses_team2);
        TextView contractTextView1 = view.findViewById(R.id.score_contract_team1);
        TextView contractTextView2 = view.findViewById(R.id.score_contract_team2);
        TextView allTextView1 = view.findViewById(R.id.score_all_team1);
        TextView allTextView2 = view.findViewById(R.id.score_all_team2);

        bonusesTextView1.setText(String.valueOf(gameState.getScore().getBonusScores(ourTeamIndex)));
        bonusesTextView2.setText(String.valueOf(gameState.getScore().getBonusScores(theyTeamIndex)));

        int[] ourContractScoresArray = gameState.getScore().getContractPoints()[ourTeamIndex];
        int[] theyContractScoresArray = gameState.getScore().getContractPoints()[theyTeamIndex];
        StringBuilder ourContractScores = new StringBuilder();
        StringBuilder theyContractScores = new StringBuilder();

        for (int score : ourContractScoresArray) {
            ourContractScores.append(score).append("\n");
        }
        for (int score : theyContractScoresArray) {
            theyContractScores.append(score).append("\n");
        }
        contractTextView1.setText(ourContractScores.substring(0, ourContractScores.length() - 1));
        contractTextView2.setText(theyContractScores.substring(0, theyContractScores.length() - 1));

        allTextView1.setText(String.valueOf(gameState.getScore().getAllScores(ourTeamIndex)));
        allTextView2.setText(String.valueOf(gameState.getScore().getAllScores(theyTeamIndex)));

        builder.setNegativeButton("Выход", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                Intent intent = new Intent(GameActivity.this, MenuActivity.class);
                startActivity(intent);
                finish();
            }
        });

        final AlertDialog alertDialog = builder.create();
        alertDialog.setCanceledOnTouchOutside(false);
        alertDialog.show();
    }

    private <K, V> K getKey(Map<K, V> map, V value) {
        for (Map.Entry<K, V> entry : map.entrySet()) {
            if (entry.getValue().equals(value)) {
                return entry.getKey();
            }
        }
        return null;
    }
}
