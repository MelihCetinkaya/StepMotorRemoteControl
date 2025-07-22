package com.example.remotecontrol

import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.remotecontrol.RetrofitInstance.RetrofitInstance
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class AngleSetActivity : AppCompatActivity() {
    
    private lateinit var button45: Button
    private lateinit var button90: Button
    private lateinit var button135: Button
    private lateinit var button180: Button
    private lateinit var button225: Button
    private lateinit var button270: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_angle_set)
        
        // Initialize buttons
        button45 = findViewById(R.id.button1)    // 45 degrees
        button90 = findViewById(R.id.button2)    // 90 degrees
        button135 = findViewById(R.id.button3)   // 135 degrees
        button180 = findViewById(R.id.button4)   // 180 degrees
        button225 = findViewById(R.id.button5)   // 270 degrees (XML'de 270 olarak etiketlenmiş)
        button270 = findViewById(R.id.button6)   // 360 degrees (XML'de 360 olarak etiketlenmiş)
        
        // Set click listeners with sequential values starting from 10
        button45.setOnClickListener { setAngle(10) }    // 45 degrees: 10
        button90.setOnClickListener { setAngle(11) }    // 90 degrees: 11
        button135.setOnClickListener { setAngle(12) }    // 135 degrees: 12
        button180.setOnClickListener { setAngle(13) }    // 180 degrees: 13
        button225.setOnClickListener { setAngle(14) }    // 225 degrees: 14
        button270.setOnClickListener { setAngle(15) }    // 270 degrees: 15
    }
    
    private fun setAngle(angleValue: Int) {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                // Try approach 1: Form URL Encoded
                var response = RetrofitInstance.angleApi.setAngleFormEncoded(angleValue)
                
                // If approach 1 fails, try approach 2: Query Parameter
                if (!response.isSuccessful) {
                    response = RetrofitInstance.angleApi.setAngleQuery(angleValue)
                }
                
                // If approach 2 fails, try approach 3: JSON Body
                if (!response.isSuccessful) {
                    val request = mapOf("value" to angleValue)
                    response = RetrofitInstance.angleApi.setAngleJson(request)
                }
                
                withContext(Dispatchers.Main) {
                    if (response.isSuccessful) {
                        showToast("angle adjusted successfully")
                    } else {
                        showToast("Angle couldnt adjust (${response.code()}). Lütfen bağlantınızı kontrol edin.")
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    showToast("error: ${e.message}")
                }
            }
        }
    }
    
    private fun showToast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }
}
