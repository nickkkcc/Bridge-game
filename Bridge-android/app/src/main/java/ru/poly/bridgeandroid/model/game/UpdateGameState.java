package ru.poly.bridgeandroid.model.game;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

import org.json.JSONObject;

public class UpdateGameState {

    @SerializedName("game_state")
    private final JsonObject gameState;

    public UpdateGameState(JsonObject gameState) {
        this.gameState = gameState;
    }

    public PlayerGameState getGameState() {
        return new Gson().fromJson(gameState.toString(), PlayerGameState.class);
    }
}
