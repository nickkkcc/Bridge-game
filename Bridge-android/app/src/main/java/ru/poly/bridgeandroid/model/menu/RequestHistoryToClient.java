package ru.poly.bridgeandroid.model.menu;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;

public class RequestHistoryToClient {

    @SerializedName("history_list")
    private final JsonArray historyList;

    public RequestHistoryToClient(JsonArray historyList) {
        this.historyList = historyList;
    }

    public ArrayList<HistoryElement> getHistoryList() {
        Type listType = new TypeToken<ArrayList<HistoryElement>>(){}.getType();
        return new Gson().fromJson(historyList.toString(), listType);
    }
}
