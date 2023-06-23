/*                                                                      Medidor de temperatura y humedad
 * Descripción:
 *
 * El presente programa permite controlar con un ESP32 DEVKit una LCD 16x2, y en ella visualizar un mensaje, la temperatura
 * en grados centigrados y la humedad.
 *
 * Para la adquisición de la temperatura y la humedad se utiliza el sensor DHT11.
 *
 * Líneas de conexión entre el Arduino UNO, la LCD 16x2, el POT de 10K y el resistor de 10K
 *
 *      GPIO (ESP32 DevKit)      LCD16x2                 POT 10K                   Res. 330                  DHT11
 *           19 -----------------> D4
 *           21 -----------------> D5
 *           22 -----------------> D6
 *           23 -----------------> D7
 *          VCC --------------------------------------------------------------------------------------------- 1
 *            4 <-------------------------------------------------------------------------------------------- 2 con un resistor de 10Kohms en pull-up
 *                                                                                                            3 Sin conexión
 *          GND --------------------------------------------------------------------------------------------- 4
 *           13 ----------------->  E 
 *           12 -----------------> RS
 *          GND -----------------> RW
 *          GND -----------------> (-) led BackLight
 *          VCC -----------------> (+) led BackLight ----------------------------- terminal 1              
 *          GND -----------------> VSS ------------  terminal derecha                                      
 *          VCC -----------------> VDD ------------  terminal izquierda
 *                                 VEE ------------- terminal central
 *          
 *
 *
 * Autor: Omar Abundis
 * Grupo: EC09S
 * Fecha: 20 de febrero del 2023
 *
 *
 * Referencias:
 *
 * https://www.arduino.cc/reference/en/libraries/liquidcrystal/?_gl=1*il6bs8*_ga*NDIzMjM0MTE2LjE2NzU3MTEwODU.*_ga_NEXN8H46L5*MTY3NTcxMTA4NS4xLjEuMTY3NTcxMTEzMC4wLjAuMA..
 * https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf
 * https://github.com/adafruit/DHT-sensor-library
 * https://cplusplus.com/reference/cmath/isnan/
 * https://www.arduino.cc/reference/en/language/structure/control-structure/return/
 *
 */
  #include <Arduino.h>
  #include <Adafruit_Sensor.h>
  #include <DHT.h>
  #include <DHT_U.h>
  #include <LiquidCrystal.h>                             // Biblioteca de control 
                                                         // LiquidCrystal Constructor.
  LiquidCrystal lcd(12,13,19,21,22,23);                      // LiquidCrystal (RS,E,D4,D5,D6,D7)

  #define DHTPIN 4                                       // terminal para introducir la señal del sensor DHT11
  #define DHTTYPE DHT11                                  // definición del sensor a utilizar de las soportadas por la biblioteca DHT.h

  DHT dht(DHTPIN, DHTTYPE);                              // Objeto

  unsigned long previousTime = 0;                          // Para el control no bloquante
  unsigned long currentTime;
  long tiempo = 3000;

  float humedad;
  float temperatura;

  void setup() {

    Serial.begin(115200);
    lcd.begin(16,2);
    dht.begin();
  }

  void loop() {

  currentTime = millis();
  if(currentTime - previousTime >= tiempo) {
    previousTime = currentTime;

    humedad = dht.readHumidity();
    temperatura = dht.readTemperature();

    if (isnan(humedad) || isnan(temperatura)) {        // Para detección de fallas de comunicación con el DHT11
    
      lcd.setCursor(0,0);                              // columna 0 fila 0
      lcd.print("Falla de lectura");
      lcd.setCursor(0,1);                              // columna 0 fila 0
      lcd.print("del sensor DHT11");
      //Serial.println(F("Failed to read from DHT sensor!"));
      return;                                          // Terminate a function and return a value from a function to the calling function, if desired.
    }
    lcd.clear();
    lcd.setCursor(0,0);                             
    lcd.print("Temp. = ");
    lcd.print(temperatura);
    lcd.print(" C");

    lcd.setCursor(0,1);                              
    lcd.print("Hum. = ");
    lcd.print(humedad);
    lcd.print(" %");

    Serial.print("humedad = ");
      Serial.print(humedad);
    Serial.print("%, ");
    Serial.print("temperatura = ");
    Serial.print(temperatura);
    Serial.print("°C");
    Serial.println(" ");
  }
}