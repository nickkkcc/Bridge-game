package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class InvitePlayersToServer {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("login")
    private final String login;

    public InvitePlayersToServer(String lobbyId, String login) {
        this.lobbyId = lobbyId;
        this.login = login;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public String getLogin() {
        return login;
    }
}
