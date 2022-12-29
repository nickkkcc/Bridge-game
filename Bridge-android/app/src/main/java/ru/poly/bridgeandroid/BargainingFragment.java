package ru.poly.bridgeandroid;

import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import ru.poly.bridgeandroid.enums.BidCall;
import ru.poly.bridgeandroid.enums.CardSuit;
import ru.poly.bridgeandroid.enums.PlayerPosition;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.Bid;
import ru.poly.bridgeandroid.model.game.BidSelected;
import ru.poly.bridgeandroid.model.game.NotifyRejected;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;

import static android.content.Context.MODE_PRIVATE;

public class BargainingFragment extends Fragment {

    private static final String TOKEN = "token";
    private static final String LOBBY = "lobby";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";

    private static final int ROWS_COUNT = 7;
    private static final int COLUMNS_COUNT = 5;

    private Gson gson;
    private String token;
    private String lobbyId;
    private String login;
    private SharedPreferences sharedPreferences;

    private Button contraButton;
    private Button recontraButton;
    private Button passButton;
    private final List<ImageView> bidsImageView = new ArrayList<>(35);

    private final Map<Integer, String> suitNames = new HashMap<Integer, String>() {{
        put(0, "c");
        put(1, "d");
        put(2, "h");
        put(3, "s");
        put(4, "nt");
    }};

