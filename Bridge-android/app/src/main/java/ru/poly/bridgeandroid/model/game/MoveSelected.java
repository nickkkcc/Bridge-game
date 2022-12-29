package ru.poly.bridgeandroid.model.game;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

public class MoveSelected {

    @SerializedName("card")
    private final JsonObject card;

    public MoveSelected(JsonObject card) {
        this.card = card;
    }

    public Card getCard() {
        return new Gson().fromJson(card.toString(), Card.class);
    }
}
