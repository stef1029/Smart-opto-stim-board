#include <Arduino.h>

#define LASER_PIN 7
#define LED_PIN 13
#define MAX_DURATIONS 10
#define SERIAL_BAUD 57600

unsigned long durations[MAX_DURATIONS];
int numDurations = 0;
int numCycles = 20;
unsigned long stimInterval = 5000;
unsigned long previousMillis = 0;
int currentCycle = 0;
int currentDurationIndex = 0;
bool pulseActive = false;
bool sequenceRunning = false;
bool parametersReceived = false;

void setup() {
    Serial.begin(SERIAL_BAUD);
    pinMode(LASER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LASER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
}

void parseParameters(String data) {
    int hashIndex = data.indexOf('#');
    if (hashIndex == -1) return;
    
    numDurations = data.substring(0, hashIndex).toInt();
    if (numDurations <= 0 || numDurations > MAX_DURATIONS) return;
    
    data = data.substring(hashIndex + 1);
    
    for (int i = 0; i < numDurations; i++) {
        int commaIndex = data.indexOf(',');
        if (commaIndex == -1 && i < numDurations - 1) return;
        
        String durationStr;
        if (commaIndex != -1) {
            durationStr = data.substring(0, commaIndex);
            data = data.substring(commaIndex + 1);
        } else {
            durationStr = data;
        }
        durations[i] = durationStr.toInt();
    }
    
    int commaIndex = data.indexOf(',');
    if (commaIndex == -1) return;
    
    numCycles = data.substring(0, commaIndex).toInt();
    stimInterval = data.substring(commaIndex + 1).toInt();
    
    parametersReceived = true;
    Serial.println("params_received");
}

void handleSerialCommand() {
    char cmd = Serial.read();
    
    if (cmd == 'p') {
        delay(10);
        String paramStr = Serial.readStringUntil('\n');
        parseParameters(paramStr);
    }
    else if (cmd == 's' && parametersReceived) {
        sequenceRunning = true;
        currentCycle = 0;
        currentDurationIndex = 0;
        previousMillis = millis();
        pulseActive = false;
    }
    else if (cmd == 'e') {
        sequenceRunning = false;
        digitalWrite(LASER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
    }
}

void loop() {
    if (Serial.available() > 0) {
        handleSerialCommand();
    }
    
    if (sequenceRunning) {
        unsigned long currentMillis = millis();
        
        if (!pulseActive) {
            if (currentMillis - previousMillis >= stimInterval) {
                digitalWrite(LASER_PIN, HIGH);
                digitalWrite(LED_PIN, HIGH);
                pulseActive = true;
                previousMillis = currentMillis;
            }
        } else {
            if (currentMillis - previousMillis >= durations[currentDurationIndex]) {
                digitalWrite(LASER_PIN, LOW);
                digitalWrite(LED_PIN, LOW);
                pulseActive = false;
                currentDurationIndex++;
                
                if (currentDurationIndex >= numDurations) {
                    currentDurationIndex = 0;
                    currentCycle++;
                    
                    if (currentCycle >= numCycles) {
                        sequenceRunning = false;
                        Serial.println("e");
                    }
                }
                previousMillis = currentMillis;
            }
        }
    }
}