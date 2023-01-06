package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class SecretQuestionToServer {

    @SerializedName("login")
    private final String login;

    @SerializedName("answer")
    private final String answer;

    public SecretQuestionToServer(String login, String answer) {
        this.login = login;
        this.answer = answer;
    }

    public String getLogin() {
        return login;
    }

    public String getAnswer() {
        return answer;
    }
}
