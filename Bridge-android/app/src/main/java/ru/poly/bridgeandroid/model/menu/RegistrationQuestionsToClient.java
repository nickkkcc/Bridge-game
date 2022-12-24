package ru.poly.bridgeandroid.model.menu;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;

public class RegistrationQuestionsToClient {

    @SerializedName("questions")
    private final JsonArray questions;

    public RegistrationQuestionsToClient(JsonArray questions) {
        this.questions = questions;
    }

    public ArrayList<Question> getQuestions() {
        Type listType = new TypeToken<ArrayList<Question>>(){}.getType();
        return new Gson().fromJson(questions.toString(), listType);
    }
}
