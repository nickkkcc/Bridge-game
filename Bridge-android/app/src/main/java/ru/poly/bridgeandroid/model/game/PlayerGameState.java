package ru.poly.bridgeandroid.model.game;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ru.poly.bridgeandroid.enums.GameEvent;
import ru.poly.bridgeandroid.enums.GamePhase;
import ru.poly.bridgeandroid.enums.PlayerPosition;

public class PlayerGameState implements Parcelable {

    @SerializedName("phase")
    private final int phase;

    @SerializedName("rubber_number")
    private final int rubberNumber;

    @SerializedName("game_number")
    private final int gameNumber;

    @SerializedName("deal_number")
    private final int dealNumber;

    @SerializedName("trick_number")
    private final int trickNumber;

    @SerializedName("player_turn")
    private final int playerTurn;

    @SerializedName("hand_to_play")
    private final int handToPlay;

    @SerializedName("dealer")
    private final int dealer;

    @SerializedName("declarer")
    private final int declarer;

    @SerializedName("current_bid")
    private final JsonObject currentBid;

    @SerializedName("contract_bid")
    private final JsonObject contractBid;

    @SerializedName("score")
    private final JsonObject score;

    @SerializedName("game_event")
    private final int gameEvent;

    @SerializedName("player_hand")
    private final JsonObject playerHand;

    @SerializedName("dummy_hand")
    private final JsonObject dummyHand;

    @SerializedName("tricks")
    private final JsonArray tricks;

    @SerializedName("player_position_keys")
    private final int[] playerPositionKeys;

    @SerializedName("player_position_values")
    private final String[] playerPositionValues;

    @SerializedName("player_card_count_keys")
    private final int[] playerCardCountKeys;

    @SerializedName("player_card_count_values")
    private final int[] playerCardCountValues;

    @SerializedName("tricks_won")
    private final int[] tricksWon;

    public PlayerGameState(int phase, int rubberNumber, int gameNumber, int dealNumber,
                           int trickNumber, int playerTurn, int handToPlay, int dealer,
                           int declarer, JsonObject currentBid, JsonObject contractBid,
                           JsonObject score, int gameEvent, JsonObject playerHand,
                           JsonObject dummyHand, JsonArray tricks, int[] playerPositionKeys,
                           String[] playerPositionValues, int[] playerCardCountKeys,
                           int[] playerCardCountValues, int[] tricksWon) {
        this.phase = phase;
        this.rubberNumber = rubberNumber;
        this.gameNumber = gameNumber;
        this.dealNumber = dealNumber;
        this.trickNumber = trickNumber;
        this.playerTurn = playerTurn;
        this.handToPlay = handToPlay;
        this.dealer = dealer;
        this.declarer = declarer;
        this.currentBid = currentBid;
        this.contractBid = contractBid;
        this.score = score;
        this.gameEvent = gameEvent;
        this.playerHand = playerHand;
        this.dummyHand = dummyHand;
        this.tricks = tricks;
        this.playerPositionKeys = playerPositionKeys;
        this.playerPositionValues = playerPositionValues;
        this.playerCardCountKeys = playerCardCountKeys;
        this.playerCardCountValues = playerCardCountValues;
        this.tricksWon = tricksWon;
    }

    protected PlayerGameState(Parcel in) {
        phase = in.readInt();
        rubberNumber = in.readInt();
        gameNumber = in.readInt();
        dealNumber = in.readInt();
        trickNumber = in.readInt();
        playerTurn = in.readInt();
        handToPlay = in.readInt();
        dealer = in.readInt();
        declarer = in.readInt();
        currentBid = new Gson().fromJson(in.readString(), JsonObject.class);
        contractBid = new Gson().fromJson(in.readString(), JsonObject.class);
        score = new Gson().fromJson(in.readString(), JsonObject.class);
        gameEvent = in.readInt();
        playerHand = new Gson().fromJson(in.readString(), JsonObject.class);
        dummyHand = new Gson().fromJson(in.readString(), JsonObject.class);
        tricks = new Gson().fromJson(in.readString(), JsonArray.class);
        playerPositionKeys = in.createIntArray();
        playerPositionValues = in.createStringArray();
        playerCardCountKeys = in.createIntArray();
        playerCardCountValues = in.createIntArray();
        tricksWon = in.createIntArray();
    }

