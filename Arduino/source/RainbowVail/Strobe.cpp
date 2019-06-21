/*-----------------------------------------------------------------------

  Author: M.Hewelt
  Updated on: 15/5/2019
  Licence: GPL-3.0

-------------------------------------------------------------------------*/

#include "Strobe.h"


Strobe::Strobe(uint16_t n, uint8_t p, uint8_t t) : Adafruit_NeoPixel(n, p, t)
{
  bOsc1_running = false;
  bOsc2_running = false;
  bOsc3_running = false;
  bOsc4_running = false;
  iStep = 0;
  bRunCmd = false;
  bLoadingNextStep = false;
  bReqNextStep = false;

}

void Strobe::Application()
{

  byte incomingByte = 0;
  //float dOsc_strobe;
  byte rgb[3];
  
  //---------------------------------------------------------------
  //Cyclic call every 10ms
  //---------------------------------------------------------------
  static unsigned long iStartTime10 = millis();
  if ((millis() - iStartTime10) >= 10)
  {
    Cycle10();
    iStartTime10 = millis();
  }

  //Cyclic call every 100ms
  static unsigned long iStartTime100 = millis();
  if ((millis() - iStartTime100) >= 100)
  {
    Cycle100();
    iStartTime100 = millis();
  }

  //Cyclic call every 1000ms
  static unsigned long iStartTime1000 = millis();
  if ((millis() - iStartTime1000) >= 1000)
  {
    Cycle1000();
    iStartTime1000 = millis();
  }


  //---------------------------------------------------------------
  //OSCILATORS
  //---------------------------------------------------------------
  //Oscilator 1
  if (bOsc1_running == true)
  {
    static double dOsc1_prev;
    dOsc1 = (sin(freq1 * (millis() - lOsc1_start) / 1000 * 2 * PI) + 1.0) / 2.0;
    if (dOsc1 > 0.0 && dOsc1_prev <= 0.0)
    {
      bOsc1_one_shot = true;
    }
    else
    {
      bOsc1_one_shot = false;
    }
    dOsc1_prev = dOsc1;
  }

  //Oscilator 2
  if (bOsc2_running == true)
  {
    static double dOsc2_prev;
    dOsc2 = (sin(freq2 * (millis() - lOsc2_start) / 1000 * 2 * PI) + 1.0) / 2.0;
    if (dOsc2 > 0.0 && dOsc2_prev <= 0.0)
    {
      bOsc2_one_shot = true;
    }
    else
    {
      bOsc2_one_shot = false;
    }
    dOsc2_prev = dOsc2;
  }

  //Oscilator 3
  if (bOsc3_running == true)
  {
    static double dOsc3_prev;
    dOsc3 = (sin(freq3 * (millis() - lOsc3_start) / 1000 * 2 * PI) + 1.0) / 2.0;
    if (dOsc3 > 0.0 && dOsc3_prev <= 0.0)
    {
      bOsc3_one_shot = true;
    }
    else
    {
      bOsc3_one_shot = false;
    }
    dOsc3_prev = dOsc3;
  }

  //Oscilator 4
  if (bOsc4_running == true)
  {
    static double dOsc4_prev;
    dOsc4 = (sin(freq4 * (millis() - lOsc4_start) / 1000 * 2 * PI) + 1.0) / 2.0;
    if (dOsc4 > 0.0 && dOsc4_prev <= 0.0)
    {
      bOsc4_one_shot = true;
    }
    else
    {
      bOsc4_one_shot = false;
    }
    dOsc4_prev = dOsc3;
  }

  if(bRunCmd)
  {
  //---------------------------------------
  // SEQUENCER
  //---------------------------------------
  
    Sequencer();  
  
  //---------------------------------------
  // Loading of the colors to the lights and execution of the show() function.
  //---------------------------------------


  c.hsvToRgb(colCentreL.h, colCentreL.s, colCentreL.l, rgb);
  setCentreLeft(Color(rgb[0], rgb[1], rgb[2]));
  c.hsvToRgb(colStrobeL.h, colStrobeL.s, colStrobeL.l, rgb);
  setRingLeft(Color(rgb[0], rgb[1], rgb[2]));
  c.hsvToRgb(colCentreR.h, colCentreR.s, colCentreR.l, rgb);
  setCentreRight(Color(rgb[0], rgb[1], rgb[2]));
  c.hsvToRgb(colStrobeR.h, colStrobeR.s, colStrobeR.l, rgb);
  setRingRight(Color(rgb[0], rgb[1], rgb[2]));
  show();
  }
  }


