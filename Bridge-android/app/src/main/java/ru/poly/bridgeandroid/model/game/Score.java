package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

public class Score {

    @SerializedName("contract_points")
    private final int[][] contractPoints;

    @SerializedName("back_score")
    private final int[] backScore;

    @SerializedName("overtricks")
    private final int[] overtricks;

    @SerializedName("undertricks")
    private final int[] undertricks;

    @SerializedName("honors")
    private final int[] honors;

    @SerializedName("slam_bonuses")
    private final int[] slamBonuses;

    @SerializedName("double_bonuses")
    private final int[] doubleBonuses;

    @SerializedName("redouble_bonuses")
    private final int[] redoubleBonuses;

    @SerializedName("rubber_bonuses")
    private final int[] rubberBonuses;

    public Score(int[][] contractPoints, int[] backScore, int[] overtricks, int[] undertricks,
                 int[] honors, int[] slamBonuses, int[] doubleBonuses, int[] redoubleBonuses,
                 int[] rubberBonuses) {
        this.contractPoints = contractPoints;
        this.backScore = backScore;
        this.overtricks = overtricks;
        this.undertricks = undertricks;
        this.honors = honors;
        this.slamBonuses = slamBonuses;
        this.doubleBonuses = doubleBonuses;
        this.redoubleBonuses = redoubleBonuses;
        this.rubberBonuses = rubberBonuses;
    }

    public int[][] getContractPoints() {
        return contractPoints;
    }

    public int[] getBackScore() {
        return backScore;
    }

    public int[] getOvertricks() {
        return overtricks;
    }

    public int[] getUndertricks() {
        return undertricks;
    }

    public int[] getHonors() {
        return honors;
    }

    public int[] getSlamBonuses() {
        return slamBonuses;
    }

    public int[] getDoubleBonuses() {
        return doubleBonuses;
    }

    public int[] getRedoubleBonuses() {
        return redoubleBonuses;
    }

    public int[] getRubberBonuses() {
        return rubberBonuses;
    }

    public int getAllScores(int teamIndex) {
        return sumArray(contractPoints[teamIndex]) + backScore[teamIndex] + overtricks[teamIndex] +
                undertricks[teamIndex] + honors[teamIndex] + slamBonuses[teamIndex] +
                doubleBonuses[teamIndex] + redoubleBonuses[teamIndex] + rubberBonuses[teamIndex];
    }

    public int getBonusScores(int teamIndex) {
        return backScore[teamIndex] + overtricks[teamIndex] + undertricks[teamIndex] +
                honors[teamIndex] + slamBonuses[teamIndex] + doubleBonuses[teamIndex] +
                redoubleBonuses[teamIndex] + rubberBonuses[teamIndex];
    }

    private int sumArray(int[] array) {
        int sum = 0;
        for (int value : array) {
            sum += value;
        }
        return sum;
    }
}
