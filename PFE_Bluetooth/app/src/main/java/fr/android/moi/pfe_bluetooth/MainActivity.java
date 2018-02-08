package fr.android.moi.pfe_bluetooth;

import android.graphics.Typeface;
import android.os.Build;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity{
    private FragmentTransaction transaction;
    //private TextView connection;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LinearLayout mainLayout = (LinearLayout)findViewById(R.id.main);
        FrameLayout fragmentLayout = (FrameLayout)findViewById(R.id.sample_content_fragment);
        final LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        //Connection layout

        TextView connection = new TextView(this);
        connection.setText(R.string.connection);
        connection.setTextSize(32);
        connection.setPadding(0,125,0,125);
        connection.setClickable(true);
        connection.setGravity(Gravity.CENTER);
        connection.setTypeface(connection.getTypeface(),Typeface.BOLD);
        connection.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,0,2));
        TextView description = new TextView(this);
        description.setText(R.string.description1);
        TextView name = new TextView(this);
        name.setText(R.string.app_name_short);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            linearLayout.setBackgroundColor(this.getColor(R.color.colorPrimary));
        }
        else
            linearLayout.setBackgroundColor(getResources().getColor(R.color.colorPrimary));
        //connection = (TextView) findViewById(R.id.connection);
        connection.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                transaction = getSupportFragmentManager().beginTransaction();
                ActivitiesManager fragment = new ActivitiesManager();
                transaction.replace(R.id.sample_content_fragment, fragment);
                linearLayout.removeAllViews();
                transaction.addToBackStack(null);
                transaction.commit();
            }
        });

        linearLayout.addView(name,lp);
        linearLayout.addView(description,lp);
        linearLayout.addView(connection);
        //fragmentLayout.removeAllViews();
        mainLayout.removeAllViews();
        fragmentLayout.addView(linearLayout);


        setContentView(fragmentLayout);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        getMenuInflater().inflate(R.menu.main, menu);

        return true;

    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()){
            case R.id.profil:
                transaction = getSupportFragmentManager().beginTransaction();
                ActivitiesManager Afragment = new ActivitiesManager();
                transaction.replace(R.id.sample_content_fragment, Afragment);
                transaction.commit();
                return true;
            case R.id.rapport:
                transaction = getSupportFragmentManager().beginTransaction();
                //RapportFragment Rfragment = new RapportFragment();
                //transaction.replace(R.id.sample_content_fragment, Rfragment);
                //transaction.commit();
                return true;
            case R.id.tps:
                transaction = getSupportFragmentManager().beginTransaction();
                BluetoothFragment Bfragment = new BluetoothFragment();
                transaction.replace(R.id.sample_content_fragment, Bfragment);
                transaction.addToBackStack(null);
                transaction.commit();
                return true;
        }
        return false;
    }

}
