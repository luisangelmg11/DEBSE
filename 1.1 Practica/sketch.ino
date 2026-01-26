#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_VERDE 5
#define LED_ROJO 18
#define BUZZER 19

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Inicia OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED falló"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Estacion Iniciada");
  display.display();
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Error lectura DHT!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Error sensor!");
    display.display();
    return;
  }

  // Mostrar en OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("T: "); display.print(t,1); display.println(" C");
  display.setTextSize(1);
  display.print("Humedad: "); display.print(h,0); display.println("%");
  
  // Lógica de control
  if (t > 30) {              // Umbral alto (ajusta a tu gusto)
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
    tone(BUZZER, 800, 500);   // Pitido
    display.setCursor(0,40);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("ALERTA! Temp Alta");
  } else {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
    noTone(BUZZER);
    display.setCursor(0,40);
    display.println("Normal");
  }
  
  display.display();
  Serial.print("Temp: "); Serial.print(t); Serial.print(" C   Hum: "); Serial.print(h); Serial.println("%");
  
  delay(2000);  // Lee cada 2 segundos
}