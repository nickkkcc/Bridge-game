package ru.poly.bridgeandroid.ui.login;


import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;
import static androidx.test.espresso.matcher.ViewMatchers.withId;
import static androidx.test.espresso.matcher.ViewMatchers.withParent;
import static androidx.test.espresso.matcher.ViewMatchers.withText;
import static org.hamcrest.Matchers.allOf;

import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;

import androidx.test.espresso.ViewInteraction;
import androidx.test.ext.junit.rules.ActivityScenarioRule;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.filters.LargeTest;

import org.hamcrest.Description;
import org.hamcrest.Matcher;
import org.hamcrest.TypeSafeMatcher;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import ru.poly.bridgeandroid.R;

@LargeTest
@RunWith(AndroidJUnit4.class)
public class TestAllRegistrationElementsExist {

    @Rule
    public ActivityScenarioRule<LoginActivity> mActivityScenarioRule =
            new ActivityScenarioRule<>(LoginActivity.class);

    @Test
    public void testAllRegistrationElementsExist() {
        ViewInteraction appCompatButton = onView(
                allOf(withId(R.id.registration), withText("Регистрация"),
                        childAtPosition(
                                allOf(withId(R.id.container),
                                        childAtPosition(
                                                withId(android.R.id.content),
                                                0)),
                                4),
                        isDisplayed()));
        appCompatButton.perform(click());

        ViewInteraction textView = onView(
                allOf(withId(R.id.textView2), withText("Регистрация"),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        textView.check(matches(isDisplayed()));

        ViewInteraction editText = onView(
                allOf(withId(R.id.registration_username),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        editText.check(matches(isDisplayed()));

        ViewInteraction editText2 = onView(
                allOf(withId(R.id.registration_password),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        editText2.check(matches(isDisplayed()));

        ViewInteraction autoCompleteTextView = onView(
                allOf(withId(R.id.registration_question_spinner),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        autoCompleteTextView.check(matches(isDisplayed()));

        ViewInteraction editText3 = onView(
                allOf(withId(R.id.registration_answer),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        editText3.check(matches(isDisplayed()));

        ViewInteraction button = onView(
                allOf(withId(R.id.registration_registration), withText("Регистрация"),
                        withParent(allOf(withId(R.id.registration_container),
                                withParent(withId(android.R.id.content)))),
                        isDisplayed()));
        button.check(matches(isDisplayed()));
    }

    private static Matcher<View> childAtPosition(
            final Matcher<View> parentMatcher, final int position) {

        return new TypeSafeMatcher<View>() {
            @Override
            public void describeTo(Description description) {
                description.appendText("Child at position " + position + " in parent ");
                parentMatcher.describeTo(description);
            }

            @Override
            public boolean matchesSafely(View view) {
                ViewParent parent = view.getParent();
                return parent instanceof ViewGroup && parentMatcher.matches(parent)
                        && view.equals(((ViewGroup) parent).getChildAt(position));
            }
        };
    }
}
