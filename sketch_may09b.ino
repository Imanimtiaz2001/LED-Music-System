/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "OPPOF11";
const char* password = "iman12345";
const int output = 12;
const int output1 = 13;
const int output2 = 14;

String sliderValue = "0";
String a = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="ISO-8859-1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body onload="initialize()">
  <h2>ESP Web Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script type="text/javascript" charset="utf-8">

var a;
var audioElementId = 'audio1';
var audioFileURLId = 'audioFileURL';
var playAudioBtnId = 'playAudioBtn';
var pauseAudioBtnId = 'pauseAudioBtn';
var stopAudioBtnId = 'stopAudioBtn';
var outputElementId = 'output1';

function initialize(){
    initialize_video();
    initialize_audio();
    
}
function initialize_audio(){
    audioElement = document.getElementById(audioElementId);
    audioElement.style.display = 'none';
    outputElement = document.getElementById(outputElementId);
    audioElement.addEventListener("ended", function(){
        message = "Play audio finish.";
        alert(message);
        outputElement.value = message;
    });
    // Add time update event listener.
    audioElement.addEventListener("timeupdate", function(){
        var audioPlayStatus = "Playing ";
        var audioPlayTime = audioElement.currentTime;
        var audioDuration = audioElement.duration;
        audioPlayStatus += Math.floor(audioPlayTime) + ' seconds / Total ' + Math.floor(audioDuration) +' seconds'
            
        outputElement.value = audioPlayStatus;
    
    });
    
    playAudioBtnElement = document.getElementById(playAudioBtnId);
    playAudioBtnElement.disabled = false;
    pauseAudioBtnElement = document.getElementById(pauseAudioBtnId);
    pauseAudioBtnElement.disabled = true;
    stopAudioBtnElement = document.getElementById(stopAudioBtnId);
    stopAudioBtnElement.disabled = true;
    a = "0";
}
function playAudio(src){
    //https://dev2qa.com/demo/media/test.mp3
    audioFileURLInput = document.getElementById(audioFileURLId);
    audioFileURLStr = audioFileURLInput.value;
    if('' == audioFileURLStr){
        alert('Please input audio file URL first.');
    }else{
        audioElement = document.getElementById(audioElementId);
        audioElement.style.display = 'block';
        audioElement.src = audioFileURLStr;
        audioElement.play();
        outputElement = document.getElementById(outputElementId);
        outputElement.style.display = 'block';
        src.disabled = true;
        pauseAudioBtnElement = document.getElementById(pauseAudioBtnId);
        pauseAudioBtnElement.disabled = false;
        stopAudioBtnElement = document.getElementById(stopAudioBtnId);
        stopAudioBtnElement.disabled = false;
        a = "1";
    }
}
function pauseAudio(src){
    audioElement = document.getElementById(audioElementId);
    btnText = src.value.toLowerCase();
    if(btnText == 'pause audio'){
        audioElement.pause();
        src.value = 'Continue Audio';
    }else if(btnText == 'continue audio'){
        audioElement.play();
        src.value = 'Pause Audio';
        a = "0";
    }
    
}
function stopAudio(src){
    outputElement = document.getElementById(outputElementId);
    outputElement.style.display = 'none';
    audioElement = document.getElementById(audioElementId);
    audioElement.load();
    audioElement.style.display = 'none';
    src.disabled = true;
    pauseAudioBtnElement = document.getElementById(pauseAudioBtnId);
    pauseAudioBtnElement.disabled = true;
    pauseAudioBtnElement.value = 'Pause Audio';
    playAudioBtnElement = document.getElementById(playAudioBtnId);
    playAudioBtnElement.disabled = false;
    a = "0";
}
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}

</script>

<h3>How To Play Video Audio In Html5 Example.</h3>
<div style="display:block;margin-top: 10px;">
    <label>Input Audio File URL:</label>
    <input type="text" id="audioFileURL"/>
    <input type="button" id="playAudioBtn" value="Play Audio" onclick="playAudio(this)" />
    <input type="button" id="pauseAudioBtn" value="Pause Audio" onclick="pauseAudio(this)" />
    <input type="button" id="stopAudioBtn" value="Stop Audio" onclick="stopAudio(this)" />
</div>
<output id="output2" style="display:block;margin-top: 10px;"></output>
<audio id="audio1" style="display:block;margin-top: 10px;" controls></audio>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

String processor1(const String& var){
  //Serial.println(var);
  if (var == "Play Audio"){
    return "1";
  }
  return String();
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(output, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output, ledChannel1);
  ledcAttachPin(output1, ledChannel2);ledcAttachPin(output2, ledChannel3);
  ledcWrite(ledChannel1, sliderValue.toInt());
ledcWrite(ledChannel2, sliderValue.toInt());ledcWrite(ledChannel3, sliderValue.toInt());
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor1);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      if(a == "1"){
        ledcWrite(ledChannel1, sliderValue.toInt());
        ledcWrite(ledChannel2, sliderValue.toInt());
        ledcWrite(ledChannel3, sliderValue.toInt());
      }
      
      }
    
    
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
