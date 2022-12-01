package ru.poly.bridgeandroid;

import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

public class WaitGameFragment extends DialogFragment {
    public Dialog onCreateDialog(Bundle savedInstanceState) {

        String title = "Ожидание лидера комнаты\n" + "игроков в лобби: ";
        //String message = "Присоединиться к игре?";
        String exit = "Exit";

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(title);  // заголовок
        builder.setCancelable(true);

        builder.setNeutralButton(exit, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.dismiss();
            }
        });

        return builder.create();
    }
}