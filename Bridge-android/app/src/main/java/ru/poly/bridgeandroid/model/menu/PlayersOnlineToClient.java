package ru.poly.bridgeandroid.model.menu;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;

public class PlayersOnlineToClient {

    @SerializedName("friends")
    private final JsonArray friends;

    @SerializedName("players")
    private final JsonArray players;

    public PlayersOnlineToClient(JsonArray friends, JsonArray players) {
        this.friends = friends;
        this.players = players;
    }

    public ArrayList<Player> getFriends() {
        Type listType = new TypeToken<ArrayList<Player>>(){}.getType();
        return new Gson().fromJson(friends.toString(), listType);
    }

    public ArrayList<Player> getPlayers() {
        Type listType = new TypeToken<ArrayList<Player>>(){}.getType();
        return new Gson().fromJson(players.toString(), listType);
    }
}
