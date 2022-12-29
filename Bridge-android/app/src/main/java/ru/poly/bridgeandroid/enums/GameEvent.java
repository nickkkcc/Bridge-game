package ru.poly.bridgeandroid.enums;

public enum GameEvent {

    INITIALIZE(0),
    BID_START(1),
    BID_RESTART(2),
    PLAYER_BID(3),
    BID_END(4),
    PLAY_START(5),
    TRICK_START(6),
    PLAYER_MOVED(7),
    TRICK_END(8),
    PLAY_END(9),
    MATCH_END(10),
    RUBBER_COMPLETED(11),
    PLAY_STOP(12),
    PLAY_CONTINUES(13);

    private final int eventIndex;

    GameEvent(int eventIndex) {
        this.eventIndex = eventIndex;
    }

    public int getEventIndex() {
        return eventIndex;
    }

    public static GameEvent getGameEvent(int eventIndex) {
        for (GameEvent gameEvent : values()) {
            if (gameEvent.eventIndex == eventIndex) {
                return gameEvent;
            }
        }
        return null;
    }
}
