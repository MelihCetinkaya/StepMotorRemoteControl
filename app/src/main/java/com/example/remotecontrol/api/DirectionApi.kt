package com.example.remotecontrol.api

import retrofit2.Response
import retrofit2.http.*

interface DirectionApi {
    // 1. As form-urlencoded
    @FormUrlEncoded
    @POST("api/direction")
    suspend fun setDirectionFormEncoded(
        @Field("value") value: Int
    ): Response<Unit>
    
    // 2. As query parameter with POST
    @POST("api/direction")
    suspend fun setDirectionQuery(
        @Query("value") value: Int
    ): Response<Unit>
    
    // 3. As raw JSON
    @POST("api/direction")
    @Headers("Content-Type: application/json")
    suspend fun setDirectionJson(
        @Body request: Map<String, Int>
    ): Response<Unit>
}
