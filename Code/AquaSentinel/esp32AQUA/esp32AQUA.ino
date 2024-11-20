  #include <WiFi.h>
  #include <WebServer.h>
  #include <HTTPClient.h>

  int number = 0;
  int prev = 0;

  const char* ssid = "kamalagrg5624_2.4";
  const char* password = "VUZ1N5334_JI45E";

  String URL = "http://192.168.1.84/aquaSentinel/aquaSentinel.php";

  const int agriculturePin = 12;
  const int damPin = 15;
  const int waterStoragePin = 14;
  const int maintainance = 13;

  // Web server instance
  WebServer server(80);

  const char* htmlContent1 = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>AquaSentinel</title>
  <style>
    body, html {
      margin: 0;
      padding: 0;
      height: 100%;
      overflow: hidden;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }
    .background {
      display: flex;
      flex-direction: column;
      justify-content: flex-start;
      align-items: center;
      height: 100vh;
      background: linear-gradient(to bottom right, #3795e8 0%, #7700d9 100%);
      position: relative;
      text-align: center;
      color: white;
      padding: 20px;
      padding-top: 60px;
    }
  
   /* Original horizontal movement animation */
  @keyframes moveWaveHorizontal {
    from {
      background-position-x: 0;
    }
    to {
      background-position-x: 1000px;
    }
  }
  
  /* New vertical movement animation */
  @keyframes moveWaveVertical {
    0%, 100% {
      transform: translateY(0);
    }
    50% {
      transform: translateY(-20px); /* Moves up by 20px */
    }
  }
  
  .wave {
    position: absolute;
    bottom: 5%;
    left: 0;
    width: 100%;
    height: 30%;
    background: url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 120" preserveAspectRatio="none"><path d="M0,0 C 150,100 350,0 500,0 C 650,100 850,100 1000,50 C 1150,0 1200,0 1200,0 L 1200,150 L 0,150" fill="white"/></svg>') repeat-x;
    background-size: cover;
    z-index: 1;
    animation: moveWaveHorizontal 30s linear infinite, moveWaveVertical 5s ease-in-out infinite; /* Apply both animations */
  }
  
    
    .loginbracket {
      background-color: #fff; /* White background */
      border-radius: 10px;
      box-shadow: 0 0 20px rgba(0, 0, 0, 0.2);
      padding: 40px;
      text-align: center;
      display: flex; /* Use flexbox */
      flex-direction: column; /* Stack items vertically */
      align-items: center; /* Center items horizontally */
      justify-content: center; /* Center items vertically */
      position: relative;
      z-index: 2; /* Ensure loginbracket is above the wave */
    }
    h2 {
      color: #3498db;
    }
    .login-button {
      border: none;
      padding: 12px 24px;
      margin: 10px;
      border-radius: 5px;
      cursor: pointer;
      font-size: 18px;
      transition: background-image 0.3s;
      background-image: linear-gradient(to bottom right, #60abdd, #0d659f);
      color: #fff;
      box-shadow: 0 3px 5px 0 rgba(0, 0, 0, 0.2);
    }
    
    .login-button:hover {
      background-image: linear-gradient(to bottom right, #2980b9, #3498db);
    }
    
    .dam-icon {
      font-size: 50px;
      color: #3498db;
      margin-bottom: 20px;
    }
    /* Responsive styling for smaller screens */
    @media (max-width: 600px) {
      .loginbracket {
        width: 80%;
      }
    }
  
    h2 span {
      display: inline-block;
      transition: transform 0.3s ease;
    }
    
    @keyframes jump {
      50% {
        transform: translateY(-10px);
      }
    }
    
  
  </style>
  </head>
  <body>
  
  <div class="background">
      <div class="loginbracket">
          <span class="dam-icon">🌊</span>
          <h2>
            <span>S</span><span>m</span><span>a</span><span>r</span><span>t</span> 
            <span>D</span><span>a</span><span>m</span> 
            <span>S</span><span>y</span><span>s</span><span>t</span><span>e</span><span>m</span>
          </h2>
          
          <button class="login-button" onclick="loginAsAdmin()">Admin</button>
          <button class="login-button" onclick="loginAsGuest()">Guest</button>
      </div>
      <div class="wave"></div>
  </div>
  <script>
    document.querySelector('h2').addEventListener('mouseover', function() {
      this.querySelectorAll('span').forEach((element, index) => {
        element.style.animation = '';
        setTimeout(() => {
          element.style.animation = 'jump 0.6s ease';
        }, index * 100); // Adjust the delay as needed
      });
    });
  
    document.querySelector('h2').addEventListener('mouseout', function() {
      this.querySelectorAll('span').forEach((element, index) => {
        element.style.animation = '';
      });
    });
  
  
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
  
  //admin
  const char* htmlContent2 = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Water Management Admin</title>
  <style>
    body, html {
        margin: 0;
        padding: 0;
        height: 100%;
        overflow: hidden;
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }
    .background {
        display: flex;
        flex-direction: column;
        justify-content: flex-start;
        align-items: center; /* This centers children horizontally */
        height: 100vh;
        background: linear-gradient(to bottom right, #3795e8 0%, #7700d9 100%);
        position: relative; /* Allows absolute positioning within */
        text-align: center;
        color: white;
        padding: 20px;
        width: 100%;
    }
    .aquasentinel {
        position: absolute; /* Position absolutely within .background */
        top: 0px; /* Adjust top position */
        left: 0px; /* Adjust left position */
        font-size: 40px;
        color: white;
        text-align: left; /* Align text to the left */
    }
    .admin-control {
        background-color: #f2f2f2; /* Grey background */
        padding: 20px; /* Padding around the content */
        border-radius: 10px; /* Rounded corners */
        margin-top: 180px; /* Increased space from the title */
        margin-left: auto;
        margin-right: auto;
        width: 80%;
        color: black;
        max-width: 960px; /* Optional: limit the max width */
    }
    
    .control-group {
        margin-top: 20px;
    }
    label {
        margin-right: 10px;
    }
    .wave {
        position: absolute;
        bottom: 0;
        left: 0;
        width: 100%;
        height: 20%;
        background: url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 120" preserveAspectRatio="none"><path d="M0,0 C 150,100 350,0 500,0 C 650,100 850,100 1000,50 C 1150,0 1200,0 1200,0 L 1200,150 L 0,150" fill="white"/></svg>') repeat-x;
        background-size: cover;
        z-index: 1;
    }
    .control-group {
        display: flex; /* Use flexbox */
        justify-content: space-between; /* Space out label and radio buttons */
        align-items: center; /* Center items vertically */
        width: 100%; /* Take full width of parent */
        margin-top: 20px;
    }
    .control-label {
        flex: 1; /* Allows the label to grow and take up space */
        text-align: left; /* Align the text to the left */
    }
    .radio-options {
        flex: 1; /* Allows the radio options to grow and take up space */
        text-align: right; /* Align the radio buttons to the right */
    }
  </style>
</head>
<body>
<div class="background">
  <h1 class="aquasentinel">AquaSentinel</h1>
  <div class="admin-control">
    <h2>Admin Control</h2>
    <h2>Maintainence</h2>
    <div class="control-group">
        <div class="control-label"><label for="agricultureIrrigation">Would You Like To Start Maintainence?</label></div>
        <div class="radio-options">
          <input type="radio" id="maintainanceOn" name="agricultureIrrigation" value="on" onchange="setSystemState('maintain', 'on')"> Start
          <input type="radio" id="maintainanceOff" name="agricultureIrrigation" value="off" onchange="setSystemState('maintain', 'off')"> Stop
        </div>
    </div>
  </div>
</div>
<div class="wave"></div>
<script>
  function setSystemState(system, state) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        console.log(this.responseText);
      }
    };
    xhttp.open("GET", "/" + system + "State?state=" + state, true);
    xhttp.send();
  }
  // Update status on page load
  getStatus();
  // Update status every 1 seconds
  setInterval(getStatus, 1000);
</script>
</body>
</html>

  )=====";
  
  //guest
  const char* htmlContent3 = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Status Page</title>
      <style>
          body, html {
            margin: 0;
            padding: 0;
            height: 100%;
            overflow: hidden;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
          }
          .background {
            display: flex;
            flex-direction: column;
            justify-content: flex-start;
            align-items: center;
            height: 100vh;
            background: linear-gradient(to bottom right, #3795e8 0%, #7700d9 100%);
            position: relative;
            text-align: center;
            color: white;
            padding: 20px;
            padding-top: 60px;
          }
          .title-container {
            width: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
          }
          .status-container {
            display: flex;
            justify-content: space-evenly;
            align-items: center;
            flex-wrap: nowrap;
            width: 100%;
            max-width: 960px;
            margin-top: 75px;
            gap: 20px;
          }
          .aquasentinel {
            position: absolute;
            top: 0;
            left: 0;
            margin: 20px;
            font-size: 40px;
            color: white;
          }
          .status-box {
            background-color: #f0f0f0;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            border-radius: 5px;
            padding: 15px;
            color: #000;
            width: 300px;
            height: 100px;
            display: flex;
            align-items: center;
            justify-content: center;
            transition: transform 0.3s ease, box-shadow 0.3s ease; /* Smooth transition for transform and shadow */
          }
          
          .status-box:hover {
            transform: translateY(-10px); /* Moves the box up when hovered */
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2); /* Optionally, increase the shadow to give a "lifted" effect */
          }
    
   /* Original horizontal movement animation */
   @keyframes moveWaveHorizontal {
    from {
      background-position-x: 0;
    }
    to {
      background-position-x: 1000px;
    }
  }
  
  /* New vertical movement animation */
  @keyframes moveWaveVertical {
    0%, 100% {
      transform: translateY(0);
    }
    50% {
      transform: translateY(-20px); /* Moves up by 20px */
    }
  }
  
  .wave {
    position: absolute;
    bottom: -5%;
    left: 0;
    width: 100%;
    height: 30%;
    background: url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 120" preserveAspectRatio="none"><path d="M0,0 C 150,100 350,0 500,0 C 650,100 850,100 1000,50 C 1150,0 1200,0 1200,0 L 1200,150 L 0,150" fill="white"/></svg>') repeat-x;
    background-size: cover;
    z-index: 1;
    animation: moveWaveHorizontal 30s linear infinite, moveWaveVertical 5s ease-in-out infinite; /* Apply both animations */
  }
  
      </style>
  </head>
  <body>
      <div class="background">
          <div class="aquasentinel">AQUASENTINEL</div>
          <div class="title-container">
              <h1>Status Page</h1>
          </div>
          <div class="status-container">
              <div class="status-box">
                  <p class="status">Dam Status: <span id="damStatus">On</span></p>
              </div>
              <div class="status-box">
                  <p class="status">Water Storage Status: <span id="waterStorageStatus">On</span></p>
              </div>
              <div class="status-box">
                  <p class="status">Agriculture Status: <span id="agricultureStatus">On</span></p>
              </div>
          </div>
      </div>
      <div class="wave"></div>
  
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
  
              sendRequest("/getAgricultureStatus", function(response) {
                  updateStatus("agricultureStatus", response);
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
  
  void handleDatabase() {
      // Handle incoming client requests
      String system = "Maintainance";
      String status = "default";

      int stat = digitalRead(maintainance);  
      if (stat == HIGH){
        status = "Start";
      }else{
        status = "Stop";
      }

      HTTPClient http;
      http.begin(URL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Move this before POST

      String postData = "system=" + String(system) + "&status=" + String(status);
      int httpCode = http.POST(postData); // Now the header is set before the POST request
      String payload = http.getString(); // Retrieve the response payload

      Serial.print("URL: "); Serial.println(URL);
      Serial.print("Data: "); Serial.println(postData);
      Serial.print("httpCode: "); Serial.println(httpCode);
      Serial.print("payload: "); Serial.println(payload);
      Serial.println("--------------------------------------");

      delay(10000); 
  }

  // Handle the root URL login page
  void handleRoot() {
    server.send(200, "text/html", htmlContent1);
  }
  
  void handleAdmin() {
    Serial.println("Handling admin request");
    server.send(200, "text/html", htmlContent2);
  }
  
  // Handle the guest page
  void handleAquasentinel() {
    Serial.println("Handling guest request");
    server.send(200, "text/html", htmlContent3);
  }

  void handleGetAgricultureStatus() {
    int buzzerState = digitalRead(agriculturePin);
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

  void handleMaintainanceChange() {
  String state = server.arg("state"); // Read the state parameter from the request
  if (state == "on") {
    digitalWrite(maintainance, HIGH); // Set the maintainance pin to HIGH
    Serial.println("Maintenance started");
    handleDatabase();
  } else if (state == "off") {
    digitalWrite(maintainance, LOW); // Set the maintainance pin to LOW
    Serial.println("Maintenance stopped");
    handleDatabase();
  }
  server.send(200, "text/plain", "Maintenance State Updated"); // Send a response back to the client
  }

  void handleDamStateChange() {
  String state = server.arg("state"); // Read the state parameter from the request
  if (state == "on") {
    digitalWrite(damPin, HIGH); // Set the damPin to HIGH
    Serial.println("Dam System turned ON");
  } else if (state == "off") {
    digitalWrite(damPin, LOW); // Set the damPin to LOW
    Serial.println("Dam System turned OFF");
  }
  server.send(200, "text/plain", "Dam State Updated"); // Send a response back to the client
  }

  void setup() {
    Serial.begin(115200);
  
    pinMode(agriculturePin, OUTPUT);
    pinMode(damPin, OUTPUT);
    pinMode(waterStoragePin, OUTPUT);
    pinMode(maintainance, OUTPUT);
  
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  
    server.on("/", HTTP_GET, handleRoot);
    server.on("/handleAdmin", HTTP_GET, handleAdmin);
    server.on("/aquasentinel", HTTP_GET, handleAquasentinel);

    server.on("/getAgricultureStatus", HTTP_GET, handleGetAgricultureStatus);
    server.on("/getDamStatus", HTTP_GET, handleGetDamStatus);
    server.on("/getWaterStorageStatus", HTTP_GET, handleGetWaterStorageStatus);

    server.on("/maintainState", HTTP_GET, handleMaintainanceChange);

    server.on("/damState", HTTP_GET, handleDamStateChange);

    server.begin();
    Serial.println("Server started");
  }
  
  void loop() {
    server.handleClient();
    delay(2000);
  }

  
  
  
  
  
  
  