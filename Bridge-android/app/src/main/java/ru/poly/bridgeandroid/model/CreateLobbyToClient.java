package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class CreateLobbyToClient {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    public CreateLobbyToClient(String lobbyId, boolean isSuccessful, String error) {
        this.lobbyId = lobbyId;
        this.isSuccessful = isSuccessful;
        this.error = error;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }
}
