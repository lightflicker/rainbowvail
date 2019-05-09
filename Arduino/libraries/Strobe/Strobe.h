/*--------------------------------------------------------------------
  This file is part of the Strobe library

  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef STROBE_H
#define STROBE_H

#include <Arduino.h>


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

 

#endif // STROBE_H
