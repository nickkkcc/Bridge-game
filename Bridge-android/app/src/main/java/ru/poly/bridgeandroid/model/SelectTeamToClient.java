package ru.poly.bridgeandroid.model;

import com.google.gson.annotations.SerializedName;

public class SelectTeamToClient {

    @SerializedName("lobby_id")
    private final String lobbyId;

    @SerializedName("team_1")
    private final int firstTeam;

    @SerializedName("team_2")
    private final int secondTeam;

    public SelectTeamToClient(String lobbyId, int firstTeam, int secondTeam) {
        this.lobbyId = lobbyId;
        this.firstTeam = firstTeam;
        this.secondTeam = secondTeam;
    }

    public String getLobbyId() {
        return lobbyId;
    }

    public int getFirstTeam() {
        return firstTeam;
    }

    public int getSecondTeam() {
        return secondTeam;
    }
}
