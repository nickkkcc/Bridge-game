package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class AddFriendToServer {

    @SerializedName("login")
    private final String login;

    public AddFriendToServer(String login) {
        this.login = login;
    }

    public String getLogin() {
        return login;
    }
}
