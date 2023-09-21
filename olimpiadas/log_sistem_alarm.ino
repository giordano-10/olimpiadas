#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Tone.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Configuración WiFi
#define WIFI_SSID "nombre_de_tu_red_wifi"
#define WIFI_PASSWORD "contraseña_de_tu_red_wifi"

// Configuración de Firebase
#define FIREBASE_HOST "tuproyecto.firebaseio.com"
#define FIREBASE_AUTH "tu_token_de_autenticacion"

// Pin del botón
const int buttonPin = D1; // Cambia esto al pin que estés utilizando
int buttonState = 0;
int lastButtonState = 0;

// Pin para el zumbador (buzzer)
const int buzzerPin = D2; // Cambia esto al pin que estés utilizando
Tone tone1;

// Inicializar el LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Cambia la dirección I2C si es diferente

// Inicializar el módulo GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // Usa los pines RX2 (D7) y TX2 (D8) para el módulo GPS

// BLE
BLEServer *pServer;
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

void setup() {
  Serial.begin(115200);

  // Inicializar WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión WiFi exitosa");

  // Inicializar Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Configurar el pin del botón como entrada
  pinMode(buttonPin, INPUT);
  
  // Configurar el pin del zumbador como salida
  pinMode(buzzerPin, OUTPUT);

  // Inicializar el LCD
  lcd.init();
  lcd.backlight();

  // Inicializar el módulo GPS
  gpsSerial.begin(9600);

  // BLE
  BLEDevice::init("ESP8266_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setValue("Hola, este es el ESP8266 BLE.");
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // Leer el estado del botón
  buttonState = digitalRead(buttonPin);

  // Si el botón se presiona (estado bajo)
  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.println("Botón presionado");

    // Hacer sonar la alarma
    tone1.begin(buzzerPin);
    tone1.play(1000); // Generar un tono a 1000Hz
    delay(1000);      // Sonar la alarma durante 1 segundo
    tone1.stop();     // Detener el tono

    // Leer datos GPS
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isUpdated()) {
      // Obtener la ubicación
      float lat = gps.location.lat();
      float lon = gps.location.lng();

      // Obtener la hora
      String hora = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());

      // Mostrar en el LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarma Activada");
      lcd.setCursor(0, 1);
      lcd.print("Hora: " + hora);
      delay(5000); // Mostrar en el LCD durante 5 segundos

      // Enviar información a la base de datos MySQL o Firebase
      String alarmaData = "Código: 123, Lugar: " + String(lat, 6) + "," + String(lon, 6) + ", Hora: " + hora;
      Firebase.setString("alarma", alarmaData);
    }
  }

  // Guardar el estado actual del botón
  lastButtonState = buttonState;

  // BLE
  if (deviceConnected) {
    // Actualiza el valor de la característica BLE si está conectado
    pCharacteristic->setValue("Conectado");
    pCharacteristic->notify();
  }
}