    public static final Creator<PlayerGameState> CREATOR = new Creator<PlayerGameState>() {
        @Override
        public PlayerGameState createFromParcel(Parcel in) {
            return new PlayerGameState(in);
        }

        @Override
        public PlayerGameState[] newArray(int size) {
            return new PlayerGameState[size];
        }
    };

    public GamePhase getPhase() {
        return GamePhase.getGamePhase(phase);
    }

    public int getRubberNumber() {
        return rubberNumber;
    }

    public int getGameNumber() {
        return gameNumber;
    }

    public int getDealNumber() {
        return dealNumber;
    }

    public int getTrickNumber() {
        return trickNumber;
    }

    public PlayerPosition getPlayerTurn() {
        return PlayerPosition.getPlayerPosition(playerTurn);
    }

    public PlayerPosition getHandToPlay() {
        return PlayerPosition.getPlayerPosition(handToPlay);
    }

    public PlayerPosition getDealer() {
        return PlayerPosition.getPlayerPosition(dealer);
    }

    public PlayerPosition getDeclarer() {
        return PlayerPosition.getPlayerPosition(declarer);
    }

    public Bid getCurrentBid() {
        return new Gson().fromJson(currentBid.toString(), Bid.class);
    }

    public Bid getContractBid() {
        return new Gson().fromJson(contractBid.toString(), Bid.class);
    }

    public Score getScore() {
        return new Gson().fromJson(score.toString(), Score.class);
    }

    public GameEvent getGameEvent() {
        return GameEvent.getGameEvent(gameEvent);
    }

    public Cards getPlayerHand() {
        return new Gson().fromJson(playerHand.toString(), Cards.class);
    }

    public Cards getDummyHand() {
        return new Gson().fromJson(dummyHand.toString(), Cards.class);
    }

    public ArrayList<Cards> getTricks() {
        Type listType = new TypeToken<ArrayList<Cards>>(){}.getType();
        return new Gson().fromJson(tricks.toString(), listType);
    }

    public Map<PlayerPosition, String> getPlayerPositionMap() {
        Map<PlayerPosition, String> playerPositionMap = new HashMap<>(4);
        for (int i = 0; i < 4; i++) {
            playerPositionMap.put(PlayerPosition.getPlayerPosition(playerPositionKeys[i]),
                    playerPositionValues[i]);
        }
        return playerPositionMap;
    }

    public PlayerPosition getPlayerPositionForLogin(String login) {
        return getKey(getPlayerPositionMap(), login);
    }

    public Map<PlayerPosition, Integer> getPlayerCardCountMap() {
        Map<PlayerPosition, Integer> playerCardCountMap = new HashMap<>(4);
        for (int i = 0; i < 4; i++) {
            playerCardCountMap.put(PlayerPosition.getPlayerPosition(playerCardCountKeys[i]),
                    playerCardCountValues[i]);
        }
        return playerCardCountMap;
    }

    public Map<PlayerPosition, Integer> getTricksWon() {
        Map<PlayerPosition, Integer> tricksWonMap = new HashMap<>(4);
        for (int i = 0; i < 4; i++) {
            tricksWonMap.put(PlayerPosition.getPlayerPosition(i), tricksWon[i]);
        }
        return tricksWonMap;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeInt(phase);
        parcel.writeInt(rubberNumber);
        parcel.writeInt(gameNumber);
        parcel.writeInt(dealNumber);
        parcel.writeInt(trickNumber);
        parcel.writeInt(playerTurn);
        parcel.writeInt(handToPlay);
        parcel.writeInt(dealer);
        parcel.writeInt(declarer);
        parcel.writeString(currentBid.toString());
        parcel.writeString(contractBid.toString());
        parcel.writeString(score.toString());
        parcel.writeInt(gameEvent);
        parcel.writeString(playerHand.toString());
        parcel.writeString(dummyHand.toString());
        parcel.writeString(tricks.toString());
        parcel.writeIntArray(playerPositionKeys);
        parcel.writeStringArray(playerPositionValues);
        parcel.writeIntArray(playerCardCountKeys);
        parcel.writeIntArray(playerCardCountValues);
        parcel.writeIntArray(tricksWon);
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
