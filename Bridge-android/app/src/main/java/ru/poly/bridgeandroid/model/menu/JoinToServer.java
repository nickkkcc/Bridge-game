package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class JoinToServer {

    @SerializedName("join")
    private final boolean readyToJoin;

    public JoinToServer(boolean readyToJoin) {
        this.readyToJoin = readyToJoin;
    }

    public boolean isReadyToJoin() {
        return readyToJoin;
    }
}
