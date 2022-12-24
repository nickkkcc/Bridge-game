package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class InvitePlayersToClient {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("alias")
    private final String alias;

    public InvitePlayersToClient(String lobbyId, String alias) {
        this.lobbyId = lobbyId;
        this.alias = alias;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public String getAlias() {
        return alias;
    }
}
