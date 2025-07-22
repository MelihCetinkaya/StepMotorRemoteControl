package com.example.remotecontrol.RetrofitInstance

import com.example.remotecontrol.api.AngleApi
import com.example.remotecontrol.api.DirectionApi
import com.example.remotecontrol.api.SpeedApi
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import java.util.concurrent.TimeUnit

object RetrofitInstance {
    private const val BASE_URL = "http://192.168.52.162:8085/"

    private val loggingInterceptor = HttpLoggingInterceptor().apply {
        level = HttpLoggingInterceptor.Level.BODY
    }

    private val client = OkHttpClient.Builder()
        .connectTimeout(30, TimeUnit.SECONDS)
        .readTimeout(30, TimeUnit.SECONDS)
        .writeTimeout(30, TimeUnit.SECONDS)
        .addInterceptor { chain ->
            val request = chain.request()
                .newBuilder()
                .addHeader("Accept", "application/json")
                .addHeader("Content-Type", "application/json")
                .build()
            chain.proceed(request)
        }
        .addInterceptor(loggingInterceptor)
        .build()

    private val retrofit by lazy {
        Retrofit.Builder()
            .baseUrl(BASE_URL)
            .client(client)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
    }

    val speedApi: SpeedApi by lazy {
        retrofit.create(SpeedApi::class.java)
    }

    val angleApi: AngleApi by lazy {
        retrofit.create(AngleApi::class.java)
    }

    val directionApi: DirectionApi by lazy {
        retrofit.create(DirectionApi::class.java)
    }
}