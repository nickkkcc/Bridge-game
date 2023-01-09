package ru.poly.bridgeandroid;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

import ru.poly.bridgeandroid.enums.PlayerPosition;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.Bid;
import ru.poly.bridgeandroid.model.game.Card;
import ru.poly.bridgeandroid.model.game.Cards;
import ru.poly.bridgeandroid.model.game.NotifyRejected;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;

public class DrawFragment extends Fragment {

    private ImageView playerCardImageView;
    private ImageView teammateCardImageView;
    private ImageView enemyLeftCardImageView;
    private ImageView enemyRightCardImageView;
    private ImageView pointerImageView;

    private PlayerGameState gameState;
    private HashMap<PlayerPosition, Integer> newPlayerPositions = new HashMap<>();

    private final Map<Integer, Float> playersRotations = new HashMap<Integer, Float>() {{
        put(0, 180f);
        put(1, 270f);
        put(2, 0f);
        put(3, 90f);
    }};

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

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_draw, container, false);

        playerCardImageView = view.findViewById(R.id.draw_player_card);
        teammateCardImageView = view.findViewById(R.id.draw_teammate_card);
        enemyLeftCardImageView = view.findViewById(R.id.draw_enemy_left_card);
        enemyRightCardImageView = view.findViewById(R.id.draw_enemy_right_card);
        pointerImageView = view.findViewById(R.id.draw_pointer);

        Bundle bundle = getArguments();
        gameState = bundle.getParcelable("gameState");
        newPlayerPositions = (HashMap<PlayerPosition, Integer>) bundle.getSerializable("newPlayerPositions");

        setCardsInvisible();
        updateFragmentUI();

        return view;
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

    public static DrawFragment newInstance(PlayerGameState playerGameState,
                                           HashMap<PlayerPosition, Integer> newPlayerPositions) {
        DrawFragment drawFragment = new DrawFragment();

        Bundle args = new Bundle();
        args.putParcelable("gameState", playerGameState);
        args.putSerializable("newPlayerPositions", newPlayerPositions);
        drawFragment.setArguments(args);

        return drawFragment;
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                gameState = updateGameState.getGameState();
                updateFragmentUI();
                break;
            case "notify_move_turn":
                break;
            case "notify_move_rejected":
                NotifyRejected notifyMoveRejected = message.getData(NotifyRejected.class);
                getActivity().runOnUiThread(() -> Toast.makeText(getActivity(), notifyMoveRejected.getReason(), Toast.LENGTH_SHORT).show());
                break;
            default:
//                throw new RuntimeException();
        }
    }

    private void updateFragmentUI() {
        PlayerPosition currentPlayerPosition = gameState.getHandToPlay();
        switch (gameState.getGameEvent()) {
            case PLAY_START:
            case TRICK_START:
                setCardsInvisible();
                rotatePointer(currentPlayerPosition);
                break;
            case PLAYER_MOVED:
                List<Cards> cardsList = gameState.getTricks();
                List<Card> cards = cardsList.get(cardsList.size() - 1).getCards();
                int newCurrentCardPosition = newPlayerPositions.get(currentPlayerPosition);
                int previousCardPlayerPosition = newCurrentCardPosition == 0 ? 3 : newCurrentCardPosition - 1;
                if (cards.size() < 4) {
                    for (int i = 0; i < cards.size(); i++) {
                        setTrickCard(previousCardPlayerPosition, i);
                        newCurrentCardPosition = previousCardPlayerPosition;
                        previousCardPlayerPosition = newCurrentCardPosition == 0 ? 3 : newCurrentCardPosition - 1;
                    }
                } else {
                    for (int i = 0; i < cards.size(); i++) {
                        setTrickCard(newCurrentCardPosition, i);
                        newCurrentCardPosition = previousCardPlayerPosition;
                        previousCardPlayerPosition = newCurrentCardPosition == 0 ? 3 : newCurrentCardPosition - 1;
                    }
                }

                rotatePointer(currentPlayerPosition);
                break;
            case TRICK_END:
                break;
            case PLAY_END:
                // TODO: По сути ничего не должно происходить
                break;
        }
    }

    private void setCardsInvisible() {
        playerCardImageView.setVisibility(View.INVISIBLE);
        teammateCardImageView.setVisibility(View.INVISIBLE);
        enemyLeftCardImageView.setVisibility(View.INVISIBLE);
        enemyRightCardImageView.setVisibility(View.INVISIBLE);
    }

    private void rotatePointer(PlayerPosition playerTurnPosition) {
        int clientPlayerPosition = newPlayerPositions.get(playerTurnPosition);
        float rotation = playersRotations.get(clientPlayerPosition);
        pointerImageView.setRotation(rotation);
    }

    private void setTrickCard(int clientPlayerPosition) {
        switch (clientPlayerPosition) {
            case 0:
                playerCardImageView.setVisibility(View.VISIBLE);
                playerCardImageView.setImageResource(getCardDrawableId(getLastTricksCard()));
                break;
            case 1:
                enemyLeftCardImageView.setVisibility(View.VISIBLE);
                enemyLeftCardImageView.setImageResource(getCardDrawableId(getLastTricksCard()));
                break;
            case 2:
                teammateCardImageView.setVisibility(View.VISIBLE);
                teammateCardImageView.setImageResource(getCardDrawableId(getLastTricksCard()));
                break;
            case 3:
                enemyRightCardImageView.setVisibility(View.VISIBLE);
                enemyRightCardImageView.setImageResource(getCardDrawableId(getLastTricksCard()));
                break;
        }
    }

    private void setTrickCard(int clientPlayerPosition, int cardIndex) {
        switch (clientPlayerPosition) {
            case 0:
                playerCardImageView.setVisibility(View.VISIBLE);
                playerCardImageView.setImageResource(getCardDrawableId(getTricksCard(cardIndex)));
                break;
            case 1:
                enemyLeftCardImageView.setVisibility(View.VISIBLE);
                enemyLeftCardImageView.setImageResource(getCardDrawableId(getTricksCard(cardIndex)));
                break;
            case 2:
                teammateCardImageView.setVisibility(View.VISIBLE);
                teammateCardImageView.setImageResource(getCardDrawableId(getTricksCard(cardIndex)));
                break;
            case 3:
                enemyRightCardImageView.setVisibility(View.VISIBLE);
                enemyRightCardImageView.setImageResource(getCardDrawableId(getTricksCard(cardIndex)));
                break;
        }
    }

    private Card getLastTricksCard() {
        List<Cards> cardsList = gameState.getTricks();
        List<Card> cards = cardsList.get(cardsList.size() - 1).getCards();
        return cards.get(cards.size() - 1);
    }

    private Card getTricksCard(int index) {
        List<Cards> cardsList = gameState.getTricks();
        List<Card> cards = cardsList.get(cardsList.size() - 1).getCards();
        return cards.get(cards.size() - 1 - index);
    }

    private int getCardDrawableId(Card card) {
        String suitName = suitDrawableNames.get(card.getSuit().getSuitIndex());
        String rankName = rankDrawableNames.get(card.getRank().getRankIndex());
        return getResources().getIdentifier(suitName + "_" + rankName, "drawable",
                getActivity().getPackageName());
    }
}
