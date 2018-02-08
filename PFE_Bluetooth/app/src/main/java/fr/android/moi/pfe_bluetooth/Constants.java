package fr.android.moi.pfe_bluetooth;

/**
 * Created by Térence on 24/01/2018.
 */

public interface Constants {
    // Message types sent from the BluetoothChatService Handler

    public static final int MESSAGE_STATE_CHANGE = 1;

    public static final int MESSAGE_READ = 2;

    public static final int MESSAGE_WRITE = 3;

    public static final int MESSAGE_DEVICE_NAME = 4;

    public static final int MESSAGE_TOAST = 5;



    // Key names received from the BluetoothChatService Handler

    public static final String DEVICE_NAME = "device_name";

    public static final String TOAST = "toast";
}
