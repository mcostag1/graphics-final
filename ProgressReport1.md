#Our current progress:#

We have done significant amount of additional reading on the process of audio visualization - we had a vague idea before but definitely have a firmer grasp on the process. We've outlined them below:

Processing Audio Component

1. Process MP3 data using RTAudio to extract important music features.

2. Convert audio information into a format that the Fast Fourier Transform algorithm (FFT) can take in. This will likely mean converting ints -> floats and splitting left and right audio channels (cite stack overflow post)

3. Call FFT algorithm to map audio info from the time domain to the frequency domain - collect information about the frequencies present in the audio file.

4. Use this data to manipulate our graphical visualization. 

Visualization Component

Create a histogram graph-like visualization where each bar in the histogram corresponds to a specific range of frequencies. The bars height will change depending on the audio of the song. 

1. Create a bar struct (trying struct now, might have to change to class) which holds basic info like the size, color, and displacement of the bar. 

2. Use this bar to create multiple other ones to visualize on the window - change the veriables within the struct to build different bars based on the first one (i.e. Change horizontal displacement to change where the bar is located on the screen).

3. Edit the vshader and mypanelopengl.cpp file to animate the bars in some way. We will begin by adjusting the height based on phases of a sin curve, but will eventually integrate the audio processing frequency Component so that the height of the bar is dependent on that instead. We can also change the color as well based on some other audio characteristic.


So far we have built a bar struct and have animated the bar object’s height based on a sinusoidal wave. We have created multiple bar objects, each with a specific offset so that all together they make a histogram type chart. We are in the process of debugging why these multiple bars are not displaying properly on our window, but will hopefully resolve that issue soon.

#Next steps: #

We have also looked into additional third party libraries we may need - especially for the FFT algorithm. We did some research on KissFFT, and think it may work as it is a pretty simple straight forward implementation (They said ‘Kiss’ represents the phrase ‘keep it simple stupid’…).

Moving forward, we want to continue building our visualization before we go too far deep into the audio processing realm. Although we may need to install KissFFT soon to make sure it works, we want to focus most of our energy at least right now at building a visualization that moves to some sort of input, as simple as a sinusoidal wave, so that it will be easier to debug if we have issues visualizing it based off of the audio input. 
