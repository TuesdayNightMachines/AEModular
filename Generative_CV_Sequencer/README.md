**GENERATIVE CV SEQUENCER**

AE Modular Synth Module using an Arduino Pro Micro (5V)

More info: http://forum.aemodular.com/thread/362/generative-ambient-cv-sequencer

License: CC-BY-NC-SA


**HOW IT WORKS:**

There are three separate sequencer lanes, listening to specific events upon which the module's CV outputs are modified. Each lane has a bipolar potentiometer to set a fixed voltage which is added or subtracted from the CV output when an event occurs. 

- L1: When a rising gate edge is detected on the L1 Input, a fixed voltage is added or subtracted to/from the L1 CV output, according to the bipolar potentiometer.  

- L2: When a falling gate edge is detected on the L2 Input, a fixed voltage is added or subtracted to/from the L2 CV output.

- L3: When the CV at the L3 CV Input rises above or falls below a set threshold, a fixed voltage is added or subtracted to/from the L3 CV output. The threshold can be set via a dedicated threshold potentiometer and there is a manual switch to set wether the input CV has to rise above or fall below the threshold.

- Apart from the L1, L2 and L3 CV outputs, there is also a SUM output of the three.

- The Offset potentiometer and CV input add a voltage to all CV outputs.

- The Wrap/Limit switch sets wether the output CV voltages are constrainted from 0V to 5V or if they "roll over" or "wrap around" when they would rise above 5V or fall below 0V. The Wrap/Limit gate input inverts the manual switch setting on a high gate.

- The "Invert L2 & L3 Pot Value" gate input inverts the potentiometer value setting the added or subtracted voltage when an event occurs.

- The "Reset L1 & L2 Value" gate input sets the L1 and L2 CV outputs back to 0V on a rising edge. 


**POSSIBLE IMPROVEMENTS OR CHANGES YOU MIGHT TRY OUT:**

- Scale the CV outputs to the Op-Amps' rail-to-rail range (i.e. 0V to a little bit below 5V) either in software or using a voltage divider.

- Try out lower resistor values, even down to 270 Ohm, for the RC Filter circuits on the CV outputs (right before the Op-Amps) for less slew/portamento and maybe higher output voltages.

- Use Arduino interrupts to handle the gate and trigger input signals more quickly (since this is supposed to be a rather slow CV sequencer I didn't bother to do that).

- Experiment with the input functionality, e.g. you could use the "Invert L2+L3 Pot" gate input to invert the L1 as well, or make it do something completely different.
