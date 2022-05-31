#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Hostels Wi-Fi";
const char* password = "";
const int output = 12;
const int output1 = 13;
const int output2 = 14;
const int output3 = 25;
const int output4 = 26;
String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int ledChannel4 = 3;
const int ledChannel5 = 4;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//html code
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>  
<html>  
    <head>  
      <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="ISO-8859-1">
        <link rel="stylesheet" href="http://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.css" />  
        <script src="http://code.jquery.com/jquery-1.11.1.min.js"></script>  
        <script src="http://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js"></script>  
     <title>ESP Web Server</title>
        <script>  
        var player;  
        
        window.onload=function()  
        {  
            player = document.getElementById('myMusic');  
            document.getElementById('btnPlay').addEventListener('click', function(){  
               player.play();
               document.getElementById("ok").disabled = false;
            });  
            document.getElementById('btnPause').addEventListener('click' ,function(){  
                player.pause();     
                document.getElementById("ok").disabled = true;
            });  
            document.getElementById('btnStop').addEventListener('click', function(){  
                player.pause();  
                player.currentTime = 0;  
                document.getElementById("ok").disabled = true;
            });  
            $('#VolumeSlider').on("slidestop", function(){  
                    var volume = document.getElementById('VolumeSlider').value;  
                    console.log(volume);  
                    player.volume = volume;  
                      
                });  
              
        }  
        function updateSliderPWM(element) {
          if(document.getElementById("ok").disabled == false){
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
else{
  document.getElementById("pwmSlider").value = 0;
  }
}
        </script> 
         <style>
                *{text-align : center;}
        </style>  
    </head>  
    <body>  
  <h1 style="font-size :75px;color: indigo;font-family: 'Times New Roman', Times, serif;">MUSIC REACTIVE LED SYSTEM</h1>
        <audio id="myMusic">  
            <source src="https://file-examples.com/storage/fe5b5df86162954479a6a35/2017/11/file_example_MP3_5MG.mp3"/>  
        </audio>  
        <button id="btnPlay" >Play</button>  
        <button id="btnPause" >Pause</button>  
        <button id="btnStop">Stop</button>  
        <button disabled id="ok"></button>   
          <h2>VOLUME</h2>
        <input type="range"  id="VolumeSlider" min="0" max="1" step=".01" value="1" class="slider"/> 
             <h2><span style="color: YELLOW;">B</span>
             <span style="color: green;">R</span>
             <span style="color: orange;">I</span>
             <span style="color: RED;">G</span>
             <span style="color: BLUE;">H</span>
             <span style="color: YELLOW;">T</span>
             <span style="color: green;">N</span>
             <span style="color: orange;">E</span>
             <span style="color: RED;">S</span>
             <span style="color: BLUE;">S</span></h2>
           <p><span id="textSliderValue" style="display: none;">%SLIDERVALUE%</span></p> 
        <input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" step="1"  value="%SLIDERVALUE%"class="slider"/>  
        <script type="text/javascript" src="cordova.js" ></script>  
    </body>  
</html> 
)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
 //decelaring all the pins for output 
  pinMode(output, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
   pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);
  ledcSetup(ledChannel4, freq, resolution);
  ledcSetup(ledChannel5, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output, ledChannel1);
  ledcAttachPin(output1, ledChannel2);ledcAttachPin(output2, ledChannel3);
  ledcAttachPin(output3, ledChannel4);ledcAttachPin(output4, ledChannel5);
  ledcWrite(ledChannel1, sliderValue.toInt());
ledcWrite(ledChannel2, sliderValue.toInt());ledcWrite(ledChannel3, sliderValue.toInt());
ledcWrite(ledChannel4, sliderValue.toInt());ledcWrite(ledChannel5, sliderValue.toInt());
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

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      
      //check condition for the slider i.e if the audio is not paused or stop slider value will not be equal to 0 and hence the leds will blink 
      while(sliderValue.toInt() != 0){
      
      ledcWrite(ledChannel1, sliderValue.toInt());
      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, sliderValue.toInt());
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, sliderValue.toInt());
      delay(100);

      ledcWrite(ledChannel1, 0);
      ledcWrite(ledChannel2, sliderValue.toInt());
      ledcWrite(ledChannel3, 0);
      ledcWrite(ledChannel4, sliderValue.toInt());
      ledcWrite(ledChannel5, sliderValue.toInt());
      delay(100);


      ledcWrite(ledChannel1, sliderValue.toInt());
      ledcWrite(ledChannel2, sliderValue.toInt());
      ledcWrite(ledChannel3, 0);
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, 0);
      delay(100);

       ledcWrite(ledChannel1, sliderValue.toInt());
      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, sliderValue.toInt());
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, sliderValue.toInt());
      delay(100);

      ledcWrite(ledChannel1, 0);
      ledcWrite(ledChannel2, sliderValue.toInt());
      ledcWrite(ledChannel3, 0);
      ledcWrite(ledChannel4, sliderValue.toInt());
      ledcWrite(ledChannel5, sliderValue.toInt());
      delay(100);


      ledcWrite(ledChannel1, sliderValue.toInt());
      ledcWrite(ledChannel2, sliderValue.toInt());
      ledcWrite(ledChannel3, 0);
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, 0);
      delay(100);

      
      }
    
    }
    //else all the leds will be off
    else {
      inputMessage = "No message sent";
    }
      ledcWrite(ledChannel1, 0);
      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, 0);
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, 0);
      
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
