package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class RegistrationToClient {

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    public RegistrationToClient(boolean isSuccessful, String error) {
        this.isSuccessful = isSuccessful;
        this.error = error;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }
}
