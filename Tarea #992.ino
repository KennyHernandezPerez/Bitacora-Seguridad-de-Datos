#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char* ssid = "MyESP32AP";
const char* password = "password";

AsyncWebServer server(80);

struct User {
  String name;
  String password;
  String accessKey;
};

User users[20] = {{"root", "root", "root"}};
int numUsers = 1; // User counter, initialized to 1
bool rootAuthenticated = false; 
bool userAuthenticated = false; 

String generateRandomAccessKey() {
  const char* charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  String accessKey;
  for (int i = 0; i < 8; i++) {
    accessKey += charset[random(0, strlen(charset))];
  }
  return accessKey;
}

String generateRandomText() {
  String textoAleatorio = "Había una vez en la profunda selva tropical, un grupo de animales que vivían en armonía. El líder de este grupo era un sabio y viejo elefante llamado Elu. Elu era conocido por su inteligencia y su paciencia, y todos los animales de la selva lo respetaban. ";
  textoAleatorio += "Un día, un pequeño conejo llamado Rabi llegó corriendo al lugar de reunión de los animales. Estaba sin aliento y parecía asustado. \"¡Elu, Elu!\", gritó. \"¡El río se está desbordando! ¡Nuestros hogares están en peligro!\" ";
  textoAleatorio += "Elu levantó su larga trompa y pidió silencio. Cuando todos los animales se calmaron, dijo: \"No debemos entrar en pánico. Debemos trabajar juntos para salvar nuestros hogares. Rabi, ve y trae a todos los castores. Ellos pueden ayudarnos a construir una barrera para detener el agua\". ";
  textoAleatorio += "Y así, todos los animales de la selva trabajaron juntos para salvar sus hogares del desbordamiento del río. A pesar de sus diferencias, se unieron en tiempos de crisis y demostraron que juntos, pueden superar cualquier desafío.";

  return textoAleatorio;
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // Monta el sistema de archivos SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Cargar usuarios desde el archivo en SPIFFS
  loadUsers();

  // Agrega el usuario "root" al arreglo
  addUser("root", "root", "root");

  // Set up the access point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Set up the HTML server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Send HTML page to the client
    request->send(200, "text/html", "<html><body><h1>Login</h1> <form action='/login' method='post'><label for='name'>Name:</label><input type='text' name='name' required><br><label for='password'>Password:</label><input type='password' name='password' required><br><input type='submit' value='Login'></form><br><a href='/register'>Register</a></body></html>");
  });

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
    String name = request->arg("name");
    String password = request->arg("password");

    // Verify credentials against the users array
    for (int i = 0; i < numUsers; i++) {
      if (users[i].name == name && users[i].password == password) {
        if (name == "root" && password == "root") {
          rootAuthenticated = true;
          request->redirect("/users");
        } else {
          rootAuthenticated = false;
          userAuthenticated = true;
          request->redirect("/authenticate_access_key");
        }
        return;
      }
    }
    // If credentials are incorrect, redirect the user to the registration form
    request->redirect("/register");
  });

  server.on("/register", HTTP_GET, [](AsyncWebServerRequest *request){
    String randomAccessKey = generateRandomAccessKey(); // Genera una clave de acceso aleatoria
    request->send(200, "text/html", "<html><body><h1>Register</h1><form action='/register' method='post'><label for='name'>Name:</label><input type='text' name='name' required><br><label for='password'>Password:</label><input type='password' name='password' required><br><label for='accessKey'>Access Key:</label><input type='text' name='accessKey' value='" + randomAccessKey + "' readonly><br><input type='submit' value='Register'></form><br><a href='/'>Back to Login</a></body></html>");
  });

  server.on("/register", HTTP_POST, [](AsyncWebServerRequest *request){
      String name = request->arg("name");
      String password = request->arg("password");
      String accessKey = request->arg("accessKey");

      // Verifica si hay campos vacíos
      if (name.isEmpty() || password.isEmpty() || accessKey.isEmpty()) {
        request->send(400, "text/plain", "Name, password, and access key are required");
        return;
      }

      // Almacena nuevos usuarios en el arreglo y en el archivo
      if (numUsers < sizeof(users)/sizeof(users[0])) {
        addUser(name, password, accessKey); // Agrega el nuevo usuario al arreglo
        saveUser(name, password, accessKey); // Guarda el nuevo usuario en el archivo
        request->send(200, "text/html", "<html><body><h1>Registration successful!</h1><br><a href='/'>Login</a></body></html>");
      } else {
        request->send(500, "text/plain", "Too many users registered");
      }
    });

  // Route to see the list of users (requires root authentication)
  server.on("/users", HTTP_GET, [](AsyncWebServerRequest *request){
    if (rootAuthenticated) {
      String response = "<html><body><h1>Registered Users</h1><ul>";

      // Iterate over the users array and add each user to the response
      for (int i = 0; i < numUsers; i++) {
        response += "<li>Name: " + users[i].name + ", Password: " + users[i].password + ", Access Key: " + users[i].accessKey + "</li>";
      }

      response += "</ul></body></html>";
      request->send(200, "text/html", response);
    } else {
      request->send(401, "text/html", "<html><body><h1>Unauthorized access!</h1></body></html>");
    }
  });

  // Route to authenticate access key (for non-root users)
  server.on("/authenticate_access_key", HTTP_GET, [](AsyncWebServerRequest *request){
    if (userAuthenticated) {
      request->send(200, "text/html", "<html><body><h1>Authenticate Access Key</h1><form action='/authenticate_access_key' method='post'><label for='accessKey'>Access Key:</label><input type='text' name='accessKey' required><br><input type='submit' value='Authenticate'></form></body></html>");
    } else {
      request->send(401, "text/html", "<html><body><h1>Unauthorized access!</h1></body></html>");
    }
  });

  server.on("/authenticate_access_key", HTTP_POST, [](AsyncWebServerRequest *request){
    String accessKey = request->arg("accessKey");

    // Verify access key against the user's access key
    for (int i = 0; i < numUsers; i++) {
      if (users[i].accessKey == accessKey) {
        request->redirect("/random_text");
        return;
      }
    }

    // If access key is incorrect, redirect user to recover access key page
    request->redirect("/recover_access_key");
  });

  // Route to recover access key (for users)
  server.on("/recover_access_key", HTTP_GET, [](AsyncWebServerRequest *request){
    String recoverForm = "<html><body><h1>Recover Access Key</h1><form action='/recover_access_key' method='post'><label for='name'>Name:</label><input type='text' name='name' required><br><label for='password'>Password:</label><input type='password' name='password' required><br><input type='submit' value='Recover'></form><br><a href='/check_access'>Check Access</a></body></html>";
    request->send(200, "text/html", recoverForm);
  });

  server.on("/recover_access_key", HTTP_POST, [](AsyncWebServerRequest *request){
    String name = request->arg("name");
    String password = request->arg("password");

    // Verify credentials against the users array
    for (int i = 0; i < numUsers; i++) {
      if (users[i].name == name && users[i].password == password) {
        request->send(200, "text/html", "<html><body><h1>Your Access Key</h1><p>Your access key is: " + users[i].accessKey + "</p></body></html>");
        return;
      }
    }

    // If credentials are incorrect, redirect to the check access page
    request->redirect("/check_access");
  });

  // Route to generate random text (requires user authentication)
  server.on("/random_text", HTTP_GET, [](AsyncWebServerRequest *request){
    if (userAuthenticated) {
      request->send(200, "text/plain", generateRandomText());
    } else {
      request->redirect("/authenticate_access_key");
    }
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404);
  });

  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
}

