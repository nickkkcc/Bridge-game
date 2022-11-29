package ru.poly.bridgeandroid.model;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

public class Message {

    @SerializedName("id")
    private final String id;

    @SerializedName("type")
    private final String type;

    @SerializedName("data")
    private final JsonObject data;

    public Message(String id, String type, JsonObject data) {
        this.id = id;
        this.type = type;
        this.data = data;
    }

    public Message(String id, String type) {
        this.id = id;
        this.type = type;
        this.data = new JsonObject();
    }

    public String getId() {
        return id;
    }

    public String getType() {
        return type;
    }

    public JsonObject getData() {
        return data;
    }

    public <T> T getData(Class<T> type) {
        return new Gson().fromJson(data.toString(), type);
    }
}
