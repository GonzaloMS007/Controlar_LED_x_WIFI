#include <WiFi.h> // Incluimos la librería WiFi para la gestión de la conexión Wi-Fi

const char* ssid     = "FAMILIA_MARTINEZ";  // Nombre de la red Wi-Fi
const char* password = "74296000";  // Contraseña de la red Wi-Fi

WiFiServer server(80);  // Creamos un objeto servidor Wi-Fi en el puerto 80

void setup()
{
    Serial.begin(115200);  // Inicializamos la comunicación serie con una velocidad de 115200 bps
    pinMode(5, OUTPUT);      // Establecemos el modo del pin del LED
    delay(10);
    // Comenzamos conectándonos a una red WiFi
    Serial.println();
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);  // Iniciamos la conexión a la red Wi-Fi con el nombre y la contraseña proporcionados

    while (WiFi.status() != WL_CONNECTED) {  // Esperamos a que la conexión Wi-Fi se establezca
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado.");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
    server.begin();  // Iniciamos el servidor Wi-Fi en el puerto 80
}

void loop(){
 WiFiClient client = server.available();   // esperar clientes entrantes

  if (client) {                             // si se recibe un cliente,
    Serial.println("Nuevo Cliente.");           // imprimir un mensaje por el puerto serie
    String currentLine = "";                // hacer una cadena para almacenar los datos entrantes del cliente
    while (client.connected()) {            // bucle mientras el cliente esté conectado
      if (client.available()) {             // si hay bytes para leer del cliente,
        char c = client.read();             // leer un byte, luego
        Serial.write(c);                    // imprimirlo por el monitor serie
        if (c == '\n') {                    // si el byte es un carácter de nueva línea
          // si la línea actual está en blanco, se tienen dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que enviar una respuesta:
          if (currentLine.length() == 0) {
            // Las cabeceras HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK)
            // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // el contenido de la respuesta HTTP sigue a la cabecera:
            client.print("Haz clic <a href=\"/H\">aqui</a> para encender el LED en el pin 5.<br>");
            client.print("Haz clic <a href=\"/L\">aqui</a> para apagar el LED en el pin 5.<br>");

            // La respuesta HTTP termina con otra línea en blanco:
            client.println();
            // salir del bucle while:
            break;
          } else {    // si se recibe una nueva línea, entonces borrar currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // si se recibe cualquier cosa que no sea un carácter de retorno de carro,
          currentLine += c;      // agregarlo al final de currentLine
        }

        // Comprobar si la solicitud del cliente fue "GET /H" o "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);               // GET /H enciende el LED
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L apaga el LED
        }
      }
    }
    // cerrar la conexión:
    client.stop();
    Serial.println("Cliente Desconectado.");
  }
}