//------------------------------------------------------------------------------------
// Cyclic calls 10ms, 100ms and 1000ms, to be used with the user defined functions
//------------------------------------------------------------------------------------
void Strobe::Cycle10()
{
}

void Strobe::Cycle100()
{

}

void Strobe::Cycle1000()
{
}


//----------------------------------------------
//Sequencer Definition
//----------------------------------------------

void Strobe::Sequencer()
{
  String msg;
  //If the current step is within 3s to its end, load the next step from the PC
  if (((millis() - iStepStartTime) > (Step.lSeqTime - 300)) && !getReqStatus())
  {
    reqNextStep();
  }
      
  //Begin with finding out if the new step should begin
  if ((millis() - iStepStartTime) > Step.lSeqTime)
  {
    iStepStartTime = millis();
    //Load new step
    Step = NextStep;
    Serial.print("Exchange complete. lSeqTime: ");
    Serial.print(Step.lSeqTime);
    Serial.println("ms");
    bReqNextStep = false;

  }
  
  if (Step.iTypeC > 0 || Step.iTypeS > 0)     //If any of the other sequence type either Centre or Strobe is greater than 0, then decode the seuqence
  {

    //Centre lights - Choose and run type accroding to iTypeC
    switch (Step.iTypeC)
    {
      case 1:
        type1_CentreL(Step.hslCOn, Step.hslCOff, Step.lSeqTime);
        type1_CentreR(Step.hslCOn, Step.hslCOff, Step.lSeqTime);
        break;
      case 2:
        type2_CentreL(Step.fCtr_A, Step.fCtr_C, Step.fCtr_D, Step.fCtr_E, Step.hslCOn, Step.hslCOff);
        type2_CentreR(Step.fCtr_B, Step.fCtr_C, Step.fCtr_D, Step.fCtr_E, Step.hslCOn, Step.hslCOff);
        break;
      case 3:
        type3_CentreL(Step.fCtr_A, Step.fCtr_B, Step.fCtr_C, Step.fCtr_D, Step.fCtr_E, Step.hslCOn, Step.hslCOff, Step.lSeqTime);
        type3_CentreR(Step.fCtr_A, Step.fCtr_B, Step.fCtr_C, Step.fCtr_D, Step.fCtr_E, Step.hslCOn, Step.hslCOff, Step.lSeqTime);
        break;
      case 4:
        //Function to run Type 4 Centre Light
        break;
      case 5:
        //Function to run Type 5 Centre Light
        break;
      case 6:
        //Function to run Type 6 Centre Light
        break;
      case 7:
        //Function to run Type 7 Centre Light
        break;
      case 8:
        //Function to run Type 8 Centre Light
        break;


      default:

        break;

    }

    //Strobe lights - Choose and run type accroding to iTypeS
    switch (Step.iTypeS)
    {
      case 1:
        type1_StrobeL(Step.hslSOn);
        type1_StrobeR(Step.hslSOn);
        break;
      case 2:
        type2_StrobeL(Step.fStrb_A, Step.fStrb_C, Step.fStrb_D, Step.hslSOn, Step.hslSOff);
        type2_StrobeR(Step.fStrb_B, Step.fStrb_C, Step.fStrb_D, Step.hslSOn, Step.hslSOff);
        break;
      case 3:
        type3_StrobeL(Step.lPattern, Step.fStrb_A, Step.fStrb_D, Step.hslSOn, Step.hslSOff);
        type3_StrobeR(Step.lPattern, Step.fStrb_B, Step.fStrb_D, Step.hslSOn, Step.hslSOff);
        break;
      case 4:
        //Function to run Type 4 Centre Light
        break;
      case 5:
        //Function to run Type 5 Centre Light
        break;
      case 6:
        //Function to run Type 6 Centre Light
        break;
      case 7:
        //Function to run Type 7 Centre Light
        break;
      case 8:
        //Function to run Type 8 Centre Light
        break;
      default:

        break;
    }
  }
  else  //Otherwise turn on the standby light;
  {
    LightsStandby();
  }
}


