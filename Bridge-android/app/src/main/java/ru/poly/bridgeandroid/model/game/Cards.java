package ru.poly.bridgeandroid.model.game;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;

import ru.poly.bridgeandroid.model.menu.Question;

public class Cards {

    @SerializedName("cards")
    private final JsonArray cards;

    public Cards(JsonArray cards) {
        this.cards = cards;
    }

    public ArrayList<Card> getCards() {
        Type listType = new TypeToken<ArrayList<Card>>(){}.getType();
        return new Gson().fromJson(cards.toString(), listType);
    }
}
