package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class JoinToClient {

    @SerializedName("successful")
    private final boolean isSuccessful;

    public JoinToClient(boolean isSuccessful) {
        this.isSuccessful = isSuccessful;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }
}
