package ru.poly.bridgeandroid.ui.login;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import ru.poly.bridgeandroid.GameActivity;
import ru.poly.bridgeandroid.MenuActivity;
import ru.poly.bridgeandroid.R;
import ru.poly.bridgeandroid.enums.GameEvent;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.LoginToClient;
import ru.poly.bridgeandroid.model.menu.LoginToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.RegistrationQuestionsToClient;

public class LoginActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private Gson gson;
    private String login;
    private LoginViewModel loginViewModel;
    private EditText usernameEditText;
    private ProgressBar loadingProgressBar;
    private boolean isStartGame;
    private Intent gameIntent;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        usernameEditText = findViewById(R.id.username);
        final EditText passwordEditText = findViewById(R.id.password);
        final Button loginButton = findViewById(R.id.login);
        final Button registrationButton = findViewById(R.id.registration);
        loadingProgressBar = findViewById(R.id.loading);

        loginViewModel.getLoginFormState().observe(this, new Observer<LoginFormState>() {
            @Override
            public void onChanged(@Nullable LoginFormState loginFormState) {
                if (loginFormState == null) {
                    return;
                }
                loginButton.setEnabled(loginFormState.isDataValid());
                if (loginFormState.getUsernameError() != null) {
                    usernameEditText.setError(getString(loginFormState.getUsernameError()));
                }
                if (loginFormState.getPasswordError() != null) {
                    passwordEditText.setError(getString(loginFormState.getPasswordError()));
                }
            }
        });

        loginViewModel.getLoginResult().observe(this, new Observer<LoginResult>() {
            @Override
            public void onChanged(@Nullable LoginResult loginResult) {
                if (loginResult == null) {
                    return;
                }
                loadingProgressBar.setVisibility(View.GONE);
                if (loginResult.getError() != null) {
                    showLoginFailed(loginResult.getError());
                }
                if (loginResult.getSuccess() != null) {
                    updateUiWithUser(loginResult.getSuccess());
                }
                setResult(Activity.RESULT_OK);

                //Complete and destroy login activity once successful
                finish();
            }
        });

        TextWatcher afterTextChangedListener = new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                // ignore
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                // ignore
            }

            @Override
            public void afterTextChanged(Editable s) {
                loginViewModel.loginDataChanged(usernameEditText.getText().toString(),
                        passwordEditText.getText().toString());
            }
        };
        usernameEditText.addTextChangedListener(afterTextChangedListener);
        passwordEditText.addTextChangedListener(afterTextChangedListener);
        passwordEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {

            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if (actionId == EditorInfo.IME_ACTION_DONE) {
                    loginViewModel.login(usernameEditText.getText().toString(),
                            passwordEditText.getText().toString());
                }
                return false;
            }
        });

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadingProgressBar.setVisibility(View.VISIBLE);
                loginViewModel.login(usernameEditText.getText().toString(),
                        passwordEditText.getText().toString());

                LoginToServer loginToServer = new LoginToServer(usernameEditText.getText().toString(),
                        passwordEditText.getText().toString());
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(loginToServer);
                Message message = new Message("0", "login", jsonObject);
                EventBus.getDefault().post(gson.toJson(message));
            }
        });

        registrationButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Message message = new Message("0", "registration_questions");
                EventBus.getDefault().post(gson.toJson(message));
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    private void updateUiWithUser(LoggedInUserView model) {
        String welcome = getString(R.string.welcome) + model.getDisplayName();
        // TODO : initiate successful logged in experience
        Toast.makeText(getApplicationContext(), welcome, Toast.LENGTH_LONG).show();
    }

    private void showLoginFailed(@StringRes Integer errorString) {
        Toast.makeText(getApplicationContext(), errorString, Toast.LENGTH_SHORT).show();
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "registration_questions":
                RegistrationQuestionsToClient questions = message.getData(
                        RegistrationQuestionsToClient.class);
                Intent intent = new Intent(LoginActivity.this, RegistrationActivity.class);
                intent.putParcelableArrayListExtra("questions", questions.getQuestions());
                startActivity(intent);
                finish();
                break;
            case "login":
                runOnUiThread(() -> loadingProgressBar.setVisibility(View.GONE));

                LoginToClient loginToClient = message.getData(LoginToClient.class);
                if (loginToClient.isSuccessful()) {
                    login = usernameEditText.getText().toString();
                    SharedPreferences sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString(TOKEN, loginToClient.getToken());
                    editor.putString(LOGIN, login);
                    editor.apply();

                    Intent intentMenu = new Intent(LoginActivity.this, MenuActivity.class);
                    startActivity(intentMenu);
                    finish();

                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(),
                                "Вы успешно авторизовались", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), loginToClient.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "update_game_state":
                UpdateGameState updateGameState = message.getData(UpdateGameState.class);
                PlayerGameState gameState = updateGameState.getGameState();
                if (gameState.getGameEvent().equals(GameEvent.PLAY_CONTINUES)) {
                    isStartGame = true;
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "Игра продолжается.", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                    return;
                }
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "update_game_state error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                gameIntent = new Intent(LoginActivity.this, GameActivity.class);
                gameIntent.putExtra("gameState", gameState);
                if (!gameState.getPlayerTurn().equals(gameState.getPlayerPositionForLogin(login))) {
                    startActivity(gameIntent);
                    finish();
                }
                break;
            case "notify_bid_turn":
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "notify_bid_turn error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                gameIntent.putExtra("notifyBidTurn", true);
                startActivity(gameIntent);
                finish();
                break;
            default:
                throw new RuntimeException();
        }
    }
}