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

import ru.poly.bridgeandroid.R;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToClient;
import ru.poly.bridgeandroid.model.menu.ForgotPasswordToServer;
import ru.poly.bridgeandroid.model.menu.SecretQuestionToClient;
import ru.poly.bridgeandroid.model.menu.SecretQuestionToServer;

public class SecretQuestionActivity extends AppCompatActivity {

    private Gson gson;
    private LoginViewModel loginViewModel;
    private EditText answerEditText;
    private ProgressBar loadingProgressBar;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_secret_question);
        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        Intent myIntent = getIntent();
        ForgotPasswordToClient forgotPassword = myIntent.getParcelableExtra("forgotPassword");

        answerEditText = findViewById(R.id.secret_question_answer);
        final Button secretQuestionButton = findViewById(R.id.secret_question_button);
        TextView question = findViewById(R.id.secret_question_question);
        loadingProgressBar = findViewById(R.id.loading);

        question.setText(forgotPassword.getQuestion());

        loginViewModel.getLoginFormState().observe(this, new Observer<LoginFormState>() {
            @Override
            public void onChanged(@Nullable LoginFormState loginFormState) {
                if (loginFormState == null) {
                    return;
                }
                secretQuestionButton.setEnabled(loginFormState.isDataValid());
                if (loginFormState.getAnswerError() != null) {
                    answerEditText.setError(getString(loginFormState.getAnswerError()));
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
                loginViewModel.answerDataChanged(answerEditText.getText().toString());
            }
        };
        answerEditText.addTextChangedListener(afterTextChangedListener);

        secretQuestionButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadingProgressBar.setVisibility(View.VISIBLE);

                SecretQuestionToServer secretQuestionToServer = new SecretQuestionToServer(
                        forgotPassword.getLogin(), answerEditText.getText().toString());
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(secretQuestionToServer);
                Message message = new Message("0", "secret_question", jsonObject);
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
            case "secret_question":
                SecretQuestionToClient secretQuestion = message.getData(SecretQuestionToClient.class);
                if (secretQuestion.isSuccessful()) {
                    Intent intent = new Intent(SecretQuestionActivity.this, ChangePasswordActivity.class);
                    intent.putExtra("login", secretQuestion.getLogin());
                    startActivity(intent);
                    finish();
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), secretQuestion.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}