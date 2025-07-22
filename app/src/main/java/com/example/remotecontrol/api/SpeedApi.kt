package com.example.remotecontrol.api

import okhttp3.RequestBody
import retrofit2.Response
import retrofit2.http.*

interface SpeedApi {
    // Try different approaches
    
    // 1. As form-urlencoded
    @FormUrlEncoded
    @POST("api/speed")
    suspend fun setSpeedFormEncoded(
        @Field("value") value: Int
    ): Response<Unit>
    
    // 2. As query parameter with POST
    @POST("api/speed")
    suspend fun setSpeedQuery(
        @Query("value") value: Int
    ): Response<Unit>
    
    // 3. As raw JSON
    @POST("api/speed")
    @Headers("Content-Type: application/json")
    suspend fun setSpeedJson(
        @Body request: Map<String, Int>
    ): Response<Unit>
}
