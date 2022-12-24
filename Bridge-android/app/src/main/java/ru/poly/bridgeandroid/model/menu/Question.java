package ru.poly.bridgeandroid.model.menu;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class Question implements Parcelable {

    @SerializedName("question_id")
    private final int questionId;

    @SerializedName("question")
    private final String question;

    public Question(int questionId, String question) {
        this.questionId = questionId;
        this.question = question;
    }

    protected Question(Parcel in) {
        questionId = in.readInt();
        question = in.readString();
    }

    public static final Creator<Question> CREATOR = new Creator<Question>() {
        @Override
        public Question createFromParcel(Parcel in) {
            return new Question(in);
        }

        @Override
        public Question[] newArray(int size) {
            return new Question[size];
        }
    };

    public int getQuestionId() {
        return questionId;
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
        parcel.writeInt(questionId);
        parcel.writeString(question);
    }
}
