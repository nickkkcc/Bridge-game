package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class RegistrationToServer {

    @SerializedName("login")
    private final String login;

    @SerializedName("password")
    private final String password;

    @SerializedName("question")
    private final int question;

    @SerializedName("answer")
    private final String answer;

    public RegistrationToServer(String login, String password, int question, String answer) {
        this.login = login;
        this.password = password;
        this.question = question;
        this.answer = answer;
    }

    public String getLogin() {
        return login;
    }

    public String getPassword() {
        return password;
    }

    public int getQuestion() {
        return question;
    }

    public String getAnswer() {
        return answer;
    }
}
