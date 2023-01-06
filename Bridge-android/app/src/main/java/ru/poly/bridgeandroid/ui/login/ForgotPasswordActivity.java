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
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.game.PlayerGameState;
import ru.poly.bridgeandroid.model.game.UpdateGameState;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToClient;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToServer;
import ru.poly.bridgeandroid.model.menu.LoginToClient;
import ru.poly.bridgeandroid.model.menu.LoginToServer;
import ru.poly.bridgeandroid.model.menu.RegistrationQuestionsToClient;

public class ForgotPasswordActivity extends AppCompatActivity {

    private Gson gson;
    private LoginViewModel loginViewModel;
    private EditText usernameEditText;
    private ProgressBar loadingProgressBar;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_forgot_password);
        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        usernameEditText = findViewById(R.id.forgot_password_username);
        final Button forgotPasswordButton = findViewById(R.id.forgot_password_button);
        loadingProgressBar = findViewById(R.id.loading);

        loginViewModel.getLoginFormState().observe(this, new Observer<LoginFormState>() {
            @Override
            public void onChanged(@Nullable LoginFormState loginFormState) {
                if (loginFormState == null) {
                    return;
                }
                forgotPasswordButton.setEnabled(loginFormState.isDataValid());
                if (loginFormState.getUsernameError() != null) {
                    usernameEditText.setError(getString(loginFormState.getUsernameError()));
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
                loginViewModel.usernameDataChanged(usernameEditText.getText().toString());
            }
        };
        usernameEditText.addTextChangedListener(afterTextChangedListener);

        forgotPasswordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadingProgressBar.setVisibility(View.VISIBLE);

                ForgotPasswordToServer forgotPasswordToServer = new ForgotPasswordToServer(
                        usernameEditText.getText().toString());
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(forgotPasswordToServer);
                Message message = new Message("0", "forgot_password", jsonObject);
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

    @Subscribe(threadMode = ThreadMode.MAIN_ORDERED)
    public void onMessage(Message message) {
        runOnUiThread(() -> loadingProgressBar.setVisibility(View.GONE));
        switch (message.getType()) {
            case "forgot_password":
                ForgotPasswordToClient forgotPassword = message.getData(ForgotPasswordToClient.class);
                if (forgotPassword.isSuccessful()) {
                    Intent intent = new Intent(ForgotPasswordActivity.this, SecretQuestionActivity.class);
                    intent.putExtra("forgotPassword", forgotPassword);
                    startActivity(intent);
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), forgotPassword.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}