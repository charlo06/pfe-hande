package fr.android.moi.pfe_bluetooth;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
//import android.util.Log;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class BluetoothFragment extends Fragment {
    private static final String TAG = "BluetoothCommFragment";
    private BluetoothAdapter mBluetoothAdapter = null;
    private BluetoothCommService mCommService = null;
    /**

     * Name of the connected device

     */

    private String mConnectedDeviceName = null;
    // Layout Views

    //private ListView mConversationView;

    private EditText mOutEditText;

    private Button mSendButton;

    /**

     * Array adapter for the conversation thread

     */

    //private ArrayAdapter<String> mConversationArrayAdapter;
    /**

     * String buffer for outgoing messages

     */
    // Intent request codes

    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;

    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;

    private static final int REQUEST_ENABLE_BT = 3;

    private StringBuffer mOutStringBuffer;

    //database
    String sX;
    String sY;
    private CommentsDataSource dB;

    TextView tvX;
    TextView tvY;
    List<Comment> values;
    int i=0;
    int j=1;
    //TextView tvY;
    //TextView tvZ;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            FragmentActivity activity = getActivity();
            Toast.makeText(activity, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            activity.finish();
        }

        //database
        dB = new CommentsDataSource(getContext());
        dB.open();
        values = dB.getAllComments();

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_bluetooth, container, false);
    }
    @Override
    public void onStart(){
        super.onStart();

        // If BT is not on, request that it be enabled.

        // setupComm() will then be called during onActivityResult

        if (!mBluetoothAdapter.isEnabled()) {

            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);

            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);

            // Otherwise, setup the chat session

        } else if (mCommService == null) {

            setupComm();

        }
    }
    @Override

    public void onResume() {

        dB.open();
        super.onResume();



        // Performing this check in onResume() covers the case in which BT was

        // not enabled during onStart(), so we were paused to enable it...

        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.

        if (mCommService != null) {

            // Only if the state is STATE_NONE, do we know that we haven't started already

            if (mCommService.getState() == BluetoothCommService.STATE_NONE) {

                // Start the Bluetooth chat services

                mCommService.start();

            }

        }

    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onDestroy(){
        dB.close();
        super.onDestroy();
    }
    @Override

    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {

        //mConversationView = (ListView) view.findViewById(R.id.in);
        tvX = (TextView) view.findViewById(R.id.tvx);
        tvY = (TextView) view.findViewById(R.id.tvY);
        //mOutEditText = (EditText) view.findViewById(R.id.edit_text_out);

        //mSendButton = (Button) view.findViewById(R.id.button_send);

    }
    /**

     * Set up the UI and background operations for chat.

     */

    private void setupComm() {

        Log.d(TAG, "setupComm()");



        // Initialize the array adapter for the conversation thread

        //mConversationArrayAdapter = new ArrayAdapter<>(getActivity(), R.layout.message);
        //mConversationView.setAdapter(mConversationArrayAdapter);

        // Initialize the compose field with a listener for the return key

        //mOutEditText.setOnEditorActionListener(mWriteListener);



        // Initialize the send button with a listener that for click events

        /*mSendButton.setOnClickListener(new View.OnClickListener() {

            public void onClick(View v) {

                // Send a message using content of the edit text widget

                View view = getView();

                if (null != view) {

                    TextView textView = (TextView) view.findViewById(R.id.edit_text_out);

                    String message = textView.getText().toString();

                    sendMessage(message);

                }

            }

        });*/



        // Initialize the BluetoothChatService to perform bluetooth connections

        mCommService = new BluetoothCommService(getActivity(), mHandler);


        // Initialize the buffer for outgoing messages

        mOutStringBuffer = new StringBuffer("");

    }
    /**

     * Makes this device discoverable for 300 seconds (5 minutes).

     */

    private void ensureDiscoverable() {

        if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {

            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);

            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);

            startActivity(discoverableIntent);

        }

    }
    /*
    /**

     * Sends a message.

     *

     * @param message A string of text to send.



    private void sendMessage(String message) {

        // Check that we're actually connected before trying anything

        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {

            Toast.makeText(getActivity(), R.string.not_connected, Toast.LENGTH_SHORT).show();

            return;

        }



        // Check that there's actually something to send

        if (message.length() > 0) {

            // Get the message bytes and tell the BluetoothChatService to write

            byte[] send = message.getBytes();

            mChatService.write(send);



            // Reset out string buffer to zero and clear the edit text field

            mOutStringBuffer.setLength(0);

            mOutEditText.setText(mOutStringBuffer);

        }

    }
     */
    /**

     * Updates the status on the action bar.

     *

     * @param resId a string resource ID

     */

    private void setStatus(int resId) {

        FragmentActivity activity = getActivity();

        if (null == activity) {

            return;

        }

        final ActionBar actionBar = activity.getActionBar();

        if (null == actionBar) {

            return;

        }

        actionBar.setSubtitle(resId);

    }



    /**

     * Updates the status on the action bar.

     *

     * @param subTitle status

     */

    private void setStatus(CharSequence subTitle) {

        FragmentActivity activity = getActivity();

        if (null == activity) {

            return;

        }

        final ActionBar actionBar = activity.getActionBar();

        if (null == actionBar) {

            return;

        }

        actionBar.setSubtitle(subTitle);

    }
    /**

     * The Handler that gets information back from the BluetoothCommService

     */

    private final Handler mHandler = new Handler() {

        @Override

        public void handleMessage(Message msg) {

            FragmentActivity activity = getActivity();

            switch (msg.what) {

                case Constants.MESSAGE_STATE_CHANGE:

                    switch (msg.arg1) {

                        case BluetoothCommService.STATE_CONNECTED:

                            setStatus(getString(R.string.title_connected_to, mConnectedDeviceName));

                            //mConversationArrayAdapter.clear();

                            break;

                        case BluetoothCommService.STATE_CONNECTING:

                            setStatus(R.string.title_connecting);

                            break;

                        case BluetoothCommService.STATE_LISTEN:

                        case BluetoothCommService.STATE_NONE:

                            setStatus(R.string.title_not_connected);

                            break;

                    }

                    break;
                /*
                case Constants.MESSAGE_WRITE:

                    byte[] writeBuf = (byte[]) msg.obj;

                    // construct a string from the buffer

                    String writeMessage = new String(writeBuf);

                    mConversationArrayAdapter.add("Me:  " + writeMessage);

                    break;
                */
                case Constants.MESSAGE_READ:

                    String accX, accY, accZ;
                    String[] tab;
                    int begin = msg.arg1;
                    int end = msg.arg2;
                    byte[] readBuf = (byte[]) msg.obj;

                    // construct a string from the valid bytes in the buffer

                    String readMessage = new String(readBuf);
                    readMessage = readMessage.substring(begin,end);
                    if (readMessage.length() > 1) {
                        tab = readMessage.split("/");
                        accX = tab[0];
                        accY = tab[1];
                        accZ = tab[2];
                    }else {
                        accX = "No value";
                        accY = "No value";
                        accZ = "No value";
                    }
                    //long x = Long.valueOf(accX);
                    //long y = Long.valueOf(accY);
                    //long z = Long.valueOf(accZ);

                    dB.createComment(accX);
                    dB.createComment(accY);
                    dB.createComment(accZ);

                    if (values.size()!=0){
                        sX=values.get(i).getComment();
                        sY=values.get(j).getComment();
                        tvX.setText("X: " + sX);
                        tvY.setText("Y: " + sY);
                    }


                    i=i+3;
                    j=j+3;
                    //tvY.setText("Y: " + sY);

                    /*mConversationArrayAdapter.add(
                            mConnectedDeviceName + ":  " + "X: "+x+" ,Y: "+y+" ,Z: "+z
                    );*/

                    break;

                case Constants.MESSAGE_DEVICE_NAME:

                    // save the connected device's name

                    mConnectedDeviceName = msg.getData().getString(Constants.DEVICE_NAME);

                    if (null != activity) {

                        Toast.makeText(activity, "Connected to "

                                + mConnectedDeviceName, Toast.LENGTH_SHORT).show();

                    }

                    break;

                case Constants.MESSAGE_TOAST:

                    if (null != activity) {

                        Toast.makeText(activity, msg.getData().getString(Constants.TOAST),

                                Toast.LENGTH_SHORT).show();

                    }

                    break;

            }

        }

    };
    public void onActivityResult(int requestCode, int resultCode, Intent data) {

        switch (requestCode) {

            case REQUEST_CONNECT_DEVICE_SECURE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {

                    connectDevice(data, true);

                }

                break;

            case REQUEST_CONNECT_DEVICE_INSECURE:

                // When DeviceListActivity returns with a device to connect

                if (resultCode == Activity.RESULT_OK) {

                    connectDevice(data, false);

                }

                break;

            case REQUEST_ENABLE_BT:

                // When the request to enable Bluetooth returns

                if (resultCode == Activity.RESULT_OK) {

                    // Bluetooth is now enabled, so set up a chat session
                    //Toast.makeText(getActivity(), "Bluetooth is now available", Toast.LENGTH_LONG).show();
                    setupComm();

                } else {

                    // User did not enable Bluetooth or an error occurred

                    Log.d(TAG, "BT not enabled");
                    Log.d(TAG, String.valueOf(resultCode));
                    Toast.makeText(getActivity(), R.string.bt_not_enabled_leaving,

                            Toast.LENGTH_SHORT).show();

                    getActivity().finish();

                }

        }

    }
    /**

     * Establish connection with other device

     *

     * @param data   An {@link Intent} with {@link DeviceListActivity#EXTRA_DEVICE_ADDRESS} extra.

     * @param secure Socket Security type - Secure (true) , Insecure (false)

     */

    private void connectDevice(Intent data, boolean secure) {

        // Get the device MAC address

        String address = data.getExtras()

                .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);

        // Get the BluetoothDevice object

        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        //Toast.makeText(getActivity(), "device: "+device.getName(), Toast.LENGTH_SHORT).show();
        // Attempt to connect to the device

        mCommService.connect(device, secure);

    }
    @Override

    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        if(mCommService == null)
            inflater.inflate(R.menu.bluetooth_comm, menu);
        else if (mCommService.getState() == BluetoothCommService.STATE_CONNECTED)
            inflater.inflate(R.menu.main, menu);
        else
            inflater.inflate(R.menu.bluetooth_comm, menu);

    }



    @Override

    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {

            case R.id.secure_connect_scan: {

                // Launch the DeviceListActivity to see devices and do scan

                Intent serverIntent = new Intent(getActivity(), DeviceListActivity.class);

                startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);

                return true;

            }

            case R.id.insecure_connect_scan: {

                // Launch the DeviceListActivity to see devices and do scan
                //Toast.makeText(getActivity(), "insecure ", Toast.LENGTH_SHORT).show();
                Intent serverIntent = new Intent(getActivity(), DeviceListActivity.class);

                startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_INSECURE);

                return true;

            }

            case R.id.discoverable: {

                // Ensure this device is discoverable by others

                ensureDiscoverable();

                return true;

            }

        }

        return false;

    }
}
