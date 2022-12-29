package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

public class NotifyRejected {

    @SerializedName("reason")
    private final String reason;

    public NotifyRejected(String reason) {
        this.reason = reason;
    }

    public String getReason() {
        return reason;
    }
}