void Strobe::setCentreLeft(uint32_t c)
{
  setPixelColor(0, c);
  setPixelColor(2, c);
  setPixelColor(4, c);
  setPixelColor(6, c);
}

void Strobe::setRingLeft(uint32_t c)
{
  setPixelColor(1, c);
  setPixelColor(3, c);
  setPixelColor(5, c);
}

void Strobe::setCentreRight(uint32_t c)
{
  setPixelColor(7, c);
  setPixelColor(9, c);
  setPixelColor(11, c);
  setPixelColor(13, c);
}

void Strobe::setRingRight(uint32_t c)
{
  setPixelColor(8, c);
  setPixelColor(10, c);
  setPixelColor(12, c);
}

void Strobe::startOsc1()
{
  bOsc1_running = true;
  lOsc1_start = millis();
}

void Strobe::stopOsc1()
{
  bOsc1_running = false;
}

void Strobe::startOsc2()
{
  bOsc2_running = true;
  lOsc2_start = millis();
}

void Strobe::stopOsc2()
{
  bOsc2_running = false;
}

void Strobe::startOsc3()
{
  bOsc3_running = true;
  lOsc3_start = millis();
}

void Strobe::stopOsc3()
{
  bOsc3_running = false;
}

void Strobe::startOsc4()
{
  bOsc4_running = true;
  lOsc4_start = millis();
}

void Strobe::stopOsc4()
{
  bOsc4_running = false;
}

void Strobe::reqNextStep()
{
  Serial.println("C000");
  bReqNextStep = true;
}

void Strobe::reqNextStepSent()
{
  
  bLoadingNextStep = true;

}


void Strobe::reqLoadComplete()
{
  bRunCmd = true;  
}


void Strobe::Filter(float nv, float& cv, float a)
{
  static unsigned long start_time;

  if ((millis() - start_time) > 10) //Constant 10ms refresh rate, thus the time constant of attenuation os 10ms
  {
    cv += (nv - cv) * a;
    start_time = millis();
  }

}

//-----------------------------------------------------------------------------------
// Definition of Light types called by the sequencer
//-----------------------------------------------------------------------------------

// CENTRE - TYPE 1
// Type 1 produces light transition from c_start HSL to c_end HSL within
// the given ramp_time in milliseconds
void Strobe::type1_CentreL(HSL c_start, HSL c_end, unsigned long ramp_time)
{
  static unsigned long time_start = millis();
  double change_rate;

  change_rate = ramp_time > 0 ? ((double)(millis() - time_start) / (double) ramp_time) : (double) 0.0;
  colCentreL.h = c_start.h + (c_end.h - c_start.h) * change_rate;
  colCentreL.s = c_start.s + (c_end.s - c_start.s) * change_rate;
  colCentreL.l = c_start.l + (c_end.l - c_start.l) * change_rate;

}

void Strobe::type1_CentreR(HSL c_start, HSL c_end, unsigned long ramp_time)
{
  static unsigned long time_start = millis();
  double change_rate;

  change_rate = ramp_time > 0 ? ((double)(millis() - time_start) / (double) ramp_time) : (double) 0.0;

  colCentreR.h = c_start.h + (c_end.h - c_start.h) * change_rate;
  colCentreR.s = c_start.s + (c_end.s - c_start.s) * change_rate;
  colCentreR.l = c_start.l + (c_end.l - c_start.l) * change_rate;

}



// CENTRE - TYPE 2
// Type 2 produces light pulses with soft rising and falling edges of
// the on and off transitons. The soft edges are modelled as exponential
// function. This produces a similar effect to a hallogen light turning
// on and off
// f - Frequency of the pulses
// fill - Period fill factor where 1 equals always on, 0 always off and
// 0.5 a half period of on and half period of off
//
void Strobe::type2_CentreL(float f, float fill, float a, float phase, HSL c_on, HSL c_off)
{
  double p;
  static unsigned long time_start = millis();
  static unsigned long f_start_time;
  double period = 1000.0 / f;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  unsigned long current_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  HSL *c;

  p = phase >= 0.0 ? phase : 0.0;

  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)


  if (current_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    c = &c_off;
  }

  if ((current_time > (unsigned long) offset) && (current_time <= (unsigned long) pulse))           //Start the flash period
  {
    c = &c_on;
  }

  if ((current_time > (unsigned long) pulse) && (current_time <= (unsigned long) period))           //Starts the off period
  {
    c = &c_off;
  }

  if (current_time > period)
  {
    time_start = millis();
  }


  colCentreL.h = c->h;
  colCentreL.s = c->s;

  if ((millis() - f_start_time) > 10) //Constant 10ms refresh rate, thus the time constant of attenuation is 10ms
  {
    // colCentreL.h = (c->h - colCentreL.h) * a + colCentreL.h;
    // colCentreL.s = (c->s - colCentreL.s) * a + colCentreL.s;
    colCentreL.l = (c->l - colCentreL.l) * a + colCentreL.l;
    f_start_time = millis();
  }
}

