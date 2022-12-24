package ru.poly.bridgeandroid;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import ru.poly.bridgeandroid.R;
import ru.poly.bridgeandroid.model.game.PlayerGameState;

public class DrawFragment extends Fragment {
//    public DrawFragment() {
//        super(R.layout.fragment_draw);
//    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_draw, container, false);
        return view;
    }

    public static DrawFragment newInstance(PlayerGameState playerGameState) {
        DrawFragment drawFragment = new DrawFragment();

        Bundle args = new Bundle();
        args.putParcelable("gameState", playerGameState);
        drawFragment.setArguments(args);

        return drawFragment;
    }
}
