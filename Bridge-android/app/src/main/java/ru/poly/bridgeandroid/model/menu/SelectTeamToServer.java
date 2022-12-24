package ru.poly.bridgeandroid.model.menu;

import com.google.gson.annotations.SerializedName;

public class SelectTeamToServer {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("team")
    private final int team;

    public SelectTeamToServer(String lobbyId, int team) {
        this.lobbyId = lobbyId;
        this.team = team;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public int getTeam() {
        return team;
    }
}
