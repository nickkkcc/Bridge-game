package ru.poly.bridgeandroid;

import android.os.Bundle;
import android.text.Html;
import android.text.SpannableStringBuilder;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.drawerlayout.widget.DrawerLayout;

public class GameActivity extends AppCompatActivity {
    public DrawerLayout drawerLayout;
    public ActionBarDrawerToggle actionBarDrawerToggleMenu;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_game);

        hideSystemBars();

        drawerLayout = findViewById(R.id.game_drawer_layout);
        actionBarDrawerToggleMenu = new ActionBarDrawerToggle(this, drawerLayout,
                R.string.nav_open, R.string.nav_close);

        drawerLayout.addDrawerListener(actionBarDrawerToggleMenu);
        actionBarDrawerToggleMenu.syncState();

        Toolbar toolbar = findViewById(R.id.game_toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeAsUpIndicator(R.drawable.ic_baseline_menu_24);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setDisplayShowTitleEnabled(false);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // R.menu.mymenu is a reference to an xml file named mymenu.xml which should be inside your res/menu directory.
        // If you don't have res/menu, just create a directory named "menu" inside res
        getMenuInflater().inflate(R.menu.game_toolbar_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.game_toolbar_chat:
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                View popupChatView = getLayoutInflater().inflate(R.layout.popup_chat, null);
                TextView popupChatTextView = popupChatView.findViewById(R.id.popup_chat_text);

                String chatString = "<b>Алёша</b>: Всем привет!<br>" +
                        "<b>Серёжа</b>: Привет!<br>" +
                        "<b>Олег</b>: Привет, мы готовы.<br>" +
                        "<b>Серёжа</b>: Мы победим :)<br>" +
                        "<b>Гена</b>: Удачи!<br>" +
                        "<b>Серёжа</b>: У лукоморья дуб зелёный;<br>" +
                        "Златая цепь на дубе том:<br>" +
                        "И днём и ночью кот учёный<br>" +
                        "Всё ходит по цепи кругом;<br>" +
                        "Идёт направо — песнь заводит,<br>" +
                        "Налево — сказку говорит.<br>" +
                        "Там чудеса: там леший бродит,<br>" +
                        "Русалка на ветвях сидит;<br>" +
                        "Там на неведомых дорожках<br>" +
                        "Следы невиданных зверей;<br>" +
                        "Избушка там на курьих ножках<br>" +
                        "Стоит без окон, без дверей;<br>" +
                        "Там лес и дол видений полны;<br>" +
                        "Там о заре прихлынут волны<br>" +
                        "На брег песчаный и пустой,<br>" +
                        "И тридцать витязей прекрасных<br>" +
                        "Чредой из вод выходят ясных,<br>" +
                        "И с ними дядька их морской;<br>" +
                        "Там королевич мимоходом<br>" +
                        "Пленяет грозного царя;<br>" +
                        "Там в облаках перед народом<br>" +
                        "Через леса, через моря<br>" +
                        "Колдун несёт богатыря;<br>" +
                        "В темнице там царевна тужит,<br>" +
                        "А бурый волк ей верно служит;<br>" +
                        "Там ступа с Бабою Ягой<br>" +
                        "Идёт, бредёт сама собой,<br>" +
                        "Там царь Кащей над златом чахнет;<br>" +
                        "Там русский дух… там Русью пахнет!<br>" +
                        "И там я был, и мёд я пил;<br>" +
                        "У моря видел дуб зелёный;<br>" +
                        "Под ним сидел, и кот учёный<br>" +
                        "Свои мне сказки говорил.";
                popupChatTextView.setText(Html.fromHtml(chatString));

                builder.setView(popupChatView);
                AlertDialog dialog = builder.create();
                dialog.show();
        }
        if (actionBarDrawerToggleMenu.onOptionsItemSelected(item)) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }
}
