package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class AcceptInvitePlayersToServer {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("successful")
    private final boolean isSuccessful;

    public AcceptInvitePlayersToServer(String lobbyId, boolean isSuccessful) {
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
