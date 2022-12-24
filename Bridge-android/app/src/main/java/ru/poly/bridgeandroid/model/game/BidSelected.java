package ru.poly.bridgeandroid.model.game;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

public class BidSelected {

    @SerializedName("bid")
    private final JsonObject bid;

    public BidSelected(JsonObject bid) {
        this.bid = bid;
    }

    public Bid getBid() {
        return new Gson().fromJson(bid.toString(), Bid.class);
    }
}
