#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "Redmi9C";
const char* password = "Redmi9CC";

WiFiServer server(80);
//led on esp32
int LED=2;
 // put your setup code here, to run once:
char webpage[] PROGRAM=R"=====(

<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>How To Play Video Audio In Html5 Example</title>
<script type="text/javascript" > var audioElementId = 'audio1';
    var audioFileURLId = 'audioFileURL';
    var playAudioBtnId = 'playAudioBtn';
    var pauseAudioBtnId = 'pauseAudioBtn';
    var stopAudioBtnId = 'stopAudioBtn';
    var outputElementId = 'output1';
    function initialize(){
        initialize_video();
        initialize_audio();
        
    }function initialize_audio(){
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
    }
    
    function playAudio(src){
        //https://dev2qa.com/demo/media/test.mp3
        audioFileURLInput = document.getElementById(audioFileURLId);
        audioFileURLStr = audioFileURLInput.value;
        if('' == audioFileURLStr){
            alert('Please input audio file URL first.');
        }else{
            document.getElementById("demo").innerHTML = "Audio Played" ;
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
        }
    }
    function pauseAudio(src){
        audioElement = document.getElementById(audioElementId);
        btnText = src.value.toLowerCase();
        if(btnText == 'pause audio'){
            document.getElementById("demo").innerHTML = "Audio Paused" ;
            audioElement.pause();
            src.value = 'Continue Audio';
        }else if(btnText == 'continue audio'){
            document.getElementById("demo").innerHTML = "Audio Played" ;
            audioElement.play();
            src.value = 'Pause Audio';
        }
    }
    </script>
</head>
<body onload="initialize()">
<h3>How To Play Video Audio In Html5 Example.</h3>
<div style="display:block;margin-top: 10px;">
    <label>Input Audio File URL:</label>
    <input type="text" id="audioFileURL"/>
    <input type="button" id="playAudioBtn" value="Play Audio" onclick=playAudio(this) />
    <input type="button" id="pauseAudioBtn" value="Pause Audio" onclick=pauseAudio(this) />
</div>
<output id="output2" style="display:block;margin-top: 10px;"></output>
<audio id="audio1" style="display:block;margin-top: 10px;" controls></audio>
<p id="demo"></p>
</body>
</html>
)=====";
void setup()
{
  pinMode(LED ,OUTPUT);
  Wifi.begin(ssid,password);
    Serial.begin(115200);
    while(WiFi.status()!=WL_CONNECTED)
    {
     delay(500);}
     
  //set the led on esp32 as output
pinMode(LED ,OUTPUT);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/",[](){server.send(200,"text/html",webpage);});
  server.on("/ledstate",getLEDState);
    server.begin();

}

void loop(){
 server.handleClient();
  }
  void getLEDState()
  {toggleLED();
  String led-state=digitalRead(LED)? "OFF":"ON";
  server.send(200,"text/plain",led_state);}
}
void toggleLED()
{
  digitalWriter(LED,!digital|Read(LED))
  server.send_P(200."text/html",webpage)}
