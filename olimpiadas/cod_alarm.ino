#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Configura tu red WiFi
const char* ssid = "nombre_de_tu_red_wifi";
const char* password = "contraseña_de_tu_red_wifi";

// Datos de la base de datos MySQL
IPAddress serverIP(192, 168, 1, 100); // Cambia esto a la dirección IP de tu servidor MySQL
char user[] = "nombre_de_usuario_mysql";
char passwordDB[] = "contraseña_mysql";
char db[] = "nombre_de_tu_base_de_datos";

// Pin del botón
const int buttonPin = D1; // Cambia esto al pin que estés utilizando
int buttonState = 0;
int lastButtonState = 0;

// Inicializar el LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Cambia la dirección I2C si es diferente

void setup() {
  Serial.begin(115200);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión WiFi exitosa");

  // Inicializar el LCD
  lcd.init();
  lcd.backlight();

  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    unsigned long currentMillis = millis();
    unsigned long seconds = currentMillis / 1000;
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secondsDisplay = seconds % 60;
    String hora = String(hours) + ":" + String(minutes) + ":" + String(secondsDisplay);

    String codigo = String(random(1000, 9999)); // Genera un código aleatorio

    // Mostrar en el LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarma Activada");
    lcd.setCursor(0, 1);
    lcd.print("Código: " + codigo);
    
    // Enviar a la base de datos MySQL
    sendToMySQL(codigo, hora);
  }

  lastButtonState = buttonState;
}

void sendToMySQL(String codigo, String hora) {
  WiFiClient client;
  MySQL_Connection conn(&client);
  
  if (conn.connect(serverIP, 3306, user, passwordDB, db)) {
    Serial.println("Conexión a la base de datos exitosa");
    delay(1000);

    char INSERT_SQL[] = "INSERT INTO tu_tabla (codigo, hora) VALUES (?, ?)";
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->prepareStatement(INSERT_SQL);
    cur_mem->setInt(1, codigo.toInt());
    cur_mem->setDateTime(2, hora);
    cur_mem->executeStatement();
    
    delete cur_mem;
    conn.close();
  } else {
    Serial.println("Error en la conexión a la base de datos");
  }
}