<h1>Rainbow Vail</h1>
<p>Deep relaxation and spiritual advancement</p>
<img src="Docs/Pictures/logo.png" alt="Goggles"> 
</br>
<h2>Set overview</h2>
<p>The RainbowVail is made up of two parts. The first part is the goggle set,
  which contains the actual lights and the Arduino Nano board. In the prototype
  the Nano board is attached to the goggles along with the set of two Adafruit
  addressable LEDs. The Nano board contains a definition of the basic light patterns,
  the building blocks of the light sequence. These light patterns are activated in
  a configurable sequence by the sequencer run from Raspberry Pi.
  The configuration of the sequence would determine parameters such as flicker
  frequency or level.</p>
<h2>Simple Sequencer usage</h2>
<p>Once the goggles are connected to the USB port, the sequencer can be invoked with
  the following commands:</p>
<code>>cd Sequencer</code></br>
<code>>source bin/activate</code></br>
<code>>python RainbowVail.py Sequences/RainbowVail_Seq005_20161014.csv</code></br>
  
