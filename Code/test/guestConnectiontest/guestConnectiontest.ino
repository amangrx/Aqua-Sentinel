#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "Galaxy A22";
const char* password = "9856024196";

// Pin assignments for various components
const int buzzerPin = 12;
const int damPin = 13;
const int waterStoragePin = 14;

// Web server instance
WebServer server(80);

// HTML content for the login page
const char* htmlContent1 = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Aqua Sentinel Login</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(to right, #3498db, #2c3e50);
      margin: 0;
      padding: 0;
      display: flex;
      align-items: center;
      justify-content: center;
      height: 100vh;
    }

    .login-container {
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 0 0 20px rgba(0, 0, 0, 0.2);
      padding: 40px;
      text-align: center;
    }

    h2 {
      color: #3498db;
    }

    .login-button {
      background-color: #3498db;
      color: #fff;
      border: none;
      padding: 12px 24px;
      margin: 10px;
      border-radius: 5px;
      cursor: pointer;
      font-size: 18px;
      transition: background-color 0.3s;
    }

    .login-button:hover {
      background-color: #2980b9;
    }

    .dam-icon {
      font-size: 50px;
      color: #3498db;
      margin-bottom: 20px;
    }

    /* Responsive styling for smaller screens */
    @media (max-width: 600px) {
      .login-container {
        width: 80%;
      }
    }
  </style>
</head>
<body>

<div class="login-container">
  <span class="dam-icon">🌊</span>
  <h2>Smart Dam System</h2>
  
  <button class="login-button" onclick="loginAsAdmin()">Admin</button>
  <button class="login-button" onclick="loginAsGuest()">Guest</button>
</div>

<script>
  function loginAsAdmin() {
    var password = prompt("Enter the password for admin:");
    if (password === "POKEMON") {
      alert("In Maintainance. Please Try Again in 3months");
      // Redirect to the admin page or perform admin-specific actions
      window.location.href = "/handleAdmin";
    } else {
      alert("Incorrect password. Admin login failed.");
    }
  }

  function loginAsGuest() {
    alert("Guest login successful!");
    // Redirect to the code.html page
    window.location.href = "/aquasentinel";
  }
</script>

</body>
</html>

)=====";

// HTML content for the status page
const char* htmlContent3 = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Status Page</title>
 <style>
        body {
            text-align: center;
            padding: 50px;
            background: #add8e6; /* LightBlue */
            border: 2px solid #4682b4; /* SteelBlue */
            border-radius: 10px;
        }

        h1 {
            color: #0066cc; /* RoyalBlue */
        }

        .status {
            font-size: 24px;
            font-weight: bold;
            margin-bottom: 20px;
        }

        .on {
            color: #008000; /* Green */
        }

        .off {
            color: #ff0000; /* Red */
        }

        #buzzerStatus, #damStatus, #waterStorageStatus {
            background-color: #e0f7fa; /* Light Cyan */
            padding: 5px;
            border-radius: 5px;
            display: inline-block;
        }
    </style>
</head>
<body>

    <h1>Status Page</h1>
    
    <p class="status">Dam Status: <span id="buzzerStatus">On</span></p>
    <p class="status">Water Storage Status: <span id="damStatus">On</span></p>
    <p class="status">Agriculture Status: <span id="waterStorageStatus">On</span></p>

    <script>
        function updateStatus(statusId, status) {
            var statusElement = document.getElementById(statusId);
            statusElement.innerText = status;
            statusElement.className = status === "On" ? "on" : "off";
        }

        function getStatus() {
            function sendRequest(url, callback) {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function() {
                    if (xhr.readyState == XMLHttpRequest.DONE) {
                        callback(xhr.responseText);
                    }
                };
                xhr.open("GET", url, true);
                xhr.send();
            }

            sendRequest("/getBuzzerStatus", function(response) {
                updateStatus("buzzerStatus", response);
            });

            sendRequest("/getDamStatus", function(response) {
                updateStatus("damStatus", response);
            });

            sendRequest("/getWaterStorageStatus", function(response) {
                updateStatus("waterStorageStatus", response);
            });
        }

        // Update status on page load
        getStatus();
        // Update status every 1 seconds
        setInterval(getStatus, 1000);
    </script>

</body>
</html>
)=====";

// Handle the root URL login page
void handleRoot() {
  server.send(200, "text/html", htmlContent1);
}

// Handle the guest page
void handleAquasentinel() {
  Serial.println("Handling guest request");
  server.send(200, "text/html", htmlContent3);
}

// Handle requests to get the status 
void handleGetBuzzerStatus() {
  int buzzerState = digitalRead(buzzerPin);
  server.send(200, "text/plain", String(buzzerState == LOW ? "On" : "Off"));
}

void handleGetDamStatus() {
  int damState = digitalRead(damPin);
  server.send(200, "text/plain", String(damState == LOW ? "On" : "Off"));
}

void handleGetWaterStorageStatus() {
  int waterStorageState = digitalRead(waterStoragePin);
  server.send(200, "text/plain", String(waterStorageState == LOW ? "On" : "Off"));
}

// Setup function
void setup() {
  Serial.begin(115200);

  // Set the pin modes for various components
  pinMode(buzzerPin, OUTPUT);
  pinMode(damPin, OUTPUT);
  pinMode(waterStoragePin, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Define routes and handlers for the web server
  server.on("/", HTTP_GET, handleRoot);

  server.on("/aquasentinel", HTTP_GET, handleAquasentinel);

  server.on("/getBuzzerStatus", HTTP_GET, handleGetBuzzerStatus);
  server.on("/getDamStatus", HTTP_GET, handleGetDamStatus);
  server.on("/getWaterStorageStatus", HTTP_GET, handleGetWaterStorageStatus);

  // Start the web server
  server.begin();
  Serial.println("Server started");

  // Set the initial states of various components
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(damPin, HIGH);
  digitalWrite(waterStoragePin, HIGH);
}

// Loop function
void loop() {
  // Handle incoming client requests
  server.handleClient();
}
