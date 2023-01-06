package ru.poly.bridgeandroid.model.menu;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class ForgotPasswordToClient implements Parcelable {

    @SerializedName("login")
    private final String login;

    @SerializedName("successful")
    private final boolean isSuccessful;

    @SerializedName("error")
    private final String error;

    @SerializedName("question")
    private final String question;

    public ForgotPasswordToClient(String login, boolean isSuccessful, String error, String question) {
        this.login = login;
        this.isSuccessful = isSuccessful;
        this.error = error;
        this.question = question;
    }

    protected ForgotPasswordToClient(Parcel in) {
        login = in.readString();
        isSuccessful = in.readByte() != 0;
        error = in.readString();
        question = in.readString();
    }

    public static final Creator<ForgotPasswordToClient> CREATOR = new Creator<ForgotPasswordToClient>() {
        @Override
        public ForgotPasswordToClient createFromParcel(Parcel in) {
            return new ForgotPasswordToClient(in);
        }

        @Override
        public ForgotPasswordToClient[] newArray(int size) {
            return new ForgotPasswordToClient[size];
        }
    };

    public String getLogin() {
        return login;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public String getError() {
        return error;
    }

    public String getQuestion() {
        return question;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeString(login);
        parcel.writeByte((byte) (isSuccessful ? 1 : 0));
        parcel.writeString(error);
        parcel.writeString(question);
    }
}
