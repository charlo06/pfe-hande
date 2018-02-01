package fr.ece.hande.handemanager;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View view){
        ImageButton imageButton = (ImageButton) view;

        switch (imageButton.getId()){
            case R.id.imgButtonContact:
                System.out.println("contact");
                break;
            case R.id.imgButtonReport:
                System.out.println("report");
                Intent intent =new Intent (this,GraphDataActivity.class);
                startActivity(intent);
                break;
            case R.id.imgButtonTraining:
                System.out.println("training");
                Intent intent2 = new Intent(Intent.ACTION_VIEW).setData(Uri.parse("http://www.stackoverflow.com"));
                startActivity(intent2);
                break;
        }
    }
}
