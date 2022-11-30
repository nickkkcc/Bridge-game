package ru.poly.bridgeandroid;

import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

public class InvitationFragment extends DialogFragment {
    //@NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {

        String title = "Игрок @SomeUserNick пригласил вас";
        String message = "Присоединиться к игре?";
        String answerNo = "Нет";
        String answerYes = "Да";

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(title);  // заголовок
        builder.setMessage(message); // сообщение
        builder.setCancelable(true);

        builder.setPositiveButton(answerYes, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                //
            }
        });
        builder.setNegativeButton(answerNo, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.dismiss();
            }
        });

        return builder.create();
    }
}