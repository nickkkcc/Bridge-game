package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class LoginToServer {
    @SerializedName("ID")
    private int id;

    @SerializedName("Type")
    private String type;

    @SerializedName("Password")
    private String password;

    @SerializedName("login")
    private String login;

    public LoginToServer(int id, String type, String password, String login) {
        this.id = id;
        this.type = type;
        this.password = password;
        this.login = login;
    }

    public int getId() {
        return id;
    }

    public String getType() {
        return type;
    }

    public String getPassword() {
        return password;
    }

    public String getLogin() {
        return login;
    }
}
