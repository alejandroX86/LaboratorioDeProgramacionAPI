#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server(80);

// Configuración como Access Point
const char* ssid = "LaboratorioProgramacionAP";
const char* password = "universidad123";

// Datos del laboratorio de software
const char* labInfo = R"({
  "nombre": "Laboratorio de Programación y Lenguajes",
  "universidad": "Universidad UNPAZ",
  "departamento": "Carrera de LGTI ",
  "profesor": "Ingeniero Gerardo Tulian",
  "ubicacion": "Edificio CEM UNPAZ",
  "descripcion": "Laboratorio equipado para desarrollo de software avanzado, prácticas de ingeniería de software y proyectos de investigación.",
  "equipamiento": "30 computadoras, servidores, equipos de red, herramientas de desarrollo"
})";

// Datos de ejemplo de 50 alumnos
const char* alumnosData = R"([
  {"id":1,"nombre":"Omar Alejandro Bazar","direccion":"Calle Primavera 123","correo":"omar.bazar@email.com"},
  {"id":2,"nombre":"Eduardo Arizza","direccion":"Avenida Central 456","correo":"eduardo.arizza@email.com"},
  {"id":3,"nombre":"Gonzalo Arizza","direccion":"Plaza Mayor 789","correo":"gonzalo.arizza@email.com"},
  {"id":4,"nombre":"Juan Perdon","direccion":"Calle del Sol 321","correo":"juan.perdon@email.com"},
  {"id":5,"nombre":"Luis García Fernández","direccion":"Avenida Norte 654","correo":"juan.garcia@email.com"},
  {"id":6,"nombre":"Sofía Martín Gómez","direccion":"Calle Este 987","correo":"sofia.martin@email.com"},
  {"id":7,"nombre":"Pedro Jiménez Ruiz","direccion":"Avenida Oeste 135","correo":"pedro.jimenez@email.com"},
  {"id":8,"nombre":"Elena Castro Moreno","direccion":"Calle Sur 246","correo":"elena.castro@email.com"},
  {"id":9,"nombre":"Javier Soto Blanco","direccion":"Avenida Libertad 357","correo":"javier.soto@email.com"},
  {"id":10,"nombre":"Laura Méndez Navarro","direccion":"Calle Paz 468","correo":"laura.mendez@email.com"},
  {"id":11,"nombre":"Diego Ortega Serrano","direccion":"Avenida Esperanza 579","correo":"diego.ortega@email.com"},
  {"id":12,"nombre":"Marta Vidal Cortés","direccion":"Calle Arcoiris 680","correo":"marta.vidal@email.com"},
  {"id":13,"nombre":"Alberto Campos Gil","direccion":"Avenida Río 791","correo":"alberto.campos@email.com"},
  {"id":14,"nombre":"Claudia Mora León","direccion":"Calle Montaña 802","correo":"claudia.mora@email.com"},
  {"id":15,"nombre":"Raúl Iglesias Vega","direccion":"Avenida Bosque 913","correo":"raul.iglesias@email.com"},
  {"id":16,"nombre":"Natalia Cruz Pardo","direccion":"Calle Lago 124","correo":"natalia.cruz@email.com"},
  {"id":17,"nombre":"Fernando Gutiérrez Calvo","direccion":"Avenida Mar 235","correo":"fernando.gutierrez@email.com"},
  {"id":18,"nombre":"Patricia Santos Losa","direccion":"Calle Océano 346","correo":"patricia.santos@email.com"},
  {"id":19,"nombre":"Rubén Peña Marcos","direccion":"Avenida Cielo 457","correo":"ruben.pena@email.com"},
  {"id":20,"nombre":"Silvia Fuentes Núñez","direccion":"Calle Estrella 568","correo":"silvia.fuentes@email.com"},
  {"id":21,"nombre":"Hugo Rivas Medina","direccion":"Avenida Luna 679","correo":"hugo.rivas@email.com"},
  {"id":22,"nombre":"Beatriz León Herrera","direccion":"Calle Cometa 780","correo":"beatriz.leon@email.com"},
  {"id":23,"nombre":"Adrián Montesinos Bravo","direccion":"Avenida Galaxia 891","correo":"adrian.montesinos@email.com"},
  {"id":24,"nombre":"Eva Roldán Carrasco","direccion":"Calle Universo 902","correo":"eva.roldan@email.com"},
  {"id":25,"nombre":"Óscar Pacheco Gallardo","direccion":"Avenida Planeta 103","correo":"oscar.pacheco@email.com"},
  {"id":26,"nombre":"Isabel del Valle Carmona","direccion":"Calle Satélite 214","correo":"isabel.valle@email.com"},
  {"id":27,"nombre":"Francisco Javier Mesa Rueda","direccion":"Avenida Asteroide 325","correo":"francisco.mesa@email.com"},
  {"id":28,"nombre":"Alicia Cordero Palacios","direccion":"Calle Meteorito 436","correo":"alicia.cordero@email.com"},
  {"id":29,"nombre":"José Antonio Valverde Espejo","direccion":"Avenida Nebulosa 547","correo":"jose.valverde@email.com"},
  {"id":30,"nombre":"Rosa María Quesada Barrios","direccion":"Calle Agujero Negro 658","correo":"rosa.quesada@email.com"},
  {"id":31,"nombre":"Manuel Ángel Pastor Llamas","direccion":"Avenida Vía Láctea 769","correo":"manuel.pastor@email.com"},
  {"id":32,"nombre":"Teresa Guerrero Espinosa","direccion":"Calle Andrómeda 870","correo":"teresa.guerrero@email.com"},
  {"id":33,"nombre":"Jorge Domínguez Barroso","direccion":"Avenida Orión 981","correo":"jorge.dominguez@email.com"},
  {"id":34,"nombre":"Carmen Rosa Aguilera Corral","direccion":"Calle Perseo 102","correo":"carmen.aguilera@email.com"},
  {"id":35,"nombre":"Álvaro Robles Arenas","direccion":"Avenida Casiopea 213","correo":"alvaro.robles@email.com"},
  {"id":36,"nombre":"Pilar Marín Bernal","direccion":"Calle Centauro 324","correo":"pilar.marin@email.com"},
  {"id":37,"nombre":"Sergio Ferrer Cuesta","direccion":"Avenida Hidra 435","correo":"sergio.ferrer@email.com"},
  {"id":38,"nombre":"Luisa Cano Dorado","direccion":"Calle Dragón 546","correo":"luisa.cano@email.com"},
  {"id":39,"nombre":"Víctor Manuel Reyes Escudero","direccion":"Avenida Fénix 657","correo":"victor.reyes@email.com"},
  {"id":40,"nombre":"Marina Beltrán Figueroa","direccion":"Calle Pegaso 768","correo":"marina.beltran@email.com"},
  {"id":41,"nombre":"Ricardo Flores Galán","direccion":"Avenida Quimera 879","correo":"ricardo.flores@email.com"},
  {"id":42,"nombre":"Esther Hortelano Hervás","direccion":"Calle Minotauro 980","correo":"esther.hortelano@email.com"},
  {"id":43,"nombre":"Alfonso Izquierdo Jover","direccion":"Avenida Esfinge 101","correo":"alfonso.izquierdo@email.com"},
  {"id":44,"nombre":"Concepción Jurado Linares","direccion":"Calle Grifo 212","correo":"concepcion.jurado@email.com"},
  {"id":45,"nombre":"Felipe Lozano Moya","direccion":"Avenida Kraken 323","correo":"felipe.lozano@email.com"},
  {"id":46,"nombre":"Rosario Navarro Ocaña","direccion":"Calle Leviatán 434","correo":"rosario.navarro@email.com"},
  {"id":47,"nombre":"José María Oliva Paredes","direccion":"Avenida Sirena 545","correo":"jose.oliva@email.com"},
  {"id":48,"nombre":"Mercedes Parra Quirós","direccion":"Calle Tritón 656","correo":"mercedes.parra@email.com"},
  {"id":49,"nombre":"Antonio Jesús Ramírez Soler","direccion":"Avenida Unicornio 767","correo":"antonio.ramirez@email.com"},
  {"id":50,"nombre":"Dolores Sierra Téllez","direccion":"Calle Yeti 878","correo":"dolores.sierra@email.com"}
])";

