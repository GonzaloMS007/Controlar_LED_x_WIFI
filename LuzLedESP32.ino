#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebSrv.h"

const char *ssid = "WIFI_GRATIS";
const char *password = "12345678910";
const int LEDPIN = 22; // D22 en el ESP32

AsyncWebServer server(80);

void setup()
{
  // Inicia el monitor serial
  Serial.begin(115200);

  // Configura el pin del LED como salida
  pinMode(LEDPIN, OUTPUT);

  // Conéctate a la red WiFi
  WiFi.begin(ssid, password);

  // Espera a que se conecte a la red WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  // Imprime la dirección IP en el monitor serial cuando está conectado
  Serial.println("Conexión exitosa a WiFi");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());

  // Configura las rutas del servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop()
{
  // Establece el estado del LED en alto (encendido)
  digitalWrite(LEDPIN, HIGH);

  // Opcional: Agrega un pequeño retraso si es necesario
  delay(100);

}


void handleRoot(AsyncWebServerRequest *request)
{
  // Maneja la solicitud de la página principal
  String html = "<html><body><h1>Control LED</h1>";
  html += "<p><a href='/on'><button>Encender LED</button></a></p>";
  html += "<p><a href='/off'><button>Apagar LED</button></a></p>";
  html += "</body></html>";
  request->send(200, "text/html", html);
}

void handleOn(AsyncWebServerRequest *request)
{
  // Maneja la solicitud para encender el LED
  digitalWrite(LEDPIN, HIGH);
  request->send(200, "text/plain", "LED Encendido");
}

void handleOff(AsyncWebServerRequest *request)
{
  // Maneja la solicitud para apagar el LED
  digitalWrite(LEDPIN, LOW);
  request->send(200, "text/plain", "LED Apagado");
}
