package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

public class NotifyBidRejected {

    @SerializedName("reason")
    private final String reason;

    public NotifyBidRejected(String reason) {
        this.reason = reason;
    }

    public String getReason() {
        return reason;
    }
}
