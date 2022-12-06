package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class PlayersCountLobbyToClient {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("count")
    private final int playersInLobby;

    @SerializedName("error")
    private final String error;

    public PlayersCountLobbyToClient(String lobbyId, int playersInLobby, String error) {
        this.lobbyId = lobbyId;
        this.playersInLobby = playersInLobby;
        this.error = error;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public int getPlayersInLobby() {
        return playersInLobby;
    }

    public String getError() {
        return error;
    }
}
