package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class DeleteFriendToServer {

    @SerializedName("login")
    private final String login;

    public DeleteFriendToServer(String login) {
        this.login = login;
    }

    public String getLogin() {
        return login;
    }
}
