
/*-----------------------------------------------------------------------

  Author: M.Hewelt
  Updated on: 15/5/2019
  Licence: GPL-3.0
  
  This file is part of the Strobe library

  --------------------------------------------------------------------*/
#include <Adafruit_NeoPixel.h>
#include <RGBConverter.h>

class Adafruit_NeoPixel;

#ifndef STROBE_H
#define STROBE_H

struct HSL            //Strunct defining colour of the light in HSL notation
{
  float h;     //Hue
  float s;     //Saturation
  float l;     //Lightness
  HSL()
  {
      h = 0.0;     //Hue
      s = 0.0;     //Saturation
      l = 0.5;     //Lightness
  }
  HSL(float h, float s, float l) : h(h), s(s), l(l)
  {
    
   }
  static HSL getHSL(float h, float s, float l)
  {
      HSL c;
      c.h = h;
      c.s = s;
      c.l = l;
      return c;      
  }
  void operator=(const HSL &c)
      {
        h = c.h;
        s = c.s;
        l = c.l; 
      }
};

//Definition of HSL colors
//HSL hslOff = HSL::getHSL(0.0,0.0,0.0);
//HSL hslDarkBlue = HSL::getHSL(0.666,1.0,0.2);



struct SeqStep
{
  //Centre light
  int       iTypeC;             //Type of sequence for the Centre lights
  float     fCtr_A;
  float     fCtr_B;
  float     fCtr_C;
  float     fCtr_D;
  float     fCtr_E;
  HSL       hslCOn;                 //Light color in HSL notation, when lit
  HSL       hslCOff;                 //Light color in HSL notation, when off   
  


  //Strobe light
  int       iTypeS;              //Type of sequence for the Strobe lights
  float     fStrb_A;
  float     fStrb_B;
  float     fStrb_C;
  float     fStrb_D;
  float     fStrb_E;
  unsigned long lPattern;         //Pattern
  HSL       hslSOn;                  //Light color in HSL notation, when lit
  HSL       hslSOff;                 //Light color in HSL notation , when off  


  //Common
  unsigned long     lSeqTime;         //Period in seconds, defines how long the step will last

  SeqStep()
  {
        iTypeC = 0;
        fCtr_A = 0.0;
        fCtr_B = 0.0;
        fCtr_C = 0.0;
        fCtr_D = 0.0;
        fCtr_E = 0.0;
        hslCOn = HSL::getHSL(0.0,0.0,0.0);
        hslCOff = HSL::getHSL(0.0,0.0,0.0);
        iTypeS = 0;
        fStrb_A = 0.0;
        fStrb_B = 0.0;
        fStrb_C = 0.0;
        fStrb_D = 0.0;
        fStrb_E = 0.0;
        lPattern = 0x00000000;
        hslSOn = HSL::getHSL(0.0,0.0,0.0);
        hslSOff = HSL::getHSL(0.0,0.0,0.0);
        lSeqTime = 0;
  }
  void operator=(const SeqStep &s)
      {
        iTypeC = s.iTypeC;
        fCtr_A = s.fCtr_A;
        fCtr_B = s.fCtr_B;
        fCtr_C = s.fCtr_C;
        fCtr_D = s.fCtr_D;
        fCtr_E = s.fCtr_E;
        hslCOn = s.hslCOn;
        hslCOff = s.hslCOff;
        iTypeS = s.iTypeS;
        fStrb_A = s.fStrb_A;
        fStrb_B = s.fStrb_B;
        fStrb_C = s.fStrb_C;
        fStrb_D = s.fStrb_D;
        fStrb_E = s.fStrb_E;
        lPattern = s.lPattern;
        hslSOn = s.hslSOn;
        hslSOff = s.hslSOff;
        lSeqTime = s.lSeqTime;
      }
  
  
};


