package ru.poly.bridgeandroid.model.menu;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class RequestScoreToClient implements Parcelable {

    @SerializedName("score")
    private final double winRate;

    @SerializedName("all_game_count")
    private final int allGamesCount;

    @SerializedName("win_game_count")
    private final int winGamesCount;

    public RequestScoreToClient(double winRate, int allGamesCount, int winGamesCount) {
        this.winRate = winRate;
        this.allGamesCount = allGamesCount;
        this.winGamesCount = winGamesCount;
    }

    protected RequestScoreToClient(Parcel in) {
        winRate = in.readDouble();
        allGamesCount = in.readInt();
        winGamesCount = in.readInt();
    }

    public static final Creator<RequestScoreToClient> CREATOR = new Creator<RequestScoreToClient>() {
        @Override
        public RequestScoreToClient createFromParcel(Parcel in) {
            return new RequestScoreToClient(in);
        }

        @Override
        public RequestScoreToClient[] newArray(int size) {
            return new RequestScoreToClient[size];
        }
    };

    public double getWinRate() {
        return winRate;
    }

    public int getAllGamesCount() {
        return allGamesCount;
    }

    public int getWinGamesCount() {
        return winGamesCount;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeDouble(winRate);
        parcel.writeInt(allGamesCount);
        parcel.writeInt(winGamesCount);
    }
}
