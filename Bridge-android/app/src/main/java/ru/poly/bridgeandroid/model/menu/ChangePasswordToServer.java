package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class ChangePasswordToServer {

    @SerializedName("login")
    private final String login;

    @SerializedName("new_password")
    private final String newPassword;

    public ChangePasswordToServer(String login, String newPassword) {
        this.login = login;
        this.newPassword = newPassword;
    }

    public String getLogin() {
        return login;
    }

    public String getNewPassword() {
        return newPassword;
    }
}
