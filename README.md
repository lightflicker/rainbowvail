# RAINBOW VAIL
Device for deep relaxation and spiritual advancement
----
## Rainbow Vail
<p><bold>For deep relaxation and spiritual advancement</bold></p>
![Goggles][Docs/Pictures/logo.png] 

## Set overview
.. The RainbowVail is made up of two parts. The first part is the goggle set, which contains the actual lights and the Arduino Nano board. In the prototype the Nano board is attached to the goggles along with the set of two Adafruit addressable LEDs. The Nano board contains a definition of the basic light patterns, the building blocks of the light sequence. These light patterns are activated in a configurable sequence by the sequencer run from Raspberry Pi. The configuration of the sequence would determine parameters such as flicker frequency or level.

## Reinstating Python environment
In order for the Sequencer to function properly the Python envronment needs to be reinstated.
Create a directory **RainbowVail** in a desired place. Within that direcory create directory **repo**. Clone git repository into that directory:
```shell
cd repo
git clone https://github.com/lightflicker/rainbowvail
```
Copy the **requrements.txt** from **repo/Sequencer** to **RainbowVail**.
Further find out the path to the <i>python3</i>:</br>
```shell
which python3
/usr/bin/python3
```
Create Python virtual environment:
```shell
virtualenv -p /usr/bin/python3 .
```
Enter virtual environment:
```shell
source bin/activate
```
Reinstate all dependencies:
```shell
pip install -r requirements.txt
```

## Simple Sequencer usage
Once the goggles are connected to the USB port, the sequencer can be invoked with the following commands:
```shell
cd Sequencer
python RainbowVail.py Sequences/RainbowVail_Seq005_20161014.csv
```
