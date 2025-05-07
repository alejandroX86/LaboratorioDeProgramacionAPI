# 🏫 Sistema de Gestión de Alumnos UNPAZ - Laboratorio de Programación y Lenguajes



> **Materia:** Laboratorio de Programación y Lenguajes  
> **Profesor:** Ing. Gerardo Tulian  
> **Desarrollado por:** Eduardo Arizza, Gonzalo Arizza, Omar Bazar  
> **Año:** 2025  
> **Tecnologías:** ESP8266, JavaScript Funcional, HTML5, CSS3, REST API



## 🌟 Descripción del Proyecto
Sistema full-stack para gestión de alumnos con:
- **Frontend**: Aplicación web responsive con programación funcional pura
- **Backend**: API REST en microcontrolador ESP8266
- **Persistencia**: Datos en formato JSON en memoria



## 🛠️ Características Técnicas

### Frontend (JavaScript Funcional)
```javascript
// Ejemplo de función pura
const highlightMatch = (text, term) => 
  term ? text.replace(new RegExp(term, 'gi'), ...) : text;
// Ejemplo de endpoint
server.on("/api/alumnos", HTTP_GET, []() {
  serializeJson(alumnosDoc, output);
  sendCORSResponse(200, "application/json", output);
});
✔️ API REST con CORS
✔️ CRUD completo
✔️ JSON como formato de datos

🔧 Instalación y Configuración
Requisitos
NodeMCU ESP8266

Arduino IDE 1.8+

Navegador moderno

Pasos
Cargar firmware al ESP8266:
# 1. Abrir sketch_may6a.ino en Arduino IDE
# 2. Seleccionar placa: NodeMCU 1.0 (ESP-12E Module)
# 3. Subir código

Conectar al Access Point:
SSID: LaboratorioProgramacionAP
Password: universidad123

Abrir Frontend:
# Simplemente abrir index.html en cualquier navegador

Funcionalidades Principales
Operación	Cómo realizarla
Agregar alumno	Formulario inferior → Guardar
Editar alumno	Click en botón "Editar"
Buscar	Usar campos superiores (ID o Nombre)
Eliminar	Click en "Eliminar" + confirmación


🧩 Estructura del Código
Archivos Principales

├── index.html        # Interfaz principal
├── styles.css        # Estilos CSS
├── app.js            # Lógica frontend (funcional)
└── sketch_may6a.ino  # Código ESP8266 (API)

Componentes Clave
Estado de la Aplicación (app.js)

javascript

const createAppState = () => {
  let allAlumnos = [];
  return {
    getAllAlumnos: () => [...allAlumnos],
    setAllAlumnos: (alumnos) => { allAlumnos = [...alumnos]; }
  };
};

API Endpoints (sketch_may6a.ino)

// GET /api/alumnos
// POST /api/alumnos
// PUT /api/alumnos/{id}
// DELETE /api/alumnos/{id}

📡 API Endpoints
Método	Endpoint	Descripción
GET	/api/alumnos	Obtiene todos los alumnos
POST	/api/alumnos	Crea nuevo alumno
PUT	/api/alumnos/{id}	Actualiza alumno
DELETE	/api/alumnos/{id}	Elimina alumno
Ejemplo de Request:

POST /api/alumnos
{
  "nombre": "Nuevo Alumno",
  "direccion": "Calle 123",
  "correo": "alumno@unpaz.edu.ar"
}


🤝 Contribuciones
Este proyecto fue creado para compartir conocimiento con los compañeros. ¡Te invitamos a:

Hacer fork del proyecto

Implementar mejoras como:

Persistencia en EEPROM/SPIFFS

Autenticación JWT

Más campos para alumnos

Ejemplo de mejora:

// En sketch_may6a.ino
void saveToEEPROM() {
  EEPROM.begin(4096);
  serializeJson(alumnosDoc, EEPROM);
  EEPROM.commit();
}

📜 Licencia
Este proyecto es educativo y de código abierto. Se permite su uso siempre que se mantengan los créditos.

⌨️ con ❤️ por [Eduardo] - [Gonzalo] - [Omar] para UNPAZ 2025






