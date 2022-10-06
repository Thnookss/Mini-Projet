#include <Arduino.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <Ticker.h>
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
DHTesp dht;


#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif
#define dhtPin 17
#define GPIO_SDA 10
#define GPIO_SCL 11
SSD1306Wire display(0x3c, GPIO_SDA, GPIO_SCL);

float humidity;
float temperature;

void DHTGet( void *pvParameters ){
   delay(dht.getMinimumSamplingPeriod());

   while(true)
   {

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();

    vTaskDelay( pdMS_TO_TICKS( 2000 ) );
   }

}

void AfficherTexte( void *pvParameters )
{

  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.

  for (;;) {
    display.setLogBuffer(5, 30);
    display.clear();
    // Print to the screen
    display.clear();
    display.print("Hum : ");
    display.print(humidity, 1);
    display.print("\t\n");
    display.print("Temp : ");
    display.print(temperature, 1);
    display.print("\t\n");
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    vTaskDelay( pdMS_TO_TICKS( 5000 ) );
  }
}


void setup()
{
Serial.begin(9600);
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);

  dht.setup(17, DHTesp::DHT22); // Connect DHT sensor to GPIO 17


  display.init();

  // display.flipScreenVertically();

  display.setContrast(255);

  xTaskCreate(DHTGet, "DHTGet", 10000, NULL, 1, NULL);
  xTaskCreate(AfficherTexte, "AfficherTexte", 10000, NULL, 2, NULL);
  

  delay(1000);
  display.clear();
}

void loop() {
}
