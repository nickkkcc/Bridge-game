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
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
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
import com.weiwangcn.betterspinner.library.material.MaterialBetterSpinner;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;
import java.util.List;

import ru.poly.bridgeandroid.MenuActivity;
import ru.poly.bridgeandroid.R;
import ru.poly.bridgeandroid.model.menu.LoginToClient;
import ru.poly.bridgeandroid.model.menu.LoginToServer;
import ru.poly.bridgeandroid.model.Message;
import ru.poly.bridgeandroid.model.menu.Question;
import ru.poly.bridgeandroid.model.menu.RegistrationToClient;
import ru.poly.bridgeandroid.model.menu.RegistrationToServer;

public class RegistrationActivity extends AppCompatActivity {

    private static final String TOKEN = "token";
    private static final String LOGIN = "login";
    private static final String PREFERENCE = "preference";
    private LoginViewModel loginViewModel;
    private Gson gson;
    private ProgressBar loadingProgressBar;
    private EditText usernameEditText;
    private EditText passwordEditText;
    private Button registrationButton;
    private int questionIndex;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registration);

        loginViewModel = new ViewModelProvider(this, new LoginViewModelFactory())
                .get(LoginViewModel.class);
        gson = new Gson();

        usernameEditText = findViewById(R.id.registration_username);
        passwordEditText = findViewById(R.id.registration_password);
        final MaterialBetterSpinner spinner = findViewById(R.id.registration_question_spinner);
        final EditText answerEditText = findViewById(R.id.registration_answer);
        registrationButton = findViewById(R.id.registration_registration);
        loadingProgressBar = findViewById(R.id.registration_loading);

        Intent myIntent = getIntent();
        List<Question> questions =
                myIntent.getParcelableArrayListExtra("questions");
        List<String> questionsList = new ArrayList<>();
        for (Question question : questions) {
            questionsList.add(question.getQuestion());
        }

        ArrayAdapter<String> adapter = new ArrayAdapter(this, R.layout.spinner_item, questionsList);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        spinner.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long l) {
                questionIndex = position;
            }
        });

        loginViewModel.getLoginFormState().observe(this, new Observer<LoginFormState>() {
            @Override
            public void onChanged(@Nullable LoginFormState loginFormState) {
                if (loginFormState == null) {
                    return;
                }
                registrationButton.setEnabled(loginFormState.isDataValid());
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

        registrationButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadingProgressBar.setVisibility(View.VISIBLE);

                RegistrationToServer registrationToServer = new RegistrationToServer(
                        usernameEditText.getText().toString(),
                        passwordEditText.getText().toString(),
                        questions.get(questionIndex).getQuestionId(),
                        answerEditText.getText().toString());
                JsonObject jsonObject = (JsonObject) gson.toJsonTree(registrationToServer);
                Message message = new Message("0", "registration", jsonObject);
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

    @Subscribe
    public void onMessage(Message message) {
        switch (message.getType()) {
            case "registration":
                runOnUiThread(() -> {
                    loadingProgressBar.setVisibility(View.GONE);
                    registrationButton.setEnabled(false);
                });

                RegistrationToClient registrationToClient = message.getData(RegistrationToClient.class);
                if (registrationToClient.isSuccessful()) {
                    LoginToServer loginToServer = new LoginToServer(usernameEditText.getText().toString(),
                            passwordEditText.getText().toString());
                    JsonObject jsonObject = (JsonObject) gson.toJsonTree(loginToServer);
                    Message loginMessage = new Message("0", "login", jsonObject);
                    EventBus.getDefault().post(gson.toJson(loginMessage));
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), registrationToClient.getError(),
                                Toast.LENGTH_SHORT);
                        toast.show();
                    });
                    runOnUiThread(() -> registrationButton.setEnabled(true));
                }
                break;
            case "login":
                LoginToClient loginToClient = message.getData(LoginToClient.class);
                if (loginToClient.isSuccessful()) {
                    SharedPreferences sharedPreferences = getSharedPreferences(PREFERENCE, MODE_PRIVATE);
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString(TOKEN, loginToClient.getToken());
                    editor.putString(LOGIN, usernameEditText.getText().toString());
                    editor.apply();

                    Intent intent = new Intent(RegistrationActivity.this, MenuActivity.class);
                    startActivity(intent);
                    finish();

                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(),
                                "Вы успешно зарегестрировались", Toast.LENGTH_SHORT);
                        toast.show();
                    });
                } else {
                    runOnUiThread(() -> {
                        Toast toast = Toast.makeText(getBaseContext(), loginToClient.getError(), Toast.LENGTH_SHORT);
                        toast.show();
                    });
                }
                break;
            default:
                throw new RuntimeException();
        }
    }
}
