/******************************************************************************
* file    main.cpp
*******************************************************************************
* brief   Main Program
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-14
*******************************************************************************
* MIT License
*
* Copyright (c) 2022 Crelin - Florian Baumgartner
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell          
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include <Arduino.h>
#include <Preferences.h>
#include "console.hpp"
#include "utils.hpp"

#include <WiFi.h>
#include <esp_now.h>

#define BUTTON            0
#define LED               10
#define BLINK_INTERVAL    200
#define WATCHDOG_TIMEOUT  30    // [s]

Utils utils;
Preferences preferences;
bool master = false;

void broadcast(const String &message);
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status);
void printMacAddress(const uint8_t* macAddr)
{
  console.log.printf("\"%02X:%02X:%02X:%02X:%02X:%02X\"", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  console.begin();
  if(!utils.begin(WATCHDOG_TIMEOUT, "DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }

  console.log.printf("[MAIN] Board Serial Number: %s\n", utils.getSerialNumber());
  if(strncmp(utils.getSerialNumber(), "0", MAX_STRING_LENGTH) == 0)
  {
    master = true;
    console.log.println("[MAIN] Board is ESP-Now Master Device");
  }
  else
  {
    master = false;
    console.log.println("[MAIN] Board is ESP-Now Slave Device");
  }

  preferences.begin("WallLight", false);
  uint32_t bootCount = preferences.getUInt("bootCount", 0);
  console.log.printf("[MAIN] Boot Count: %d\n", bootCount);
  bootCount++;
  preferences.putUInt("bootCount", bootCount);
  preferences.end();


  WiFi.mode(WIFI_STA);                      // Set device in STA mode to begin with
  WiFi.setTxPower(WIFI_POWER_19_5dBm);      // Set Max RF-Power
  console.log.printf("[MAIN] My MAC Address is: \"%s\"\n", WiFi.macAddress().c_str()); 
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK)             // Startup ESP Now
  {
    console.ok.println("[MAIN] ESP-Now Init Success");
    esp_now_register_recv_cb(receiveCallback);
    esp_now_register_send_cb(sentCallback);
  }
  else
  {
    console.error.println("[MAIN] ESP-Now Init Failed");
    // restart();
  }
}

void loop()
{
  utils.feedWatchdog();

  static bool btnOld = false, btnNew = false;
  btnOld = btnNew; btnNew = !digitalRead(BUTTON);
  if((!btnOld && btnNew))
  {
    console.log.println("[MAIN] Button pressed!");
    broadcast("Test Payload");
  }
 
  static int t = 0;
  if(millis() - t > 5000)
  {
    t = millis();
    console.log.printf("[MAIN] Time: %d\n", t);
  }
  digitalWrite(LED, (millis() / BLINK_INTERVAL) & 1);
  delay(10);
}


void broadcast(const String &message)
{
  // this will broadcast a message to everyone in range
  // uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  // esp_now_peer_info_t peerInfo = {};
  // memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  // if (!esp_now_is_peer_exist(broadcastAddress))
  // {
  //   esp_now_add_peer(&peerInfo);
  // }
  // esp_err_t result = esp_now_send(broadcastAddress, (const uint8_t *)message.c_str(), message.length());


  // and this will send a message to a specific device
  uint8_t peerAddress[] = {0x34, 0x85, 0x18, 0x8B, 0x98, 0x98};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, peerAddress, 6);
  if (!esp_now_is_peer_exist(peerAddress))
  {
    esp_now_add_peer(&peerInfo);
  }
  esp_err_t result = esp_now_send(peerAddress, (const uint8_t *)message.c_str(), message.length());
  if (result == ESP_OK)
  {
    console.log.println("[MAIN] Broadcast message success");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_INIT)
  {
    console.log.println("[MAIN] ESPNOW not Init.");
  }
  else if (result == ESP_ERR_ESPNOW_ARG)
  {
    console.log.println("[MAIN] Invalid Argument");
  }
  else if (result == ESP_ERR_ESPNOW_INTERNAL)
  {
    console.log.println("[MAIN] Internal Error");
  }
  else if (result == ESP_ERR_ESPNOW_NO_MEM)
  {
    console.log.println("[MAIN] ESP_ERR_ESPNOW_NO_MEM");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
  {
    console.log.println("[MAIN] Peer not found.");
  }
  else
  {
    console.log.println("[MAIN] Unknown error");
  }
}


void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
{
  console.print("[MAIN] Data received from "); printMacAddress(macAddr); console.log.print(": ");
  for(int i = 0; i < dataLen; i++)
  {
    console.log.write(data[i]);
  }
  console.log.println();
}

void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status)
{
  if(status == ESP_NOW_SEND_SUCCESS)
  {
    console.print("[MAIN] Sending to "); printMacAddress(macAddr); console.log.println(" was successful!");
  }
  else
  {
    console.error.printf("[MAIN] Could not send data to "); printMacAddress(macAddr); console.log.printf("(ESP-NOW Error), Error: %d\n", (uint8_t)status);
  }
}
