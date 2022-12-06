package ru.poly.bridgeandroid.model;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class Player implements Parcelable {

    @SerializedName("login")
    private final String login;

    @SerializedName("alias")
    private final String alias;

    public Player(String login, String alias) {
        this.login = login;
        this.alias = alias;
    }

    protected Player(Parcel in) {
        login = in.readString();
        alias = in.readString();
    }

    public static final Creator<Player> CREATOR = new Creator<Player>() {
        @Override
        public Player createFromParcel(Parcel in) {
            return new Player(in);
        }

        @Override
        public Player[] newArray(int size) {
            return new Player[size];
        }
    };

    public String getLogin() {
        return login;
    }

    public String getAlias() {
        return alias;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeString(login);
        parcel.writeString(alias);
    }
}
