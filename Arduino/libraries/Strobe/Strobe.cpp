/*-------------------------------------------------------------------------
  
  <http://www.gnu.org/licenses/>.
  -------------------------------------------------------------------------*/

#include "Strobe.h"


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