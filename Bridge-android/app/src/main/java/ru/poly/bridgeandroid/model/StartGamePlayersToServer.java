package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class StartGamePlayersToServer {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("successful")
    private final boolean isSuccessful;

    public StartGamePlayersToServer(String lobbyId, boolean isSuccessful) {
        this.lobbyId = lobbyId;
        this.isSuccessful = isSuccessful;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }
}
