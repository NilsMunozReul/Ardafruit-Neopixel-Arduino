#include <WiFi.h>


#define WIFI_SSID "Redmi Note 13 5G" 
#define WIFI_PASS "hnxiqxdd9kg4i2k"  

// Static IP configuration
IPAddress staticIP(192,168,214,188); // ESP32 static IP
IPAddress gateway(192,168,214,127);    // IP Address of your network gateway (router)
IPAddress subnet(255, 255, 255, 0);   // Subnet mask

// Configura UDP
WiFiUDP udp;
const int udpPort = 8888;  // Puerto en el que escuchará la ESP32
char packetBuffer[255];  // Buffer para almacenar el mensaje recibido
int packetCount = 0; 
 unsigned long lastCheckTime = 0; 


void setup() {
  Serial.begin(9600);
  Serial.println ("Inicio"); 
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS );
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Conectado a WiFi.");

  // Configuring static IP
  if(!WiFi.config(staticIP, gateway, subnet)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address to Serial Monitor

  //iniciar UDP
  udp.begin(udpPort);
    Serial.print("Escuchando en el puerto UDP: ");
    Serial.println(udpPort);
  

  
  

}
void loop() {
  int packetSize = udp.parsePacket();  // Verifica si llegó un paquete
  if (packetSize) {
    packetCount++;

    Serial.print("Paquete recibido de ");
        Serial.print(udp.remoteIP());
        Serial.print(":");
        Serial.println(udp.remotePort());

        // Leer el paquete
        int len = udp.read(packetBuffer, 255);
        if (len > 0) {
            packetBuffer[len] = '\0'; // Terminar la cadena correctamente
        }

        Serial.println("Contenido:");
        Serial.println(packetBuffer);

        // Responder con un mensaje de confirmación
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print("Mensaje recibido");
        udp.endPacket();
    }

    if (millis()-lastCheckTime> 60000){
      if (packetCount ==0){
        Serial.println ("No se ha recibido ningún paquete en el último minuto."); 
      }
      packetCount = 0; 
      lastCheckTime = millis(); 
    }

    delay(10);
}


