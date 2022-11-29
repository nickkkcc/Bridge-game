package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class AcceptSelectTeamToClient {

    @SerializedName("lobby_id")
    private final int lobbyId;

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    public AcceptSelectTeamToClient(int lobbyId, boolean isSuccessful, String error) {
        this.lobbyId = lobbyId;
        this.isSuccessful = isSuccessful;
        this.error = error;
    }

    public int getLobbyId() {
        return lobbyId;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }
}