struct LoadSeqStep
{
  //Centre light
  int       iTypeC;             //Step mode for the Centre lights
  float     fCtr_A;             //Centre Light - Parameter A - Usage depending on the step mode 
  float     fCtr_B;             //Centre Light - Parameter B - Usage depending on the step mode 
  float     fCtr_C;             //Centre Light - Parameter C - Usage depending on the step mode 
  float     fCtr_D;             //Centre Light - Parameter D - Usage depending on the step mode 
  float     fCtr_E;             //Centre Light - Parameter E - Usage depending on the step mode 
  float     fCtr_H_On;                 //Centre Light - color in HSL when On; Hue
  float     fCtr_S_On;                 //Centre Light - color in HSL when On; Saturtion
  float     fCtr_L_On;                 //Centre Light - color in HSL when On; Luminocity
  float     fCtr_H_Off;                //Centre Light - color in HSL when Off; Hue 
  float     fCtr_S_Off;                //Centre Light - color in HSL when Off; Saturation
  float     fCtr_L_Off;                //Centre Light - color in HSL when Off; Luminocity
  


  //Strobe light
  int       iTypeS;              //Step mode for the Centre lights
  float     fStrb_A;             //Strobe Light - Parameter A - Usage depending on the step mode
  float     fStrb_B;             //Strobe Light - Parameter A - Usage depending on the step mode 
  float     fStrb_C;             //Strobe Light - Parameter A - Usage depending on the step mode 
  float     fStrb_D;             //Strobe Light - Parameter A - Usage depending on the step mode  
  float     fStrb_E;             //Strobe Light - Parameter A - Usage depending on the step mode 
  unsigned long lPattern;         //Pattern
  float     fStrb_H_On;                 //Strobe Light - color in HSL when On; Hue
  float     fStrb_S_On;                 //Strobe Light - color in HSL when On; Saturtion
  float     fStrb_L_On;                 //Strobe Light - color in HSL when On; Luminocity
  float     fStrb_H_Off;                //Strobe Light - color in HSL when Off; Hue 
  float     fStrb_S_Off;                //Strobe Light - color in HSL when Off; Saturation
  float     fStrb_L_Off;                //Strobe Light - color in HSL when Off; Luminocity
  

  //Common
  unsigned long     lSeqTime;         //Period in seconds, defines how long the step will last

  void operator=(const LoadSeqStep &s)
      {
        iTypeC = s.iTypeC;
        fCtr_A = s.fCtr_A;
        fCtr_B = s.fCtr_B;
        fCtr_C = s.fCtr_C;
        fCtr_D = s.fCtr_D;
        fCtr_E = s.fCtr_E;
        fCtr_H_On = s.fCtr_H_On;
        fCtr_S_On = s.fCtr_S_On;
        fCtr_L_On = s.fCtr_L_On;
        fCtr_H_Off = s.fCtr_H_Off;
        fCtr_S_Off = s.fCtr_S_Off;
        fCtr_L_Off = s.fCtr_L_Off;
        iTypeS = s.iTypeS;
        fStrb_A = s.fStrb_A;
        fStrb_B = s.fStrb_B;
        fStrb_C = s.fStrb_C;
        fStrb_D = s.fStrb_D;
        fStrb_E = s.fStrb_E;
        lPattern = s.lPattern;
        fStrb_H_On = s.fStrb_H_On;
        fStrb_S_On = s.fStrb_H_On;
        fStrb_L_On = s.fStrb_H_On;
        fStrb_H_Off = s.fStrb_H_Off;
        fStrb_S_Off = s.fStrb_S_Off;
        fStrb_L_Off = s.fStrb_L_Off;
        lSeqTime = s.lSeqTime;
      }
  
  
};


class Strobe : public Adafruit_NeoPixel {
  public:
    Strobe(uint16_t n, uint8_t p = 6, uint8_t t = NEO_GRB + NEO_KHZ800);
    void
                Application(),              //This will be executed every scan
                Cycle10(),                  //Cyclic scan, executed every 10ms
                Cycle100(),                 //Cyclic scan, executed every 100ms
                Cycle1000(),                //Cyclic scan, executed every 1000ms
                Sequencer(),                //Responsible for running the light sequence
                setCentreLeft(uint32_t c),  //Configures Centre Left leds with a color value from paramter c
                setRingLeft(uint32_t c),    //Configures the ring left leds with a color value from paramter c
                setCentreRight(uint32_t c),  //Configures Centre Left leds with a color value from paramter c
                setRingRight(uint32_t c),    //Configures the ring left leds with a color value from paramter c
                startOsc1(),
                stopOsc1(),
                startOsc2(),
                stopOsc2(),
                startOsc3(),
                stopOsc3(),
                startOsc4(),
                stopOsc4(),
                reqNextStep(),
                reqNextStepSent(),
                reqLoadComplete(),
                                
