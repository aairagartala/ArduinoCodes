/*

#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;

String ssid =     "AUTOMATION";
String password = "pass@1234";


void setup()
{

  Serial.begin(115200);

    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(2000);
        Serial.print(".");
    }

  Serial.println("Wifi Connected Successfully to AUTOMATION");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  
  audio.setVolume(100);

  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
   
}

void loop()

{

  audio.loop();

}

void audio_info(const char *info) {
  Serial.print("audio_info: "); Serial.println(info);
}
*/
/*
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

// I2S Pin configuration
#define I2S_DOUT      14
#define I2S_BCLK      13
#define I2S_LRC       12

// Audio object
Audio audio;

// WiFi credentials
const char* ssid =     "AUTOMATION";
const char* password = "pass@1234";

// Stream URL (use a direct AAC stream instead of M3U8 for better compatibility)
const char* streamURL = "https://air.pc.cdn.bitgravity.com/air/live/pbaudio130/playlist.m3u8";
//const char* streamURL =  "http://www.wdr.de/wdrlive/media/einslive.m3u";
//https://air.pc.cdn.bitgravity.com/air/live/pbaudio130/playlist.m3u8

// Debugging states
enum DebugState {
  DEBUG_WIFI,
  DEBUG_STREAM_CONNECT,
  DEBUG_CODEC_CHECK,
  DEBUG_PLAYBACK,
  DEBUG_ERROR
};

DebugState currentDebugState = DEBUG_WIFI;
bool codecSupported = false;
bool isPlaying = false;
unsigned long playbackStartTime = 0;
int connectionAttempts = 0;
const int MAX_CONNECTION_ATTEMPTS = 3;

void printDebugInfo(const char* message) {
  Serial.print("[DEBUG] ");
  Serial.println(message);
}

void printDebugState() {
  Serial.print("Current state: ");
  switch(currentDebugState) {
    case DEBUG_WIFI: Serial.println("WiFi connection"); break;
    case DEBUG_STREAM_CONNECT: Serial.println("Stream connection"); break;
    case DEBUG_CODEC_CHECK: Serial.println("Codec verification"); break;
    case DEBUG_PLAYBACK: Serial.println("Playback monitoring"); break;
    case DEBUG_ERROR: Serial.println("Error state"); break;
  }
}

void checkCodecSupport(const char* info) {
  // Check if the codec information contains AAC
  if (strstr(info, "aac") != NULL || strstr(info, "AAC") != NULL) {
    codecSupported = true;
    printDebugInfo("AAC codec detected - supported!");
  }
  // Check for other supported codecs if needed
  else if (strstr(info, "mp3") != NULL || strstr(info, "MP3") != NULL) {
    codecSupported = true;
    printDebugInfo("MP3 codec detected - supported!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\nESP32 Audio Player (With PSRAM)");
  Serial.println("==============================");
  //Serial.println("Note: Using smaller buffers for ESP32-WROOM without PSRAM");
  
  // Check memory status
  Serial.print("Free heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  
  Serial.print("Total heap: ");
  Serial.print(ESP.getHeapSize());
  Serial.println(" bytes");

  // Step 1: Connect to WiFi
  currentDebugState = DEBUG_WIFI;
  printDebugState();
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  unsigned long wifiTimeout = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Timeout after 20 seconds
    if (millis() - wifiTimeout > 20000) {
      Serial.println("\nFailed to connect to WiFi!");
      currentDebugState = DEBUG_ERROR;
      printDebugState();
      return;
    }
  }
  
  Serial.println("\nWiFi connected successfully!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // Step 2: Setup audio with memory-optimized settings
  currentDebugState = DEBUG_STREAM_CONNECT;
  printDebugState();
  
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(80); // Reduced volume to prevent distortion
  
  // Note: setBufsize() and setConnectionTimeout() are not available in this library version
  // The library should automatically use smaller buffers for devices without PSRAM

  // Step 3: Connect to stream
  Serial.print("Connecting to stream: ");
  Serial.println(streamURL);
  
  audio.connecttohost(streamURL);
  connectionAttempts++;
  
  Serial.println("Stream connection initiated");
  Serial.print("Free heap after audio init: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
}

void loop() {
  // Main audio processing loop
  audio.loop();
  
  // Additional debugging and monitoring
  static unsigned long lastStatusCheck = 0;
  if (millis() - lastStatusCheck > 5000) { // Print status every 5 seconds
    lastStatusCheck = millis();
    
    if (currentDebugState != DEBUG_ERROR) {
      Serial.println("\n--- Status Update ---");
      printDebugState();
      Serial.print("Codec supported: ");
      Serial.println(codecSupported ? "YES" : "NOT YET DETERMINED");
      Serial.print("Is playing: ");
      Serial.println(isPlaying ? "YES" : "NO");
      if (isPlaying) {
        Serial.print("Playback time: ");
        Serial.print((millis() - playbackStartTime) / 1000);
        Serial.println(" seconds");
      }
      Serial.print("WiFi RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      Serial.print("Free heap: ");
      Serial.print(ESP.getFreeHeap());
      Serial.println(" bytes");
      Serial.print("Connection attempts: ");
      Serial.println(connectionAttempts);
      Serial.println("---------------------");
    }
    
    // If not playing after 20 seconds, try to reconnect
    if (!isPlaying && millis() > 20000 && connectionAttempts < MAX_CONNECTION_ATTEMPTS) {
      Serial.println("No playback detected, attempting reconnection...");
      audio.connecttohost(streamURL);
      connectionAttempts++;
    }
  }
  
  // Monitor memory and prevent heap exhaustion
  static unsigned long lastMemCheck = 0;
  if (millis() - lastMemCheck > 10000) { // Check memory every 10 seconds
    lastMemCheck = millis();
    
    if (ESP.getFreeHeap() < 10000) { // If free heap drops below 10KB
      Serial.println("Low memory detected! Restarting audio...");
      audio.stopSong();
      delay(100);
      audio.connecttohost(streamURL);
    }
  }
}

// Audio event handlers for debugging
void audio_info(const char *info) {
  Serial.print("audio_info: "); 
  Serial.println(info);
  
  // Check for codec information
  checkCodecSupport(info);
  
  // Check for playback information
  if (strstr(info, "Start") != NULL) {
    if (strstr(info, "audio") != NULL) {
      isPlaying = true;
      playbackStartTime = millis();
      if (currentDebugState != DEBUG_PLAYBACK) {
        currentDebugState = DEBUG_PLAYBACK;
        printDebugState();
        Serial.println("Playback started successfully!");
      }
    }
  }
}

void audio_id3data(const char *info) {
  Serial.print("id3data: "); 
  Serial.println(info);
}

void audio_eof_stream(const char *info) {
  Serial.print("eof_stream: "); 
  Serial.println(info);
  isPlaying = false;
  
  // Try to reconnect if stream ends unexpectedly
  if (currentDebugState == DEBUG_PLAYBACK) {
    Serial.println("Stream ended, attempting to reconnect...");
    audio.connecttohost(streamURL);
    currentDebugState = DEBUG_STREAM_CONNECT;
    printDebugState();
  }
}

void audio_showstation(const char *info) {
  Serial.print("station: "); 
  Serial.println(info);
}

void audio_showstreamtitle(const char *info) {
  Serial.print("streamtitle: "); 
  Serial.println(info);
}

void audio_showstreaminfo(const char *info) {
  Serial.print("streaminfo: "); 
  Serial.println(info);
}

void audio_error(const char *info) {
  Serial.print("audio_error: "); 
  Serial.println(info);
  currentDebugState = DEBUG_ERROR;
  printDebugState();
  
  // Try to reconnect on error
  if (connectionAttempts < MAX_CONNECTION_ATTEMPTS) {
    Serial.println("Attempting to reconnect after error...");
    delay(3000);
    audio.connecttohost(streamURL);
    connectionAttempts++;
    currentDebugState = DEBUG_STREAM_CONNECT;
  }
}
*/
