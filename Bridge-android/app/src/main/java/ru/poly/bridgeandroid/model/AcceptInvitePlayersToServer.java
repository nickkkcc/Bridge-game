package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class AcceptInvitePlayersToServer {

    @SerializedName("lobby_id")
    private final int lobbyId;

    @SerializedName("successful")
    private final boolean isSuccessful;

    public AcceptInvitePlayersToServer(int lobbyId, boolean isSuccessful) {
        this.lobbyId = lobbyId;
        this.isSuccessful = isSuccessful;
    }

    public int getLobbyId() {
        return lobbyId;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }
}
