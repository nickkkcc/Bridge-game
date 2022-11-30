package ru.poly.bridgeandroid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class ChoosePlaceActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_place);

        final Button northSouth = findViewById(R.id.NS);
        final Button eastWest = findViewById(R.id.EW);

        northSouth.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*Intent intent = new Intent(ChoosePlaceActivity.this, GameActivity.class);
                startActivity(intent);
                finish();*/
                FragmentManager manager = getSupportFragmentManager();
                WaitGameFragment waitGame = new WaitGameFragment();
                waitGame.show(manager, "WaitGameDialog");
            }
        });
        eastWest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*Intent intent = new Intent(ChoosePlaceActivity.this, GameActivity.class);
                startActivity(intent);
                finish();*/
                FragmentManager manager = getSupportFragmentManager();
                WaitGameFragment waitGame = new WaitGameFragment();
                waitGame.show(manager, "WaitGameDialog");
            }
        });
    }
}