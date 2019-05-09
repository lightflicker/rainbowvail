#include <Adafruit_NeoPixel.h>

#define PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      7

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

class strobe : public Adafruit_NeoPixel
{
  private:
    float freq1; //Frequency of the first wave
    float freq2; //Frequency of the first wave
    float freq3; //Frequency of the first wave
    uint16_t n; //Number of pixels
    uint32_t cStrobe_color;
    uint8_t p; //Pin number
    uint8_t t; //Type of the NeoPixel
    int
    iStrobe_period,
    iStrobe_freq;
    float
    fOsc1,
    fOsc2,
    fOsc3,
    fStrobe_offset,
    fStrobe_trig_prev;
    double
    dOsc1_prev,
    dOsc2_prev,
    dOsc3_prev,
    dStrobe_prev,
    dStrobe_start;
    bool
    bOsc1_running,
    bOsc2_running,
    bOsc3_running,
    bStrobe_running,
    bStrobe_on;
  public:

    strobe(uint16_t n, uint8_t p = 6, uint8_t t = NEO_GRB + NEO_KHZ800);
    //strobe(float f1, float f2, float f3);
    void
    application(),
                setCentre(uint32_t c),
                setStrobe(uint32_t c),
                sendStrobe(int period, int f, float o, uint32_t c),
                stopStrobe(),
                startOsc1(),
                stopOsc1(),
                startOsc2(),
                stopOsc2(),
                startOsc3(),
                stopOsc3();
    void setFreq1(float f1) {
      freq1 = f1;
    }
    void setFreq2(float f2) {
      freq2 = f2;
    }
    void setFreq3(float f3) {
      freq3 = f3;
    }
    float getOsc1() {
      return fOsc1;
    }
    float getOsc2() {
      return fOsc2;
    }
    float getOsc3() {
      return fOsc3;
    }

  private:

};


//strobe leds = strobe(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
strobe * leds;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


// Neopixel LED - LED numbers diagram
//                     [6]   [1]
//
//                  [5]   [0]   [2]
//
//                     [4]   [3]


void setup() {
  //Serial.begin(9600);
  //while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  //}

  leds = new strobe(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  leds->begin();
  leds->show(); // Initialize all pixels to 'off'
  leds->setFreq1(0.15);
  leds->setFreq2(11);
  leds->startOsc1();
  //Serial.println("Started");
}

void loop() {

  float r, g, b;
  float brg = 0.8;

  r = (255.0 * leds->getOsc1());
  g = (180.0 * leds->getOsc1());
  b = (100.0 * leds->getOsc1());

  if ( r > 255.0) r = 255.0;
  r = r * brg;
  if ( g > 255.0) g = 255.0;
  g = g * brg;
  if ( b > 255.0) b = 255.0;
  b = b * brg;





  //Serial.println(br);
  leds->setCentre(leds->Color((int)r,(int)g,(int)b));

  leds->sendStrobe(20, 2, 90.0, leds->Color(200, 200, 200));
  leds->application();
  leds->show();


}



//-----------------------------------------------------------------------------------------
//    Definition of the class
//-----------------------------------------------------------------------------------------

strobe::strobe(uint16_t n, uint8_t p, uint8_t t) : Adafruit_NeoPixel(n, p, t)
{
  bOsc1_running = false;
  fStrobe_trig_prev = 0.0;
}


void strobe::application()
{
  float fOsc_strobe;

  //Oscilator 1
  if (bOsc1_running == true)
  {
    fOsc1 = (sin(freq1 * (millis() - dOsc1_prev) / 1000 * 2 * PI) + 1.0) / 2.0;
  }

  //Oscilator 2
  if (bOsc2_running == true)
  {
    fOsc2 = (sin(freq2 * (millis() - dOsc2_prev) / 1000 * 2 * PI) + 1.0) / 2.0;
  }

  //Oscilator 3
  if (bOsc3_running == true)
  {
    fOsc3 = (sin(freq3 * (millis() - dOsc3_prev) / 1000 * 2 * PI) + 1.0) / 2.0;
  }


  //Strobe control
  switch (iStrobe_freq)
  {
    case 1:
      fOsc_strobe = sin(freq1 * (millis() - dOsc1_prev) / 1000 * 2 * PI + fStrobe_offset);
      break;
    case 2:
      fOsc_strobe = sin(freq2 * (millis() - dOsc2_prev) / 1000 * 2 * PI + fStrobe_offset);
      break;
    case 3:
      fOsc_strobe = sin(freq3 * (millis() - dOsc3_prev) / 1000 * 2 * PI + fStrobe_offset);
      break;
  }

  if (bStrobe_running == true)
  {
    if ((fOsc_strobe > 0.0) && (fStrobe_trig_prev <= 0.0) && !bStrobe_on)
    {
      dStrobe_start = millis();
      setStrobe(cStrobe_color);
      bStrobe_on = true;
    }
  }
  if ((millis() - dStrobe_start) > (double)iStrobe_period)
  {
    setStrobe(Color(0, 0, 0));
    bStrobe_on = false;
  }
  fStrobe_trig_prev = fOsc_strobe;






}



void strobe::startOsc1()
{
  bOsc1_running = true;
  fOsc1 = 0.0;
  dOsc1_prev = millis();
}

void strobe::stopOsc1()
{
  bOsc1_running = false;
}

void strobe::startOsc2()
{
  bOsc2_running = true;
  fOsc2 = 0.0;
  dOsc2_prev = millis();
}

void strobe::stopOsc2()
{
  bOsc2_running = false;
}

void strobe::startOsc3()
{
  bOsc3_running = true;
  fOsc3 = 0.0;
  dOsc3_prev = millis();
}

void strobe::stopOsc3()
{
  bOsc3_running = false;
}

void strobe::setCentre(uint32_t c)
{
  setPixelColor(0, c);
  setPixelColor(2, c);
  setPixelColor(4, c);
  setPixelColor(6, c);
}

void strobe::setStrobe(uint32_t c)
{
  setPixelColor(1, c);
  setPixelColor(3, c);
  setPixelColor(5, c);
}


void strobe::sendStrobe(int p, int f, float o, uint32_t c)
{
  bStrobe_running = true;
  iStrobe_period = p;
  iStrobe_freq = f;
  fStrobe_offset = o / 360.0 * 2 * PI;
  cStrobe_color = c;
}

void strobe::stopStrobe()
{
  bStrobe_running = false;
}

