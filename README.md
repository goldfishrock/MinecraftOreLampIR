# MinecraftOreLampIR
Remixed code for  Danjhamer's amazing Minecraft Ore Lamp (https://www.thingiverse.com/thing:3401758)

I'd been meaning to update this for a while now and while I was printing a new lamp for someone, I thought I'd give the old code a total overhaul and update the hardware to make it more user friendly. Out went the little tactile button
and in comes an IR receiver. After bending the legs of the receiver 90°, it fitted (upside down) perfectly in the opening that had been made for the tactile switch. Feeding the receiver +5v power from the Arduino Nano, I connected the 
data pin to PIN2 on the board.

As for the IR Remote, I chose a simple & cheap chinese type freely available on the internet (see photo). The code reflects the coding of this remote which simply sends a number starting at zero and finishing at 23. The four 'feature'
buttons have been programmed to perform simple 'animations', simple flashing, random leds, pulsing & a nice rainbow effect. The rest of the keys on the remote work as advertised.

**Arduino Pin**
- 2		->		IR Receiver Data Pin
- 5		->		DIN (Data In on LED strip)
- GND		->	GND
- 5V	->		5V 

**LED Strip**
- +5v		->		5V Pin (Arduino)
- DIN		->		Pin 5 (Arduino)
- GND		-> 		Ground (Arduino)

**IR Receiver**
- Pin 1		->		Pin 2 (Arduino)
- Pin 2		->		Ground  (Arduino)
- Pin 3		->		5v (Arduino)