                type1_CentreL(HSL c_start, HSL c_end, unsigned long ramp_time),
                type1_CentreR(HSL c_start, HSL c_end, unsigned long ramp_time),
                type2_CentreL(float f, float fill, float a, float phase, HSL c_on, HSL c_off),
                type2_CentreR(float f, float fill, float a, float phase, HSL c_on, HSL c_off),
                type3_CentreL(float f_start, float f_end, float fill, float a, float phase, HSL c_on, HSL c_off, unsigned long ramp_time),
                type3_CentreR(float f_start, float f_end, float fill, float a, float phase, HSL c_on, HSL c_off, unsigned long ramp_time),
                type4_CentreL(float f, float fill, float phase, HSL c_on, HSL c_off),
                type4_CentreR(float f, float fill, float phase, HSL c_on, HSL c_off),
                
                type1_StrobeL(HSL c_on),
                type1_StrobeR(HSL c_on),
                type2_StrobeL(float f, float fill, float phase, HSL c_on, HSL c_off),
                type2_StrobeR(float f, float fill, float phase, HSL c_on, HSL c_off),
                type3_StrobeL(unsigned long p, float f, float phase, HSL c_on, HSL c_off),
                type3_StrobeR(unsigned long p, float f, float phase, HSL c_on, HSL c_off),
                type4_StrobeL(float f, float fill, float phase, HSL c_on, HSL c_off),
                type4_StrobeR(float f, float fill, float phase, HSL c_on, HSL c_off),
                LoadStep(const LoadSeqStep &s);
    
                                
    void setFreq1(float f1) {
      freq1 = f1;
    }
    void setFreq2(float f2) {
      freq2 = f2;
    }
    void setFreq3(float f3) {
      freq3 = f3;
    }
    void setFreq4(float f4) {
      freq4 = f4;
    }
    double getOsc1() {
      return dOsc1;
    }
    double getOsc2() {
      return dOsc2;
    }
    double getOsc3() {
      return dOsc3;
    }
    double getOsc4() {
      return dOsc4;
    }
    bool getReqStatus(){
      return bReqNextStep;
    }
    bool getLoadingStatus(){
      return bLoadingNextStep;
    }
    void Filter(  float nv, //New value
                  float& cv, //Reference to the current value
                  float a); //Attenuation
    HSL
        colCentreL,
        colStrobeL,
        colCentreR,
        colStrobeR;
    SeqStep Step;                 //Holds current sequence step
    SeqStep NextStep;             //Holds the next step
  private:
    void LightsStandby();             //Changes lights color to blue, low lightness; this is for standb periods, where no sequence is running;
    uint16_t  n,                      //Number of pixels
              iStep;                  //Holds value of the current sequence step; iStep is initialised by the creator and manupulated by Sequencer();              
    uint32_t  cStrobe_color;
    unsigned long   iStepStartTime;         //Current step start time in milliseconds; this value is initialised on the begining of each step, and is serves as a reference to calculate step's elapsed time               
    uint8_t p; //Pin number
    uint8_t t; //Type of the NeoPixel
    double
    freq1, //Frequency of the first wav
    freq2, //Frequency of the first wav
    freq3, //Frequency of the first wav
    freq4, //Frequency of the first wav
    dOsc1,
    dOsc2,
    dOsc3,
    dOsc4;
    bool
    bOsc1_running,
    bOsc1_one_shot,
    bOsc2_running,
    bOsc2_one_shot,
    bOsc3_running,
    bOsc3_one_shot,
    bOsc4_running,
    bOsc4_one_shot,
    bStrobe_running,
    bStrobe_on,
    bLoadingNextStep,
    bReqNextStep,
    bRunCmd;
    long
    lOsc1_start,
    lOsc2_start,
    lOsc3_start,
    lOsc4_start;
    RGBConverter c;
};



#endif // STROBE_H
