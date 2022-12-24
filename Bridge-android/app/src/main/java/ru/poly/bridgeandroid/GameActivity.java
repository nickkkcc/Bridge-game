package ru.poly.bridgeandroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
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

import com.google.gson.Gson;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ru.poly.bridgeandroid.enums.BidCall;
import ru.poly.bridgeandroid.enums.CardSuit;
import ru.poly.bridgeandroid.enums.GamePhase;
import ru.poly.bridgeandroid.enums.PlayerPosition;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.Card;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;

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
    private Map<PlayerPosition, Integer> newPlayerPositions = new HashMap<>();

    private DrawerLayout drawerLayout;
    private ActionBarDrawerToggle actionBarDrawerToggleMenu;
    private final List<ImageView> playerCardsImageView = new ArrayList<>(14);
    private final List<ImageView> teammateCardsImageView = new ArrayList<>(14);
    private final List<ImageView> leftEnemyCardsImageView = new ArrayList<>(14);
    private final List<ImageView> rightEnemyCardsImageView = new ArrayList<>(14);
    private ImageView contractBidImageView;
    private TextView localScoreTextView;
    private View gameFragment;

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

        drawerLayout = findViewById(R.id.game_drawer_layout);
        actionBarDrawerToggleMenu = new ActionBarDrawerToggle(this, drawerLayout,
                R.string.nav_open, R.string.nav_close);

        contractBidImageView = findViewById(R.id.game_table_contract_bid);
        localScoreTextView = findViewById(R.id.local_score);

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

//        gameFragment = findViewById(R.id.game_fragment);
        loadFragment(BargainingFragment.newInstance(gameState, notifyBidTurn));

        drawerLayout.addDrawerListener(actionBarDrawerToggleMenu);
        actionBarDrawerToggleMenu.syncState();

        Toolbar toolbar = findViewById(R.id.game_toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeAsUpIndicator(R.drawable.ic_baseline_menu_24);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setDisplayShowTitleEnabled(false);

        for (ImageView cardImageView : playerCardsImageView) {
            cardImageView.setClickable(true);
            cardImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    // TODO: нажатие на карту
                }
            });
        }

        initializeNewPlayerPositions();
        updateGameUI();
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

    @Subscribe
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                gameState = updateGameState.getGameState();
                notifyBidTurn = false;
                updateGameUI();
                break;
            case "notify_bid_turn":
                notifyBidTurn = true;
                break;
            case "notify_bid_rejected":
                break;
            default:
                throw new RuntimeException();
        }
    }

    private void updateGameUI() {
        switch (gameState.getGameEvent()) {
            case BID_START:
            case BID_RESTART:
                for (PlayerPosition playerPosition : PlayerPosition.values()) {
                    changeCardsVisibility(playerPosition, gameState.getPlayerCardCountMap().get(playerPosition));
                }
                highlightPlayer(gameState.getPlayerTurn());
                showCards(true);
                break;
                // TODO: тоже самое, что и BID_START
            case PLAYER_BID:
                highlightPlayer(gameState.getPlayerTurn());
                break;
            case BID_END:
                loadFragment(DrawFragment.newInstance(gameState));
                runOnUiThread(() -> {
                    contractBidImageView.setImageResource(getBidDrawableId(gameState.getContractBid().getCardSuit(),
                            gameState.getContractBid().getTricksAbove()));
                    localScoreTextView.setVisibility(View.VISIBLE);
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
                            cardImageView.setClickable(gameState.getPhase().equals(GamePhase.CARD_PLAY));
                            cardImageView.clearColorFilter();
                        }
                    } else {
                        for (ImageView cardImageView : playerCardsImageView) {
                            cardImageView.setClickable(false);
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

    private void showCards(boolean isPlayerHand) {
        int playerPosition = isPlayerHand ? newPlayerPositions.get(mainPlayerPosition) :
                newPlayerPositions.get(gameState.getPlayerTurn());
        List<Card> cards = isPlayerHand ? gameState.getPlayerHand().getCards() :
                gameState.getDummyHand().getCards();

        Collections.sort(cards, (card1, card2) -> {
            int compareSuit = Integer.compare(card1.getSuit().getSuitIndex(),
                    card2.getSuit().getSuitIndex());
            return compareSuit != 0 ? compareSuit :
                    Integer.compare(card1.getRank().getRankIndex(), card2.getRank().getRankIndex());
        });

        switch (playerPosition) {
            case 0:
                for (int i = 0; i < cards.size(); i++) {
                    ImageView cardImageView = playerCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                }
                break;
            case 1:
                for (int i = 0; i < cards.size(); i++) {
                    ImageView cardImageView = leftEnemyCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                }
                break;
            case 2:
                for (int i = 0; i < cards.size(); i++) {
                    ImageView cardImageView = teammateCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                }
                break;
            case 3:
                for (int i = 0; i < cards.size(); i++) {
                    ImageView cardImageView = rightEnemyCardsImageView.get(i);
                    cardImageView.setVisibility(View.VISIBLE);
                    cardImageView.setImageResource(getCardDrawableId(cards.get(i)));
                }
                break;
        }
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
}
