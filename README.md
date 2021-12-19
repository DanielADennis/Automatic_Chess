# Automatic_Chess

ECE 3710 - Fall 2021

Daniel Dennis 

Dallin Schiffman 


# Objectives 
The main goal of this project was to make a chess board that moves the pieces for you. Features that we wanted to include are a menu to select different modes. One mode would be a “Manal Game” where you and an opponent take turns inputting desired moves. The other mode, “Auto Game” plays back a preset game from a tournament in history.

# Supplies 
For this project, we tried to use as many parts as possible that we already had. The parts from the Lab kit include:
2 “28BYJ-48” Stepper Motors
1 16 digit keypad
1 LCD 
1 Nucleo STM 32-L476RG Board
Additional parts required include:
Timing belt
Plastic Filament for 3D Printing
4 hinge roller micro switches
2 Linear Ball Bearings
2 Linear Motion Rod Shafts
32 round 6x2 mm neodymium magnets
5v power supply
5v electromagnet with driver board.
Alternative: 1 linear servo


Preparation
Before actually building this project, there were quite a few things that we needed to design.
 First of all, we needed to figure out how we wanted to keep track of all the pieces on the board. We eventually decided that trying to keep track of things like where each piece is, or whether a move is legal or not would make this project unnecessarily complicated. We decided to simply check if a square is occupied. If you try to move a piece to an occupied square, it will first kill the piece to clear the square by moving it off to the side, then move the new piece to the desired coordinate. Our design includes an 8x8 array that holds a 1 or a 0 to indicate its vacancy. See Figure 1 for our UML diagram.
We also drew a system schematic to show where we needed to make connections to each hardware component. See Figure 2 for the Schematic.
We designed the actual chess board and pieces as well. We designed each square to be 4.5cm in length and width so that, including the buffer, the board would be 45 x 45 cm. In order to allow the pieces to slip past one another, the bottom diameter cannot exceed half the width of the square. All the pieces are designed to be 3D printed. We plan to use wood for the outside border of the board, and a thin acrylic sheet for the playing area. The enclosure for the keypad, LCD, and Nucleo board will be 3D printed. We also designed the linear rail mounts and track to be 3D printed as well.
Procedure 
We took our design from the UML and started programming the microcontroller. We split up the functionality into 4 different files with their respective header files. One for the Motors, Keypad, LCD, and Clock. Inside of our main, we set up the virtual chess board to keep track of space vacancy. Using a state machine, we have a menu as well. The options are to “1.) Manual Game”, or “2.) Auto Game”. We await input from the keypad. Once a key is pressed, we check which option was selected and move to that respective state in the state machine. If “Manual Game” is selected, we go to a state that awaits a starting and ending coordinate. Once the coordinates are input, the magnet will move into position, enable, then move the pieces to the desired location. The LCD displays what was input and displays what state we’re currently in.
As for the hardware, we cut out the wood side panels and screwed them together. We mounted the side walls to a flat baseboard. We cut holes to mount the metal rails, the Nucleo board enclosure box, and the timing belt. We printed off a chess board with 4.5 mm squares and taped that to the acrylic which slides into the wooden side panels. We mounted the 3d printed cart on the metal rails using the linear bearings and attached the electromagnet and interrupt switches. Following the Schematic in Figure 2, we wired all the components to the Nucleo Board. Since the Stepper Motors and Electromagnet required more current, we used an external power supply. As per the documentation, in order to use an external power supply, we moved the jumper on JP5 over towards E5V and disconnected the jumper from JP1. With everything assembled, it was time for some tests. 
Test 1: Keypad, LCD, and electromagnet enable
We tested to see if the input from the keypad would output onto the LCD. Our testing was a success. We were able to copy the input onto the screen. The electromagnet also turned on and off through calling functions to enable and disable the magnet respectively. That test also passed
Test 2:  Vertical and Horizontal Movement
We tested the vertical movement by just calling the vertical movement function and observed as it moved any number of spaces that we passed into the function. This also worked with the horizontal movement.
Test 3: Input movement (From->To)
We next ensured that after inputting two coordinates  it would be able to move a piece from the first coordinate to the second coordinate. In the process we especially did not want the magnet to drag pieces on its way to the first coordinate. So we had the controller disable the magnet, move the magnet to the first coordinate, turn on the magnet, and finally move the piece to the second coordinate. It passed this test as well. 









Figures  

Figure 1: UML of the program



Figure 2: System Schematic


Results

As an alternative, we got some linear servos that could physically raise up a neodymium magnet to attract the pieces because we were pretty worried that the electromagnet would not be strong enough to move the piece across the acrylic. First off, we found that the bare PLA plastic pieces would not slide very well. We solved this by adding felt to the bottom of each piece. We also noticed that the acrylic itself was fairly tacky, so we decided to leave the protective plastic covering on to help keep down the friction. Lastly,  after quite a bit of testing, we found that the electromagnet couldn’t draw enough current to be able to move a piece. We finally solved that problem by using an external power source to power the microcontroller and peripherals. That also meant that we needed to change the configuration of some jumpers to allow the board to be powered externally. Since the electromagnet seemed to be doing just fine, we never implemented the servo design. 

The new external power supply also helped with an issue in our stepper motors. Originally, we clocked our stepper motors moving the electromagnet at .2cm/second. That made moving each piece unbearably slow. The movement of the pieces was almost imperceptibly slow. After switching to the new power source, we were able to crank the speed up to a slow, yet reasonable speed of .833 cm/second. 

Another issue we found was with wire management. We wanted the design to be as thin as possible, which led to some tight spaces. Tried taping the wires in strategic locations to help guide the wires into the channels that we wanted but they seemed to have a mind of their own. They’d get bunched up, pinch, and cause the motors to jam. We tried to resolve that issue by designing a fan like cover plate that would help the wires pivot at the center areas where we would have pinching. That seemed to help the problem a lot, however, we’d occasionally still have some pinching. In the end, we added an extra centimeter of thickness to the bottom to allow the wires more flexibility. After that modification, we never had any more issues with the wires.

The last major issue came right as we were about to film the video. This entire time of testing and building this project, we have never had any issues with our keypad. Basically, once we wired it up, it was good to go. After running through the tests as a preliminary run, we added a small change to the code to display a phrase on the LCD (not at all related to the keypad). Once we loaded that new code onto the microcontroller we could not get the keypad to work! We quickly deleted whatever changes we had made and tried to run the code again. Strangely, our previous code was not working. We ended up taking apart the housing for the nucleo board, LCD, and Keypad to take a look. After repeatedly checking every single line going into the nucleo board and testing voltages and continuity on each one, we still could not get the keypad to work. We even disconnected the keypad and connected a brand new one to see if that was the issue. At that point, we just had to cut our losses because a critical part of the chess board was not working. The only thing we could record was the Auto game playback because that doesn’t require any keypad input from the user. After recording the Automatic game, we tried the keypad just in case. Somehow it had miraculously fixed itself and we were able to record the manual inputs as well.
	

Conclusion
In the end we were able to get everything working the way we originally envisioned. We were able to meet all of our proposed goals for our project. Though it is a little slow, it is a lot of fun watching it move pieces for you. If we had more time, we would probably use it to replace the stepper motors to some more powerful ones. That way we can have it run at two speeds. One very quick speed to move to the next piece it's going to move, and a slower speed when moving the pieces. It was very cool to be able to envision something based off of what we have been learning all semester, and see it become a reality.
