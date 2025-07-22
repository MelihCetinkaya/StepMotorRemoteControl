Kullanılan Donanımlar

28 BYJ-48 Redüktörlü Step Motor
ULN2003A Step Motor Sürücü Kartı
STM32
ESP32

🔌 Bağlantılar

Step Motor IN pinleri -> STM32 pinleri

IN1 -> PA4
IN2 -> PA5
IN3 -> PA6
IN4 -> PA7

ESP32 UART pinleri -> STM32 pinleri

TX2 -> PA3
RX2 -> PA2

⚠️ STM32 ile ESP32’nin GND pinleri ortaklanmıştır.

📝 Kod Tarafında Bilinmesi Gerekenler
🎚️ Hız Kodları
20 -> x2 hız
21 -> /2 hız
22 -> x3 hız
23 -> /3 hız
24 -> x5 hız
25 -> /5 hız

🔄 Açı Kodları
10 -> 45 derece
11 -> 90 derece
12 -> 135 derece
13 -> 180 derece
14 -> 270 derece
15 -> 360 derece
16 -> 720 derece

↔️ Yön Kodları
40 -> Clockwise (Saat yönü)
41 -> Counterclockwise (Saat yönü tersi)
