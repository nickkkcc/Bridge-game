package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class ForgotPasswordToServer {

    @SerializedName("login")
    private final String login;

    public ForgotPasswordToServer(String login) {
        this.login = login;
    }

    public String getLogin() {
        return login;
    }
}