DynamicJsonDocument alumnosDoc(4096);
int nextId = 51;

// Función para enviar respuesta con headers CORS
void sendCORSResponse(int code, const String& contentType, const String& content) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(code, contentType, content);
}

void setup() {
  Serial.begin(9600);
  
  // Configurar como Access Point
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Inicializar datos de alumnos
  deserializeJson(alumnosDoc, alumnosData);

  // Configurar rutas de la API
  server.on("/", HTTP_GET, []() {
    sendCORSResponse(200, "text/plain", "API del Laboratorio de Programacion y Lenguajes - Universidad UNPAZ");
  });

  // Manejar opciones CORS (preflight)
  server.on("/", HTTP_OPTIONS, []() {
    sendCORSResponse(200, "text/plain", "");
  });

  // Obtener información del laboratorio
  server.on("/api/laboratorio", HTTP_GET, []() {
    sendCORSResponse(200, "application/json", labInfo);
  });

  server.on("/api/laboratorio", HTTP_OPTIONS, []() {
    sendCORSResponse(200, "text/plain", "");
  });

  // Obtener todos los alumnos
  server.on("/api/alumnos", HTTP_GET, []() {
    String output;
    serializeJson(alumnosDoc, output);
    sendCORSResponse(200, "application/json", output);
  });

  server.on("/api/alumnos", HTTP_OPTIONS, []() {
    sendCORSResponse(200, "text/plain", "");
  });

  // Obtener un alumno por ID
  server.on("/api/alumnos/", HTTP_GET, []() {
    String idStr = server.uri().substring(12);
    int id = idStr.toInt();

    for (JsonObject alumno : alumnosDoc.as<JsonArray>()) {
      if (alumno["id"] == id) {
        String output;
        serializeJson(alumno, output);
        sendCORSResponse(200, "application/json", output);
        return;
      }
    }
    
    sendCORSResponse(404, "application/json", "{\"error\":\"Alumno no encontrado\"}");
  });

  server.on("/api/alumnos/", HTTP_OPTIONS, []() {
    sendCORSResponse(200, "text/plain", "");
  });

  // Crear un nuevo alumno
  server.on("/api/alumnos", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      DynamicJsonDocument newAlumno(256);
      deserializeJson(newAlumno, server.arg("plain"));
      
      JsonObject alumno = alumnosDoc.createNestedObject();
      alumno["id"] = nextId++;
      alumno["nombre"] = newAlumno["nombre"].as<String>();
      alumno["direccion"] = newAlumno["direccion"].as<String>();
      alumno["correo"] = newAlumno["correo"].as<String>();
      
      String output;
      serializeJson(alumno, output);
      sendCORSResponse(201, "application/json", output);
    } else {
      sendCORSResponse(400, "application/json", "{\"error\":\"Datos inválidos\"}");
    }
  });

  // Actualizar un alumno
  server.on("/api/alumnos/", HTTP_PUT, []() {
    String idStr = server.uri().substring(12);
    int id = idStr.toInt();

    if (server.hasArg("plain")) {
      DynamicJsonDocument updatedAlumno(256);
      deserializeJson(updatedAlumno, server.arg("plain"));
      
      for (JsonObject alumno : alumnosDoc.as<JsonArray>()) {
        if (alumno["id"] == id) {
          alumno["nombre"] = updatedAlumno["nombre"].as<String>();
          alumno["direccion"] = updatedAlumno["direccion"].as<String>();
          alumno["correo"] = updatedAlumno["correo"].as<String>();
          
          String output;
          serializeJson(alumno, output);
          sendCORSResponse(200, "application/json", output);
          return;
        }
      }
      
      sendCORSResponse(404, "application/json", "{\"error\":\"Alumno no encontrado\"}");
    } else {
      sendCORSResponse(400, "application/json", "{\"error\":\"Datos inválidos\"}");
    }
  });

  // Eliminar un alumno
  server.on("/api/alumnos/", HTTP_DELETE, []() {
    String idStr = server.uri().substring(12);
    int id = idStr.toInt();

    JsonArray alumnosArray = alumnosDoc.as<JsonArray>();
    for (size_t i = 0; i < alumnosArray.size(); i++) {
      if (alumnosArray[i]["id"] == id) {
        alumnosArray.remove(i);
        sendCORSResponse(200, "application/json", "{\"message\":\"Alumno eliminado\"}");
        return;
      }
    }
    
    sendCORSResponse(404, "application/json", "{\"error\":\"Alumno no encontrado\"}");
  });

  // Manejar rutas no encontradas
  server.onNotFound([]() {
    sendCORSResponse(404, "application/json", "{\"error\":\"Ruta no encontrada\"}");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}