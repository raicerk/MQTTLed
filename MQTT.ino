#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

const int Led  = 4;

const char* mqtt_server = "mqtt.clubarduino.cl";

char message_buff[100];

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
IPAddress ip(192, 168, 0, 20);
IPAddress dnServer(192, 168, 0, 240);
IPAddress gateway(192, 168, 0, 2);
IPAddress subnet(255, 255, 255, 0);

EthernetClient ethClient;
PubSubClient client(ethClient);

void setup(){
  
  Serial.begin(9600);

  pinMode(Led, OUTPUT);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  
  Conectar();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("algo");
  int i = 0;
  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);

  if (msgString.equals("Enciende")) {
    digitalWrite(Led, HIGH);
  }else if(msgString.equals("Apaga")){
    digitalWrite(Led, LOW);
  }
}

void loop(){
  while (!client.connected()) {
    Conectar();
  }
  client.loop();  
}

void Conectar(){
  if (client.connect("ArduinoLeonardoRaicerk")) {
      Serial.println("Conectado");
      client.publish("Raicerk/Luz", "Hola, soy Juan");
      client.subscribe("Raicerk/Luz");
    } else {
      Serial.println("Cliente se a Caido");
      delay(5000);
    }
}

