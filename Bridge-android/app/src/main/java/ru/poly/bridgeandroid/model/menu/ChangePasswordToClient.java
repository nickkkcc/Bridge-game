package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class ChangePasswordToClient {

    @SerializedName("login")
    private final String login;

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    public ChangePasswordToClient(String login, boolean isSuccessful, String error) {
        this.login = login;
        this.isSuccessful = isSuccessful;
        this.error = error;
    }

    public String getLogin() {
        return login;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }
}
