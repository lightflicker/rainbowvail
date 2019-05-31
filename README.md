<h1>RAINBOW VAIL</h1>
<p>Device for deep relaxation and spiritual advancement</p>
</ul>
<h1>Rainbow Vail</h1>
<p><bold>For deep relaxation and spiritual advancement</bold></p>
<img src="Docs/Pictures/logo.png" alt="Goggles" width="450"> 
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
<h2>Reinstating Python environment</h2>
In order for the Sequencer to function properly the Python envronment needs to be reinstated.
Create a directory <i>RainbowVail</i> in a desired place. Within that direcory create directory <i>repo</i>. Clone git repositury into that directory:
<code>>>cd repo</code>
<code>>>git clone https://github.com/lightflicker/rainbowvail</code>
Copy the requrements.txt from <i>repo/Sequencer/requirements.txt</i> to <i>RainbowVail</i>.
Further find out the path to the <i>python3</i>:
<code>>>which python3</code>
<code>/usr/bin/python3</code>
Create Python virtual environment:
<code>>>virtualenv -p /usr/bin/python3 .</code>
Enter virtual environment:
<code>>>source bin/activate</code>
Reinstate all dependencies:
<code>>>pip install -r requirements.txt</code>
</br>
<h2>Simple Sequencer usage</h2>
<p>Once the goggles are connected to the USB port, the sequencer can be invoked with
  the following commands:</p>
<code>>cd Sequencer</code></br>
<code>>source bin/activate</code></br>
<code>>python RainbowVail.py Sequences/RainbowVail_Seq005_20161014.csv</code></br>
