# Decimal Clock

This is the documentation to build a simple decimal clock with one clock-hand that displays decimal time. 

![decimal_clock_image](https://github.com/FloEll/DecimalClock/blob/main/Decimal_Clock_images/Decimal_Clock_image.png)

# Background 

The Earth needs exactly 1 day to rotate on its own axis. How we divide this day into a finer resolution is up to us. We take it for a certainty that the day has 24 hours and that a mechanical clock needs two revolutions to display this accordingly. This hour on the other hand is then split into 60 minutes and every minute is further divided into 60 seconds (I'll add bit more on this and why the Egyptians used some fancy counting bases such as 12 and 60 soon to explain how we ended up here...) 

However, time kept ticking on and I decided to get used to decimal time, at least for this small DIY project. Decimal time is pretty fancy, because a day has 10 hours and every hour has 100 minutes (especially if you happen to live in France after 1792). How you further divide the hours is actually up to you, since this clock basically just has an hour clock-hand. I added also percentage values to the clock-face in order to display how much of my day has already passed. I start to regret this decision, since it can also be quite stressfull when you get up in the morning and you realize that 30% of your day has already passed...

# The Decimal Clock build

I used an old table clock as a case for this project, since there is plenty of space inside. 

I used an ESP32 Dev Kit as the microcontroller since it already comes with a USB power supply and a WiFi chip. I attached a stepper motor with a driver board and a light switch to calibrate the clock-hand. I'll add a parts list soon. Everything was wired as shown here:

![decimal_clock_wiring](https://github.com/FloEll/DecimalClock/blob/main/Decimal_Clock_images/Decimal_Clock_Breadboard.png)

Just solder everything together an upload the script provided in the Decimal_Clock_v001 folder e.g. using the Arduino IDE. 

# License

Copyright (C) 2020-2021 Florian Ellsäßer. Released under GNU GPL License v3.
