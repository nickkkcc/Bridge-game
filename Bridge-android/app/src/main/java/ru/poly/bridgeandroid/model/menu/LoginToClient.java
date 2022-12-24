package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class LoginToClient {

    @SerializedName("token")
    private final String token;

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    public LoginToClient(String token, boolean isSuccessful, String error) {
        this.token = token;
        this.isSuccessful = isSuccessful;
        this.error = error;
    }

    public String getToken() {
        return token;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }
}
