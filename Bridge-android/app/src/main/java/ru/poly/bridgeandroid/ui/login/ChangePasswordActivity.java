package ru.poly.bridgeandroid.ui.login;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
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

import ru.poly.bridgeandroid.R;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.ChangePasswordToClient;
import ru.poly.bridgeandroid.model.menu.ChangePasswordToServer;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToClient;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToServer;
import ru.poly.bridgeandroid.model.menu.SecretQuestionToClient;

public class ChangePasswordActivity extends AppCompatActivity {

    private Gson gson;
    private LoginViewModel loginViewModel;
    private EditText passwordEditText;
    private ProgressBar loadingProgressBar;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_password);
        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        Intent myIntent = getIntent();
        String login = myIntent.getStringExtra("login");

        passwordEditText = findViewById(R.id.change_password_password);
        final Button changePasswordButton = findViewById(R.id.change_password_button);
        loadingProgressBar = findViewById(R.id.loading);

        loginViewModel.getLoginFormState().observe(this, new Observer<LoginFormState>() {
            @Override
            public void onChanged(@Nullable LoginFormState loginFormState) {
                if (loginFormState == null) {
                    return;
                }
                changePasswordButton.setEnabled(loginFormState.isDataValid());
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
                loginViewModel.passwordDataChanged(passwordEditText.getText().toString());
            }
        };
        passwordEditText.addTextChangedListener(afterTextChangedListener);

        changePasswordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadingProgressBar.setVisibility(View.VISIBLE);

                ChangePasswordToServer changePassword = new ChangePasswordToServer(login,
                        passwordEditText.getText().toString());
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(changePassword);
                Message message = new Message("0", "change_password", jsonObject);
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
            case "change_password":
                ChangePasswordToClient changePassword = message.getData(ChangePasswordToClient.class);
                if (changePassword.isSuccessful()) {
                    Intent intent = new Intent(ChangePasswordActivity.this, LoginActivity.class);
                    startActivity(intent);
                    finishAffinity();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), changePassword.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}