package ru.poly.bridgeandroid.enums;

public enum BidCall {

    PASS(0),
    BID(1),
    DOUBLE_BID(2),
    REDOUBLE_BID(3);

    private final int callIndex;

    BidCall(int callIndex) {
        this.callIndex = callIndex;
    }

    public int getCallIndex() {
        return callIndex;
    }

    public static BidCall getBidCall(int callIndex) {
        for (BidCall bidCall : values()) {
            if (bidCall.callIndex == callIndex) {
                return bidCall;
            }
        }
        return null;
    }
}
