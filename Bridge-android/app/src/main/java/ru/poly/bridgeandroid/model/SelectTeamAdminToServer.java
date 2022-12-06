package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class SelectTeamAdminToServer {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("team")
    private final int team;

    public SelectTeamAdminToServer(String lobbyId, int team) {
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