void Strobe::type2_CentreR(float f, float fill, float a, float phase, HSL c_on, HSL c_off)
{
  double p;
  static unsigned long time_start = millis();
  static unsigned long f_start_time;
  double period = 1000.0 / f;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  unsigned long current_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  HSL *c;

  p = phase < 0.0 ? phase * -1.0 : 0.0;

  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)


  if (current_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    c = &c_off;
  }

  if ((current_time > (unsigned long) offset) && (current_time <= (unsigned long) pulse))           //Start the flash period
  {
    c = &c_on;
  }

  if ((current_time > (unsigned long) pulse) && (current_time <= (unsigned long) period))           //Starts the off period
  {
    c = &c_off;
  }

  if (current_time > period)
  {
    time_start = millis();
  }


  colCentreR.h = c->h;
  colCentreR.s = c->s;

  if ((millis() - f_start_time) > 10) //Constant 10ms refresh rate, thus the time constant of attenuation is 10ms
  {
    colCentreR.l = (c->l - colCentreR.l) * a + colCentreR.l;
    f_start_time = millis();
  }
}


// CENTRE - TYPE 3
void Strobe::type3_CentreL(float f_start, float f_end, float fill, float a, float phase, HSL c_on, HSL c_off, unsigned long ramp_time)
{
  double p;
  static unsigned long time_start = millis();
  static unsigned long function_start = millis();
  static unsigned long f_start_time;
  double period;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  double change_rate;
  unsigned long current_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  HSL *c;

  change_rate = ramp_time > 0 ? ((double)(millis() - function_start) / (double) ramp_time) : (double) 0.0;

  period = 1000.0 / (f_start + (f_end - f_start) * change_rate);

  p = phase >= 0.0 ? phase : 0.0;
  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)

  if (current_time > period)
  {
    time_start = millis();
  }


  if (current_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    c = &c_off;
  }

  if ((current_time > (unsigned long) offset) && (current_time <= (unsigned long) pulse))           //Start the flash period
  {
    c = &c_on;
  }

  if ((current_time > (unsigned long) pulse) && (current_time <= (unsigned long) period))           //Starts the off period
  {
    c = &c_off;
  }



  colCentreL.h = c->h;
  colCentreL.s = c->s;

  if ((millis() - f_start_time) > 10) //Constant 10ms refresh rate, thus the time constant of attenuation is 10ms
  {
    colCentreL.l = (c->l - colCentreL.l) * a + colCentreL.l;
    f_start_time = millis();
  }
}

void Strobe::type3_CentreR(float f_start, float f_end, float fill, float a, float phase, HSL c_on, HSL c_off, unsigned long ramp_time)
{
  double p;
  static unsigned long time_start = millis();
  static unsigned long function_start = millis();
  static unsigned long f_start_time;
  double period;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  double change_rate;
  unsigned long current_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  HSL *c;

  change_rate = ramp_time > 0 ? ((double)(millis() - function_start) / (double) ramp_time) : (double) 0.0;

  period = 1000.0 / (f_start + (f_end - f_start) * change_rate);


  p = phase < 0.0 ? phase * -1.0 : 0.0;

  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)


  if (current_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    c = &c_off;
  }

  if ((current_time > (unsigned long) offset) && (current_time <= (unsigned long) pulse))           //Start the flash period
  {
    c = &c_on;
  }

  if ((current_time > (unsigned long) pulse) && (current_time <= (unsigned long) period))           //Starts the off period
  {
    c = &c_off;
  }

  if (current_time > period)
  {
    time_start = millis();
  }


  colCentreR.h = c->h;
  colCentreR.s = c->s;

  if ((millis() - f_start_time) > 10) //Constant 10ms refresh rate, thus the time constant of attenuation is 10ms
  {
    colCentreR.l = (c->l - colCentreR.l) * a + colCentreR.l;
    f_start_time = millis();
  }
}



