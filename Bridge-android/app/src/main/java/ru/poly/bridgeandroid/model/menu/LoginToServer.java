package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class LoginToServer {

    @SerializedName("login")
    private final String login;

    @SerializedName("password")
    private final String password;

    public LoginToServer(String login, String password) {
        this.login = login;
        this.password = password;
    }

    public String getLogin() {
        return login;
    }

    public String getPassword() {
        return password;
    }
}
