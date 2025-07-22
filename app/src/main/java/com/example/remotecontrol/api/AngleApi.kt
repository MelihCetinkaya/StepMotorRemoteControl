package com.example.remotecontrol.api

import retrofit2.Response
import retrofit2.http.*

interface AngleApi {
    // 1. As form-urlencoded
    @FormUrlEncoded
    @POST("api/angle")
    suspend fun setAngleFormEncoded(
        @Field("value") value: Int
    ): Response<Unit>
    
    // 2. As query parameter with POST
    @POST("api/angle")
    suspend fun setAngleQuery(
        @Query("value") value: Int
    ): Response<Unit>
    
    // 3. As raw JSON
    @POST("api/angle")
    @Headers("Content-Type: application/json")
    suspend fun setAngleJson(
        @Body request: Map<String, Int>
    ): Response<Unit>
}
