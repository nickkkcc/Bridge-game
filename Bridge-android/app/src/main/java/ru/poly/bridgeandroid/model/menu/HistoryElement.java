package ru.poly.bridgeandroid.model.menu;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class HistoryElement implements Parcelable {

    @SerializedName("game_start")
    private final String gameStartTime;

    @SerializedName("game_end")
    private final String gameEndTime;

    @SerializedName("owner_login")
    private final String ownerLogin;

    @SerializedName("owner_alias")
    private final String ownerAlias;

    @SerializedName("winner_team")
    private final int winnerTeam;

    @SerializedName("player_n_login")
    private final String playerNorthLogin;

    @SerializedName("player_n_alias")
    private final String playerNorthAlias;

    @SerializedName("player_s_login")
    private final String playerSouthLogin;

    @SerializedName("player_s_alias")
    private final String playerSouthAlias;

    @SerializedName("player_e_login")
    private final String playerEastLogin;

    @SerializedName("player_e_alias")
    private final String playerEastAlias;

    @SerializedName("player_w_login")
    private final String playerWestLogin;

    @SerializedName("player_w_alias")
    private final String playerWestAlias;

    @SerializedName("total_score_ns")
    private final int totalScoreNorthSouth;

    @SerializedName("total_score_ew")
    private final int totalScoreEastWest;

    public HistoryElement(String gameStartTime, String gameEndTime, String ownerLogin,
                          String ownerAlias, int winnerTeam, String playerNorthLogin,
                          String playerNorthAlias, String playerSouthLogin, String playerSouthAlias,
                          String playerEastLogin, String playerEastAlias, String playerWestLogin,
                          String playerWestAlias, int totalScoreNorthSouth, int totalScoreEastWest) {
        this.gameStartTime = gameStartTime;
        this.gameEndTime = gameEndTime;
        this.ownerLogin = ownerLogin;
        this.ownerAlias = ownerAlias;
        this.winnerTeam = winnerTeam;
        this.playerNorthLogin = playerNorthLogin;
        this.playerNorthAlias = playerNorthAlias;
        this.playerSouthLogin = playerSouthLogin;
        this.playerSouthAlias = playerSouthAlias;
        this.playerEastLogin = playerEastLogin;
        this.playerEastAlias = playerEastAlias;
        this.playerWestLogin = playerWestLogin;
        this.playerWestAlias = playerWestAlias;
        this.totalScoreNorthSouth = totalScoreNorthSouth;
        this.totalScoreEastWest = totalScoreEastWest;
    }

    public HistoryElement(Parcel in) {
        this.gameStartTime = in.readString();
        this.gameEndTime = in.readString();
        this.ownerLogin = in.readString();
        this.ownerAlias = in.readString();
        this.winnerTeam = in.readInt();
        this.playerNorthLogin = in.readString();
        this.playerNorthAlias = in.readString();
        this.playerSouthLogin = in.readString();
        this.playerSouthAlias = in.readString();
        this.playerEastLogin = in.readString();
        this.playerEastAlias = in.readString();
        this.playerWestLogin = in.readString();
        this.playerWestAlias = in.readString();
        this.totalScoreNorthSouth = in.readInt();
        this.totalScoreEastWest = in.readInt();
    }

    public String getGameStartTime() {
        return gameStartTime;
    }

    public String getGameEndTime() {
        return gameEndTime;
    }

    public String getOwnerLogin() {
        return ownerLogin;
    }

    public String getOwnerAlias() {
        return ownerAlias;
    }

    public int getWinnerTeam() {
        return winnerTeam;
    }

    public String getPlayerNorthLogin() {
        return playerNorthLogin;
    }

    public String getPlayerNorthAlias() {
        return playerNorthAlias;
    }

    public String getPlayerSouthLogin() {
        return playerSouthLogin;
    }

    public String getPlayerSouthAlias() {
        return playerSouthAlias;
    }

    public String getPlayerEastLogin() {
        return playerEastLogin;
    }

    public String getPlayerEastAlias() {
        return playerEastAlias;
    }

    public String getPlayerWestLogin() {
        return playerWestLogin;
    }

    public String getPlayerWestAlias() {
        return playerWestAlias;
    }

    public int getTotalScoreNorthSouth() {
        return totalScoreNorthSouth;
    }

    public int getTotalScoreEastWest() {
        return totalScoreEastWest;
    }

    public static final Creator<HistoryElement> CREATOR = new Creator<HistoryElement>() {
        @Override
        public HistoryElement createFromParcel(Parcel in) {
            return new HistoryElement(in);
        }

        @Override
        public HistoryElement[] newArray(int size) {
            return new HistoryElement[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeString(gameStartTime);
        parcel.writeString(gameEndTime);
        parcel.writeString(ownerLogin);
        parcel.writeString(ownerAlias);
        parcel.writeInt(winnerTeam);
        parcel.writeString(playerNorthLogin);
        parcel.writeString(playerNorthAlias);
        parcel.writeString(playerSouthLogin);
        parcel.writeString(playerSouthAlias);
        parcel.writeString(playerEastLogin);
        parcel.writeString(playerEastAlias);
        parcel.writeString(playerWestLogin);
        parcel.writeString(playerWestAlias);
        parcel.writeInt(totalScoreNorthSouth);
        parcel.writeInt(totalScoreEastWest);
    }
}
