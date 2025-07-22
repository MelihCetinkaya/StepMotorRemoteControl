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

class SpeedSetActivity : AppCompatActivity() {
    
    private lateinit var button1: Button
    private lateinit var button2: Button
    private lateinit var button3: Button
    private lateinit var button4: Button
    private lateinit var button5: Button
    private lateinit var button6: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_speed_set)
        
        // Initialize buttons
        button1 = findViewById(R.id.button1)
        button2 = findViewById(R.id.button2)
        button3 = findViewById(R.id.button3)
        button4 = findViewById(R.id.button4)
        button5 = findViewById(R.id.button5)
        button6 = findViewById(R.id.button6)
        
        // Set click listeners with sequential values starting from 20
        button1.setOnClickListener { setSpeed(20) }    // x2 speed
        button2.setOnClickListener { setSpeed(21) }    // /2 speed
        button3.setOnClickListener { setSpeed(22) }    // x3 speed
        button4.setOnClickListener { setSpeed(23) }    // /3 speed
        button5.setOnClickListener { setSpeed(24) }    // x5 speed
        button6.setOnClickListener { setSpeed(25) }    // /5 speed
    }
    
    private fun setSpeed(speedValue: Int) {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                // Try approach 1: Form URL Encoded
                var response = RetrofitInstance.speedApi.setSpeedFormEncoded(speedValue)
                
                // If approach 1 fails, try approach 2: Query Parameter
                if (!response.isSuccessful) {
                    response = RetrofitInstance.speedApi.setSpeedQuery(speedValue)
                }
                
                // If approach 2 fails, try approach 3: JSON Body
                if (!response.isSuccessful) {
                    val request = mapOf("value" to speedValue)
                    response = RetrofitInstance.speedApi.setSpeedJson(request)
                }
                
                withContext(Dispatchers.Main) {
                    if (response.isSuccessful) {
                        showToast("Hız başarıyla ayarlandı")
                    } else {
                        showToast("Hız ayarlanamadı (${response.code()}). Lütfen bağlantınızı kontrol edin.")
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    showToast("Hata oluştu: ${e.message}")
                }
            }
        }
    }
    
    private fun showToast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }
}
