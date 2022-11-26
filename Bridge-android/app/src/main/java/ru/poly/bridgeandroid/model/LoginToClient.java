package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class LoginToClient {
    @SerializedName("ID")
    private int id;

    @SerializedName("Type")
    private String type;

    @SerializedName("loginSuccessful")
    private boolean loginSuccessful;

    @SerializedName("reason")
    private String reason;

    public LoginToClient(int id, String type, boolean loginSuccessful, String reason) {
        this.id = id;
        this.type = type;
        this.loginSuccessful = loginSuccessful;
        this.reason = reason;
    }

    public int getId() {
        return id;
    }

    public String getType() {
        return type;
    }

    public boolean isLoginSuccessful() {
        return loginSuccessful;
    }

    public String getReason() {
        return reason;
    }
}