// STROBE - TYPE 1
void Strobe::type1_StrobeL(HSL c_on)
{
  colStrobeL = c_on;
}

void Strobe::type1_StrobeR(HSL c_on)
{
  colStrobeR = c_on;
}


// STROBE - TYPE 2
void Strobe::type2_StrobeL(float f, float fill, float phase, HSL c_on, HSL c_off)
{
  double p;
  static unsigned long time_start = millis();       //Record timestamp of the function start
  double period = 1000.0 / f;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  unsigned long start_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  p = phase >= 0 ? phase : 0;

  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)


  if (start_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    colStrobeL = c_off;
  }

  if ((start_time > (unsigned long) offset) && (start_time <= (unsigned long) pulse))           //Start the flash period
  {
    colStrobeL = c_on;
  }

  if ((start_time > (unsigned long) pulse) && (start_time <= (unsigned long) period))           //Starts the off period
  {
    colStrobeL = c_off;
  }

  if (start_time > (unsigned long) period) //Resets the time on the end of the cycle
  {
    time_start = millis();

  }
}

void Strobe::type2_StrobeR(float f, float fill, float phase, HSL c_on, HSL c_off)
{
  double p;
  static unsigned long time_start = millis();       //Record timestamp of the function start
  double period = 1000.0 / f;                       //How long is the period of one cycle 1/f
  double offset;
  double pulse;
  unsigned long start_time = millis() - time_start; //Calculation of the time differential between the start time and the current millisecond value

  p = phase < 0 ? phase * -1.0 : 0;

  offset = period * p / 360.0;    //If the phase offset has been defined, it defines a delay in milliseconds before the flash starts
  pulse = period * fill + offset;                     //How long is the flash pulse base on the PWM factor plus phase offset (if > 0)


  if (start_time <= (unsigned long) offset)         //Starts the off period of the pahse offset
  {
    colStrobeR = c_off;
  }

  if ((start_time > (unsigned long) offset) && (start_time <= (unsigned long) pulse))           //Start the flash period
  {
    colStrobeR = c_on;
  }

  if ((start_time > (unsigned long) pulse) && (start_time <= (unsigned long) period))           //Starts the off period
  {
    colStrobeR = c_off;
  }

  if (start_time > (unsigned long) period) //Resets the time on the end of the cycle
  {
    time_start = millis();

  }
}


// STROBE - TYPE 3
void Strobe::type3_StrobeL(unsigned long p, float f, float phase, HSL c_on, HSL c_off)
{
  static unsigned long start_time = 0;
  unsigned long period = (unsigned long) (1 / f * 1000000.0);
  unsigned long pulse = (unsigned long) (1 / f * 1000000.0) / 32.0;
  static int bit_id = 0;

  if ((micros() - start_time) > pulse)
  {
    colStrobeL = c_off;
    if ((0x00000001 & p >> bit_id) == 1)
    {
      colStrobeL = c_on;
    }

    bit_id++;
    if (bit_id > 31)
    {
      bit_id = 0;
    }

    start_time = micros();
  }
}


void Strobe::type3_StrobeR(unsigned long p, float f, float phase, HSL c_on, HSL c_off)
{
  static unsigned long start_time = 0;
  unsigned long period = (unsigned long) (1 / f * 1000000.0);
  unsigned long pulse = (unsigned long) (1 / f * 1000000.0) / 32.0;
  static int bit_id = 0;

  if ((micros() - start_time) > pulse)
  {
    colStrobeR = c_off;
    if ((0x00000001 & p >> bit_id) == 1)
    {
      colStrobeR = c_on;
    }

    bit_id++;
    if (bit_id > 31)
    {
      bit_id = 0;
    }

    start_time = micros();
  }
}


void Strobe::LightsStandby()
{
  colCentreL = HSL::getHSL(0.666, 1.0, 0.1);
  colCentreR = HSL::getHSL(0.666, 1.0, 0.1);
  colStrobeL = HSL::getHSL(0.666, 1.0, 0.0);
  colStrobeR = HSL::getHSL(0.666, 1.0, 0.0);

}
