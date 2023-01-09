package ru.poly.bridgeandroid.ui.login;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
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
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.LoginToClient;
import ru.poly.bridgeandroid.model.menu.LoginToServer;
import ru.poly.bridgeandroid.model.menu.RegistrationQuestionsToClient;

public class LoginActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOGIN = "login";
    private static final String PASSWORD = "password";
    private static final String RESTART = "restart";
    private static final String RESTART_GAME = "restartGame";
    private static final String PREFERENCE = "preference";
    private SharedPreferences sharedPreferences;
    private Gson gson;
    private LoginViewModel loginViewModel;
    private EditText usernameEditText;
    private EditText passwordEditText;
    private ProgressBar loadingProgressBar;
    private boolean isStartGame;
    private Intent gameIntent;

    private long backPressedTime;
    private Toast backToast;

    private boolean isRestart;
    private boolean isRestartGame;

    private CountDownTimer timer;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
        isRestart = sharedPreferences.getBoolean(RESTART, false);
        isRestartGame = sharedPreferences.getBoolean(RESTART_GAME, false);

        Log.i("isRestart", String.valueOf(isRestart));
        Log.i("isRestartGame", String.valueOf(isRestartGame));
//
//        SharedPreferences.Editor editor = sharedPreferences.edit();
//        editor.putBoolean(RESTART, false);
//        editor.putBoolean(RESTART_GAME, false);
//        editor.apply();

        usernameEditText = findViewById(R.id.username);
        passwordEditText = findViewById(R.id.password);
        final Button loginButton = findViewById(R.id.login);
        final Button registrationButton = findViewById(R.id.registration);
        TextView forgotPasswordTextView = findViewById(R.id.forgot_password);
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

        forgotPasswordTextView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intentMenu = new Intent(LoginActivity.this, ForgotPasswordActivity.class);
                startActivity(intentMenu);
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
//        if (isRestart || isRestartGame) {
//            login = sharedPreferences.getString(LOGIN, "");
//            String password = sharedPreferences.getString(PASSWORD, "");
//
//            LoginToServer loginToServer = new LoginToServer(login, password);
//            JsonObject jsonObject = (JsonObject) gson.toJsonTree(loginToServer);
//            Message message = new Message("0", "login", jsonObject);
//            EventBus.getDefault().post(gson.toJson(message));
//        }
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    @Override
    public void onBackPressed() {
        if (backPressedTime + 2000 > System.currentTimeMillis()) {
            backToast.cancel();
            super.onBackPressed();
            return;
        } else {
            backToast = Toast.makeText(getBaseContext(), "Нажмите ещё раз, чтобы выйти", Toast.LENGTH_SHORT);
            backToast.show();
        }
        backPressedTime = System.currentTimeMillis();
    }

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        runOnUiThread(() -> loadingProgressBar.setVisibility(View.GONE));
        switch (message.getType()) {
            case "registration_questions":
                RegistrationQuestionsToClient questions = message.getData(
                        RegistrationQuestionsToClient.class);
                Intent intent = new Intent(LoginActivity.this, RegistrationActivity.class);
                intent.putParcelableArrayListExtra("questions", questions.getQuestions());
                startActivity(intent);
                break;
            case "login":
                runOnUiThread(() -> loadingProgressBar.setVisibility(View.GONE));

                LoginToClient loginToClient = message.getData(LoginToClient.class);
                if (loginToClient.isSuccessful()) {
                    String login = usernameEditText.getText().toString();
                    String password = passwordEditText.getText().toString();
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString(TOKEN, loginToClient.getToken());
                    if (!isRestart && !isRestartGame) {
                        editor.putString(LOGIN, login);
                        editor.putString(PASSWORD, password);
                    }
                    editor.apply();

                    if (!isRestartGame) {
                        Intent intentMenu = new Intent(LoginActivity.this, MenuActivity.class);
                        startActivity(intentMenu);
                        finish();
                    }

                    String toastMessage;
                    if (isRestart) {
                        toastMessage = "Вы свернули приложение и были возвращены в главное меню";
                    } else {
                        toastMessage = "Вы успешно авторизовались";
                    }

                    String finalToastMessage = toastMessage;
                    if (!isRestartGame) {
                        runOnUiThread(() -> {
                            Toast toast = Toast.makeText(getBaseContext(), finalToastMessage,
                                    Toast.LENGTH_SHORT);
                            toast.show();
                        });
                    }

                    if (isRestartGame) {
                        timer = new CountDownTimer(1000, 1000) {

                            public void onTick(long millisUntilFinished) {
                                // Ничего не делать
                            }

                            public void onFinish() {
                                Intent intentMenu = new Intent(LoginActivity.this, MenuActivity.class);
                                startActivity(intentMenu);
                                finish();
                            }
                        }.start();
                    }
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), loginToClient.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            case "update_game_state":
                timer.cancel();
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
                String login = sharedPreferences.getString(LOGIN, "");
                gameIntent = new Intent(LoginActivity.this, GameActivity.class);
                gameIntent.putExtra("gameState", gameState);
                gameIntent.putExtra("restartGame", isRestartGame);
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
            case "notify_move_turn":
                if (!isStartGame) {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), "notify_move_turn error", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                gameIntent.putExtra("notifyMoveTurn", true);
                startActivity(gameIntent);
                finish();
                break;
            default:
                throw new RuntimeException();
        }
    }
}