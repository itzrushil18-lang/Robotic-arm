#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "RushilRobot";
const char* password = "12345678";

WebServer server(80);

Servo shoulder;
Servo elbow;

int shoulderAngle = 90;
int elbowAngle = 90;

void handleRoot() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Rushil Robot Arm</title>

<style>
body{
  font-family:Arial;
  text-align:center;
  background:#f2f2f2;
  margin-top:40px;
}

.slider{
  width:80%;
}

button{
  padding:12px 25px;
  font-size:18px;
  border:none;
  border-radius:10px;
  background:#2196F3;
  color:white;
  cursor:pointer;
}

button:hover{
  background:#1976D2;
}
</style>

</head>

<body>

<h1>🤖 Rushil Robot Arm</h1>

<h2>Shoulder : <span id="sAngle">90</span>°</h2>

<input
type="range"
min="0"
max="180"
value="90"
class="slider"
id="shoulder"
oninput="moveShoulder(this.value)"
>

<br><br>

<h2>Elbow : <span id="eAngle">90</span>°</h2>

<input
type="range"
min="0"
max="180"
value="90"
class="slider"
id="elbow"
oninput="moveElbow(this.value)"
>

<br><br><br>

<button onclick="homePosition()">🏠 HOME</button>

<script>

function moveShoulder(angle){

document.getElementById("sAngle").innerHTML=angle;

fetch("/shoulder?angle="+angle);

}

function moveElbow(angle){

document.getElementById("eAngle").innerHTML=angle;

fetch("/elbow?angle="+angle);

}

function homePosition(){

document.getElementById("shoulder").value=90;
document.getElementById("elbow").value=90;

document.getElementById("sAngle").innerHTML=90;
document.getElementById("eAngle").innerHTML=90;

fetch("/home");

}

</script>

</body>
</html>
)rawliteral";

  server.send(200,"text/html",html);
}

void handleShoulder(){

  if(server.hasArg("angle")){
    shoulderAngle=server.arg("angle").toInt();
    shoulder.write(shoulderAngle);
  }

  server.send(200,"text/plain","OK");
}

void handleElbow(){

  if(server.hasArg("angle")){
    elbowAngle=server.arg("angle").toInt();
    elbow.write(elbowAngle);
  }

  server.send(200,"text/plain","OK");
}

void handleHome(){

  shoulderAngle=90;
  elbowAngle=90;

  shoulder.write(90);
  elbow.write(90);

  server.send(200,"text/plain","HOME");
}

void setup(){

  Serial.begin(115200);

  shoulder.attach(13,500,2400);
  elbow.attach(12,500,2400);

  shoulder.write(90);
  elbow.write(90);

  WiFi.softAP(ssid,password);

  Serial.println("WiFi Started");
  Serial.print("IP : ");
  Serial.println(WiFi.softAPIP());

  server.on("/",handleRoot);

  server.on("/shoulder",handleShoulder);

  server.on("/elbow",handleElbow);

  server.on("/home",handleHome);

  server.begin();

  Serial.println("Server Started");

}

void loop(){

  server.handleClient();

}