package com.springexample.stepmotorcontrol;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

@RestController
@RequestMapping("/api")
public class Api {

    private final RestTemplate restTemplate = new RestTemplate();
    private final String esp32Url = "http://192.168.52.57:8086"; // ESP32 IP adresi

    @PostMapping("/speed")
    public String setSpeed(@RequestParam String value){

        String url = esp32Url + "/setSpeed?value=" + value;
        try {
            String response = restTemplate.getForObject(url, String.class);
            return "ESP32 yanıtı: " + response;
        } catch (Exception e) {
            return "Hata: " + e.getMessage();
        }
    }

    @PostMapping("/angle")
    public String setAngle(@RequestParam String value){

        String url = esp32Url + "/setAngle?value=" + value;
        try {
            String response = restTemplate.getForObject(url, String.class);
            return "ESP32 yanıtı: " + response;
        } catch (Exception e) {
            return "Hata: " + e.getMessage();
        }

    }

    @PostMapping("/direction")
    public String setDirection(@RequestParam String value){

        String url = esp32Url + "/setDirection?value=" + value;
        try {
            String response = restTemplate.getForObject(url, String.class);
            return "ESP32 yanıtı: " + response;
        } catch (Exception e) {
            return "Hata: " + e.getMessage();
        }
    }

}
