/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://github.com/khoih-prog/WiFiWebServer
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://github.com/luisllamasbinaburo/ESP32-Examples
  https://www.luisllamas.es/esp8266-esp32/
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "ESP32-19389";  // Enter SSID here
const char* password = "lechepapu";  //Enter Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

int spotqty;

#define TXD2 17
#define RXD2 16

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);

  server.on("/", handle_OnConnect); // página de inicio
  server.on("/spot", handle_spts); // handlers indicadores de spots
 
  
  server.onNotFound([]() {                  // si el cliente solicita una uri desconocida
    if (!HandleFileRead(server.uri()))      // enviar archivo desde SPIFF, si existe
      handleNotFound();             // sino responder con un error 404 (no existe)
  });
  //server.onNotFound(handle_NotFound); //si no se encuentra el uri, responder con error 404 (no existe)

  server.begin(); // iniciar servidor
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient(); // escuchar solicitudes de clientes

  if (Serial2.available()) {
    // read the incoming byte:
    spotqty = Serial2.read();
  }

  
}
//************************************************************************************************
// Handler de página de inicio
//************************************************************************************************
void handle_OnConnect() {
  handle_spts();
}
//************************************************************************************************
// Handler de spots
//************************************************************************************************
void handle_spts() {
  server.send(200, "text/html", SendHTML(spotqty)); //responde con un OK (200) y envía HTML
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t qttyspot) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Parqueos Disponibles S1</title>\n";
  ptr += "<style>html { font-family: Verdana; display: inline-block; margin: 100px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".dpb {background-color: #e67e22;}\n";
  ptr += ".dpb:active {background-color: #efc60f;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>&#128677 Parqueos S&G &#128677</h1>\n";
  ptr += "<h2>&#127949 &#128663 &#128690 &#129469 &#128657</h2>\n";
  ptr += "<h3>Sotano evaluado: 1</h3>\n";
  ptr += "<p>Cantidad de espacios disponibles:</p>\n";

  if (qttyspot == 0)
  {
    ptr += "<h2>0</h2><a class=\"button button-on\" href=\"/\">UPDATE</a>\n";
  }
  if (qttyspot == 1)
  {
    ptr += "<h2>1</h2><a class=\"button button-on\" href=\"/\">UPDATE</a>\n";
  }
  if (qttyspot == 2)
  {
    ptr += "<h2>2</h2><a class=\"button button-on\" href=\"/\">UPDATE</a>\n";
  }
  if (qttyspot == 3)
  {
    ptr += "<h2>3</h2><a class=\"button button-on\" href=\"/\">UPDATE</a>\n";
  }
  if (qttyspot == 4)
  {
    ptr += "<h2>4</h2><a class=\"button button-on\" href=\"/\">UPDATE</a>\n";
  }
  
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
