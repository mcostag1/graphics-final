# Final Project: Real-Time Audio Visualizer
## Project Overview
For our final project, we built a real-time reactive audio visualizer. We were interested in using the basic graphic concepts we learned throughout the course and combining them with another discipline, which is how we settled on an audio visualizer. We were particularly inspired by certain audiovisualizers we found online, that visualized the changing frequency data of the song.   

The basic flow of our program works as follows:

1. Read in and play an audio music file.
2. Process the audio stream by reading in small samples of data (number of frames per sample and the sampling rate changes depending on the song).
3. Find the maximum values of both the left and right channel and then convert these values to fit a range between 0 and 1.
4. Convert to proper data type format and pass these maximum values into function specified in KissFFT - which then outputs frequency values.
5. Use these values to alter the bar heights in our implementation. 

The bars in our visualization act like bins for the frequency values. When a song hits a specific frequency, that bars height is increased by the amplitude of that frequency. The number of bars in our implementation is modifiable. We tested the number of bars between 1 and 240, and found the range of frequency changes depending on the number of bars available. 
## Methods + Tools
We built our program using C++, OpenGL, Qt Audio APIs (including QtMediaPlayer and QtAudioProbe), and KissFFT - a simple implementation/API of the fast Fourier Transform. The tools we did not use throughout the course and their specific purpose to our project are explained in more detail below. 

### Qt
With help from Professor Danner, we were able to successfully playback and MP3 media file and process audio from that file in real time using the Qt Mediaplayer and Qt Audio probe. As of now, it takes in only MP3 files, but moving forward we would extend the capabilities for it to read different audio formats. We use Qt AudioProbe to read in samples of the audio into a buffer, and then we pass that data into KissFFT which performs the FFT. 

### KissFFT
We included a folder in our project called "kissfft-130", but only included two headerfiles within our program. KissFFT is named accordingly because it has built a Fast Fourier Transform implementation based up on the principle, "Keep It Simple, Stupid." It is very small in terms of file size (nothing needed to be installed), and works reasonably efficiently. Before implementing the frequency-effected visualization, we were simply using the raw amplitude data collected from the Qt AudioProbe and visualizing that information. Using KissFFT did not affect the run-time or real-time implementation of our program. Additionally, it is a mixed radix FFT library that can use either fixed or floating point data types.

## How to run our program
Yes, external files are necessary. We have a set of 7 MP3 files that are included in our implementation. We will send them to you via email in a zip file. If you place the MP3 files within the build directory, the program should run successfully. 

Once you are in the build directory, you need only run this command and the program should run:

make -j8 && ./final

## Primary Features

Our main feature is being able to watch the program run in real time, as opposed to many other audio visualizers that need to be rendered before watching them. With our program the user can partially alter the look of the visualizer as well. To change the number of bars, you can just alter these values in mypanelopenGL.h:

    int numBars = 40;
		bar bars[40];

The program can take in any number but we suggest only using 1-240. Beyond 240, the bars get extremely narrow. You can also change the spacing of the visualizer by altering this value in mypanelopenGL.h:

	float spacing = .4/numBars;
  
The visualizer can look very cool with an altered spacing value. We also have a menu where the a user can select which song they wish to listen to and the visualizer will adapt to it. We currently have 7 audio files available, but we could easily add as many as we wanted. This also showcases the real time aspect of our project since the program must visualize whichever option the user chooses. 

## Setbacks + Future Work
### Setbacks
As we were not experienced in audio-processing at all, we found that aspect of our project challenging, and it henceforth set us back a bit in terms of time. Additionally, we did a fair amount of research in understanding common implementations of audio-visualizers today, which involved a lot of research in third party softwares/APIs and the fast Fourier Transform. Before we settled on Qt for our audio processing, we attempted to use RtAudio, which seemed straightforward enough, but lacked extensive documentation or tutorials. The only ones we were able to access were tutorials that used an older version of RtAudio, which was not helpful. 


### Future Work
If we had additional time, we first want to add a smoothing function to the changing heights of our vertical bars. Right now, the bars heights change instantly, which causes a somewhat spastic visual display, especially when the song being played has an intense range of volume and frequency. We would additionally want to work on different visualizations. Now that we have figured out and understood the audio-processing behavior of our program, we could create additional visualizations with the additional graphics projects we have built. For example, we could take our planets from Lab 4 + 5 and make the radii of the planets change depending on the frequency. 

