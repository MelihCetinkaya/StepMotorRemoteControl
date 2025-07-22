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

class DirectionSetActivity : AppCompatActivity() {
    
    private lateinit var buttonClockwise: Button
    private lateinit var buttonCounterClockwise: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_direction_set)
        
        // Initialize buttons
        buttonClockwise = findViewById(R.id.buttonForward)        // Clockwise button
        buttonCounterClockwise = findViewById(R.id.buttonBackward) // Counterclockwise button
        
        // Set click listeners
        buttonClockwise.setOnClickListener { setDirection(40) }        // Clockwise: 40
        buttonCounterClockwise.setOnClickListener { setDirection(41) }  // Counterclockwise: 41
    }
    
    private fun setDirection(directionValue: Int) {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                // Try approach 1: Form URL Encoded
                var response = RetrofitInstance.directionApi.setDirectionFormEncoded(directionValue)
                
                // If approach 1 fails, try approach 2: Query Parameter
                if (!response.isSuccessful) {
                    response = RetrofitInstance.directionApi.setDirectionQuery(directionValue)
                }
                
                // If approach 2 fails, try approach 3: JSON Body
                if (!response.isSuccessful) {
                    val request = mapOf("value" to directionValue)
                    response = RetrofitInstance.directionApi.setDirectionJson(request)
                }
                
                withContext(Dispatchers.Main) {
                    if (response.isSuccessful) {
                        showToast("Direction set successfully")
                    } else {
                        showToast("Failed to set direction (${response.code()}). Please check your connection.")
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    showToast("Error: ${e.message}")
                }
            }
        }
    }
    
    private fun showToast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }
}
