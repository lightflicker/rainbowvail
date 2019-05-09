//#include <RGBConverter.h>
#include <Arduino.h>
//#include <WString.h>
#include "Strobe.h"


#define PIN 2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      14

// create a queue of steps.
LoadSeqStep NewStep;
boolean bStepComplete = false;  // whether the string is complete
const byte numChars = 200;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
boolean newData = false;
long start_time = 0;
boolean time_taken = false;



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Strobe   leds = Strobe(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


// Neopixel LED - LED numbers diagram (Left)
//                     [6]   [1]
//
//                  [5]   [0]   [2]
//
//                     [4]   [3]

// Neopixel LED - LED numbers diagram (Right)
//                     [13]   [8]
//
//                  [12]   [7]   [9]
//
//                     [11]   [10]

//----------------------------------------------------------------
// Definition of light colors
//----------------------------------------------------------------



//--------------------------------------------------------------------

void setup() {
  leds.begin();
  leds.show();
  Serial.begin(38400);
  Serial.println("Started");
  pinMode(13, OUTPUT);
  leds.reqNextStep();
  
}

void loop()
{
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        Serial.println("Received params: ");
        Serial.println(receivedChars);
        parseData();
        showParsedData();
        newData = false;
    }
  
  leds.Application();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    

    while (Serial.available() > 0 && newData == false) {
        if(!time_taken)
        {
          start_time = millis();
          time_taken = true;  
        }
        digitalWrite(13, HIGH);
        rc = Serial.read();
        Serial.write(rc);
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,";");      // get the first part - the string
    leds.NextStep.iTypeC = (int) atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fCtr_A = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fCtr_B = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fCtr_C = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fCtr_D = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fCtr_E = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOn.h = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOn.s = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOn.l = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOff.h = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOff.s = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslCOff.l = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.iTypeS = (int) atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fStrb_A = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fStrb_B = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fStrb_C = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fStrb_D = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.fStrb_E = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.lPattern = (long) atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOn.h = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOn.s = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOn.l = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOff.h = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOff.s = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.hslSOff.l = atof(strtokIndx);
    strtokIndx = strtok(NULL, ";");
    leds.NextStep.lSeqTime = (long) atof(strtokIndx);
    Serial.println(leds.NextStep.lSeqTime);
    bStepComplete = true;
    leds.reqLoadComplete();
    digitalWrite(13, LOW);
    Serial.print("Transfer took: ");
    Serial.print(millis() - start_time);
    Serial.println("ms");
    time_taken = false;
}

void showParsedData() {
    
    Serial.println("Load Complete");
    bStepComplete = false;
}