void loadUsers() {
  // Abre el archivo de usuarios
  File file = SPIFFS.open("/users.txt", "r");
  if (!file) {
    Serial.println("Error al abrir el archivo de usuarios");
    return;
  }

  // Lee los usuarios del archivo
  while (file.available()) {
    String name = file.readStringUntil(',');
    String password = file.readStringUntil(',');
    String accessKey = file.readStringUntil('\n');

    if (name != "" && password != "" && accessKey != "") {
      // Agrega el usuario al arreglo
      addUser(name, password, accessKey);
    }
  }

  // Cierra el archivo
  file.close();
}

void addUser(String name, String password, String accessKey) {
  // Verifica si el usuario ya está registrado
  for (int i = 0; i < numUsers; i++) {
    if (users[i].name == name) {
      return; // No agrega el usuario si ya está registrado
    }
  }

  // Agrega el nuevo usuario al arreglo
  users[numUsers++] = {name, password, accessKey};
}

void saveUser(String name, String password, String accessKey) {
  // Abre el archivo de usuarios en modo escritura, anexando al final
  File file = SPIFFS.open("/users.txt", "a");
  if (!file) {
    Serial.println("Error al abrir el archivo de usuarios para escritura");
    return;
  }

  // Escribe el nuevo usuario en el archivo
  file.print(name);
  file.print(',');
  file.print(password);
  file.print(',');
  file.println(accessKey);

  // Cierra el archivo
  file.close();
}