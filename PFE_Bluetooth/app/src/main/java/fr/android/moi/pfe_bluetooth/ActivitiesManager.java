package fr.android.moi.pfe_bluetooth;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * Created by Térence on 06/02/2018.
 */

public class ActivitiesManager extends Fragment{
    private static final String TAG = "ActivitiesManager";
    private TextView name, age, macAddr;
    private static String userName = "Othman";
    private static int userAge = 22;
    private static String userMac = "00:14:03:06:41:8B";
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);

    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_manager, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        name = (TextView) getActivity().findViewById(R.id.name);
        name.setText(getString(R.string.hello_name,userName));
        age = (TextView) getActivity().findViewById(R.id.age);
        age.setText(getString(R.string.hello_age,userAge));
        macAddr = (TextView) getActivity().findViewById(R.id.device);
        macAddr.setText(getString(R.string.MAC,userMac));
    }
}
