package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class InvitePlayersToServer {

    @SerializedName("login")
    private final String login;

    public InvitePlayersToServer(String login) {
        this.login = login;
    }

    public String getLogin() {
        return login;
    }
}