    private PlayerGameState gameState;
    private PlayerPosition mainPlayerPosition;
    private boolean notifyBidTurn;
    private Bid contractBid;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_bargaining, container, false);

        sharedPreferences = this.getActivity().getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        token = sharedPreferences.getString(TOKEN, "");
        lobbyId = sharedPreferences.getString(LOBBY, "");
        login = sharedPreferences.getString(LOGIN, "");

        gson = new Gson();

        contraButton = view.findViewById(R.id.button_contra);
        recontraButton = view.findViewById(R.id.button_recontra);
        passButton = view.findViewById(R.id.button_pass);

        for (int i = 0; i < ROWS_COUNT; i++) {
            for (int j = 0; j < COLUMNS_COUNT; j++) {
                int id = getBidId(i, j);
                bidsImageView.add(view.findViewById(id));
            }
        }

        Bundle bundle = getArguments();
        gameState = bundle.getParcelable("gameState");
        notifyBidTurn = bundle.getBoolean("notifyBidTurn");

        mainPlayerPosition = gameState.getPlayerPositionForLogin(login);

        for (int i = 0; i < bidsImageView.size(); i++) {
            ImageView bidImageView = bidsImageView.get(i);
            final int suitIndex = i % 5;
            final int tricksAbove = i / 5 + 1;
            bidImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    BidCall bidCall = BidCall.BID;
                    CardSuit cardSuit = CardSuit.getCardSuit(suitIndex);
                    Bid currentBid = new Bid(mainPlayerPosition.getPositionIndex(), bidCall.getCallIndex(), cardSuit.getSuitIndex(), tricksAbove);
                    BidSelected bidSelected = new BidSelected(gson.fromJson(gson.toJson(currentBid), JsonObject.class));
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(bidSelected);
                    Message message = new Message(token, "bid_send", jsonObject);
                    EventBus.getDefault().post(gson.toJson(message));
                }
            });
        }

        contraButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BidCall bidCall = BidCall.DOUBLE_BID;
                CardSuit cardSuit = CardSuit.getCardSuit(0);
                Bid currentBid = new Bid(mainPlayerPosition.getPositionIndex(), bidCall.getCallIndex(), cardSuit.getSuitIndex(), 0);
                BidSelected bidSelected = new BidSelected(gson.fromJson(gson.toJson(currentBid), JsonObject.class));
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(bidSelected);
                Message message = new Message(token, "bid_send", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        recontraButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BidCall bidCall = BidCall.REDOUBLE_BID;
                CardSuit cardSuit = CardSuit.getCardSuit(0);
                Bid currentBid = new Bid(mainPlayerPosition.getPositionIndex(), bidCall.getCallIndex(), cardSuit.getSuitIndex(), 0);
                BidSelected bidSelected = new BidSelected(gson.fromJson(gson.toJson(currentBid), JsonObject.class));
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(bidSelected);
                Message message = new Message(token, "bid_send", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        passButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BidCall bidCall = BidCall.PASS;
                CardSuit cardSuit = CardSuit.getCardSuit(0);
                Bid currentBid = new Bid(mainPlayerPosition.getPositionIndex(), bidCall.getCallIndex(), cardSuit.getSuitIndex(), 0);
                BidSelected bidSelected = new BidSelected(gson.fromJson(gson.toJson(currentBid), JsonObject.class));
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(bidSelected);
                Message message = new Message(token, "bid_send", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        passButton.setEnabled(notifyBidTurn);
        contraButton.setEnabled(false);
        recontraButton.setEnabled(false);

        if (notifyBidTurn) {
            setClickableBidsFromIndex(0);
        } else {
            setClickableBidsFromIndex(bidsImageView.size());
        }

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

    public static BargainingFragment newInstance(PlayerGameState playerGameState, boolean notifyBidTurn) {
        BargainingFragment bargainingFragment = new BargainingFragment();

        Bundle args = new Bundle();
        args.putParcelable("gameState", playerGameState);
        args.putBoolean("notifyBidTurn", notifyBidTurn);
        bargainingFragment.setArguments(args);

        return bargainingFragment;
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                gameState = updateGameState.getGameState();
                if (gameState.getCurrentBid().getBidCall().equals(BidCall.BID) ||
                        gameState.getCurrentBid().getBidCall().equals(BidCall.DOUBLE_BID) ||
                        gameState.getCurrentBid().getBidCall().equals(BidCall.REDOUBLE_BID)) {
                    contractBid = gameState.getCurrentBid();
                    Log.i("update_game_state", "contractBid is initialized");
                }
                notifyBidTurn = false;
                getActivity().runOnUiThread(() -> {
                    contraButton.setEnabled(false);
                    recontraButton.setEnabled(false);
                    passButton.setEnabled(false);
                });
                updateFragmentUI();
                break;
            case "notify_bid_turn":
                notifyBidTurn = true;
                if (gameState.getCurrentBid().getTricksAbove() == 0 ||
                        gameState.getCurrentBid().getBidCall().equals(BidCall.PASS) && contractBid == null) {
                    setClickableBidsFromIndex(0);
                } else {
                    Bid currentBid = gameState.getCurrentBid();
                    if (currentBid.getBidCall().equals(BidCall.PASS)) {
                        setClickableBidsFromIndex((contractBid.getTricksAbove() - 1) * COLUMNS_COUNT +
                                contractBid.getCardSuit().getSuitIndex() + 1);
                    } else {
                        setClickableBidsFromIndex((currentBid.getTricksAbove() - 1) * COLUMNS_COUNT +
                                currentBid.getCardSuit().getSuitIndex() + 1);
                    }

                    PlayerPosition teammatePosition = mainPlayerPosition.getPositionIndex() < 2 ?
                            PlayerPosition.getPlayerPosition(mainPlayerPosition.getPositionIndex() + 2) :
                            PlayerPosition.getPlayerPosition(mainPlayerPosition.getPositionIndex() - 2);
                    Log.i("notify_bid_turn", "Change buttons");
                    if (!contractBid.getPlayerPosition().equals(teammatePosition) &&
                            contractBid.getBidCall().equals(BidCall.BID)) {
                        getActivity().runOnUiThread(() -> contraButton.setEnabled(true));
                    }
                    if ((contractBid.getPlayerPosition().equals(teammatePosition) ||
                            contractBid.getPlayerPosition().equals(mainPlayerPosition)) &&
                            contractBid.getBidCall().equals(BidCall.DOUBLE_BID)) {
                        getActivity().runOnUiThread(() -> recontraButton.setEnabled(true));
                    }
                }
                getActivity().runOnUiThread(() -> passButton.setEnabled(true));
                break;
            case "notify_bid_rejected":
                NotifyRejected notifyBidRejected = message.getData(NotifyRejected.class);
                getActivity().runOnUiThread(() -> Toast.makeText(getActivity(), notifyBidRejected.getReason(), Toast.LENGTH_SHORT).show());
                break;
            default:
//                throw new RuntimeException();
        }
    }

    private void updateFragmentUI() {
        setClickableBidsFromIndex(bidsImageView.size());
        switch (gameState.getGameEvent()) {
            case BID_START:
                clearBids();
                break;
            case BID_RESTART:
                clearBids();
                getActivity().runOnUiThread(() -> Toast.makeText(getActivity(), "Все игроки спасовали.", Toast.LENGTH_SHORT).show());
                break;
            case PLAYER_BID:
                Bid currentBid = gameState.getCurrentBid();
                updateBids(currentBid);
                break;
            case BID_END:
                break;
        }
    }

    private void updateBids(Bid currentBid) {
        int currentBidIndex = (currentBid.getTricksAbove() - 1) * COLUMNS_COUNT +
                currentBid.getCardSuit().getSuitIndex();
        switch (currentBid.getBidCall()) {
            case BID:
                getActivity().runOnUiThread(() -> {
                    bidsImageView.get(currentBidIndex).setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_contract_bid));
                });
                break;
            case DOUBLE_BID:
                getActivity().runOnUiThread(() -> {
                    bidsImageView.get(currentBidIndex).setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_contra_bid));
                });
                break;
            case REDOUBLE_BID:
                getActivity().runOnUiThread(() -> {
                    bidsImageView.get(currentBidIndex).setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_recontra_bid));
                });
                break;
            case PASS:
                if (gameState.getCurrentBid().getPlayerPosition().equals(mainPlayerPosition)) {
                    return;
                }
                getActivity().runOnUiThread(() -> Toast.makeText(getActivity(), "Игрок спасовал.", Toast.LENGTH_SHORT).show());
                return;
        }

        for (int i = 0; i < currentBidIndex; i++) {
            int bidIndex = i;
            getActivity().runOnUiThread(() -> {
                ImageView bidImageView = bidsImageView.get(bidIndex);
                bidImageView.setColorFilter(ContextCompat.getColor(getActivity(), R.color.black_overlay_cards));
                if (compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                        getActivity(), R.drawable.table_background_contract_bid))) ||
                        compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                                getActivity(), R.drawable.table_background_contract_bid_darkened)))) {
                    bidImageView.setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_contract_bid_darkened));
                } else if (compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                        getActivity(), R.drawable.table_background_contra_bid))) ||
                        compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                                getActivity(), R.drawable.table_background_contra_bid_darkened)))) {
                    bidImageView.setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_contra_bid_darkened));
                } else if (compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                        getActivity(), R.drawable.table_background_recontra_bid))) ||
                        compareDrawable(bidImageView.getBackground(), Objects.requireNonNull(ContextCompat.getDrawable(
                                getActivity(), R.drawable.table_background_recontra_bid_darkened)))) {
                    bidImageView.setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background_recontra_bid_darkened));
                } else {
                    bidImageView.setBackground(ContextCompat.getDrawable(
                            getActivity(), R.drawable.table_background));
                }
            });
        }
    }

    private void setClickableBidsFromIndex(int index) {
        for (int i = 0; i < index; i++) {
            int bidIndex = i;
            getActivity().runOnUiThread(() -> {
                bidsImageView.get(bidIndex).setClickable(false);
            });
        }
        for (int i = index; i < ROWS_COUNT * COLUMNS_COUNT; i++) {
            int bidIndex = i;
            getActivity().runOnUiThread(() -> {
                bidsImageView.get(bidIndex).setClickable(true);
            });
        }
    }

    private void clearBids() {
        for (ImageView bidImageView : bidsImageView) {
            getActivity().runOnUiThread(() -> {
                bidImageView.clearColorFilter();
                bidImageView.setBackground(ContextCompat.getDrawable(getActivity(), R.drawable.table_background));
            });
        }
    }

    private int getBidId(int row, int column) {
        String suitName = suitNames.get(column);
        String name = "bargaining_" + suitName + "_" + (row + 1);
        return getResources().getIdentifier(name, "id",
                this.getActivity().getPackageName());
    }

    private boolean compareDrawable(Drawable d1, Drawable d2){
        Drawable.ConstantState stateA = d1.getConstantState();
        Drawable.ConstantState stateB = d2.getConstantState();
        // If the constant state is identical, they are using the same drawable resource.
        // However, the opposite is not necessarily true.
        return (stateA != null && stateB != null && stateA.equals(stateB))
                || getBitmap(d1).sameAs(getBitmap(d2));
    }

    private Bitmap getBitmap(Drawable drawable) {
        Bitmap result;
        if (drawable instanceof BitmapDrawable) {
            result = ((BitmapDrawable) drawable).getBitmap();
        } else {
            int width = drawable.getIntrinsicWidth();
            int height = drawable.getIntrinsicHeight();
            // Some drawables have no intrinsic width - e.g. solid colours.
            if (width <= 0) {
                width = 1;
            }
            if (height <= 0) {
                height = 1;
            }

            result = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(result);
            drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            drawable.draw(canvas);
        }
        return result;
    }
}
