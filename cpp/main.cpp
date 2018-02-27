/*
 *  player.cpp
 *  rtaudiotest
 *
 *  Created by Chris on 23/08/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "main.h"
#include "Maximilian/maximilian.h"
#include "drumSynth.h"
#include "drumMachineBrain.h"
#include <iostream>
// #include <unistd.h>
// #include <fstream>
// #include <termios.h>


#ifdef MAXIMILIAN_PORTAUDIO
#include "Maximilian/portaudio.h"
//#include "pa_mac_core.h"
#elif defined(MAXIMILIAN_RT_AUDIO)
	#if defined( __WIN32__ ) || defined( _WIN32 )
		#include <dsound.h>
	#endif
#include "Maximilian/RtAudio.h"
#endif


void setup();//use this to do any initialisation if you want.

void play(double *output);//run dac! Very very often. Too often in fact. er...

#ifdef MAXIMILIAN_PORTAUDIO
int routing(const void *inputBuffer,
		void *outputBuffer,
		unsigned long nBufferFrames,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags status,
		void *userData ){
#elif defined(MAXIMILIAN_RT_AUDIO)
int routing	(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
			 double streamTime, RtAudioStreamStatus status, void *userData ) {
#endif

	unsigned int i, j;

#ifdef MAXIMILIAN_PORTAUDIO
	float *buffer = (float *) outputBuffer;
#elif defined(MAXIMILIAN_RT_AUDIO)
	double *buffer = (double *) outputBuffer;
#endif
	double *lastValues = (double *) userData;
	//	double currentTime = (double) streamTime; Might come in handy for control
	if ( status )
		std::cout << "Stream underflow detected!" << std::endl;
	for ( i=0; i<nBufferFrames; i++ ) {
	}
	// Write interleaved audio data.
	for ( i=0; i<nBufferFrames; i++ ) {
		play(lastValues);
		for ( j=0; j<maxiSettings::channels; j++ ) {
			*buffer++=lastValues[j];
		}
	}
	return 0;
}

  int playHead;

  maxiClock benKlock;
  float freqs[16]  = {400,160,0,80,0,160,120,200,180,80,40,80,40,160,40,80};
  drumSynth test, test2, test3, test4;

drumMachineBrain tim;
maxiOsc mySine;//One oscillator - can be called anything. Can be any of the available waveforms.
void setup() {//some inits
   playHead = 0;
   benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
    //freqs = {10,160,40,80,160};
		test2 = drumSynth(260, 3.66, 0.5, 0.75, 0.075, 0.815, 0, 750, 1, 0, 16,  0, 100);
		int chan2Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
		test2.frequency = 160;
	memcpy(test2.sequence, chan2Seq, sizeof(chan2Seq));

	test3 = drumSynth(260, 3.66, 0.5, 0.75, 0., 0.815, 0, 750, 1, 0, 16,  0, 30);
	int chan3Seq[]= { 0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1};
	test3.frequency = 0;
memcpy(test3.sequence, chan3Seq, sizeof(chan3Seq));

int chan4Seq[]= { 0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1};
memcpy(test4.sequence, chan4Seq, sizeof(chan4Seq));
}

void play(double *output) {
  benKlock.ticker();

        if (benKlock.tick) {
            playHead++;
            playHead%=16;
						test4.frequency = freqs[playHead];
          tim.trigger(playHead);
        }

   output[0]=tim.play();
   output[1]=output[0];

}

//This is main()
int main()
{
	setup();
// 	int fd;                             // File descriptor
// 	// Open port
// 	fd = open("/dev/cu.usbmodem1421", O_RDWR | O_NOCTTY | O_NDELAY);
// 	if (fd == -1){
// 	    printf("Device cannot be opened.\n");
// 	    exit(-1);                       // If the device is not open, return -1
// 	}
// 	cout << "fd = " << fd << endl;
// 	struct termios options;
//
// fcntl(fd, F_SETFL, FNDELAY);                    // Open the device in nonblocking mode
//
// // Set parameters
// tcgetattr(fd, &options);                        // Get the current options of the port
// bzero(&options, sizeof(options));               // Clear all the options
// speed_t         Speed;
// switch (baudRate)                               // Set the speed (baudRate)
// {
//     case 110  :     Speed=B110; break;
//     case 300  :     Speed=B300; break;
//     case 600  :     Speed=B600; break;
//     case 1200 :     Speed=B1200; break;
//     case 2400 :     Speed=B2400; break;
//     case 4800 :     Speed=B4800; break;
//     case 9600 :     Speed=B9600; break;
//     case 19200 :    Speed=B19200; break;
//     case 38400 :    Speed=B38400; break;
//     case 57600 :    Speed=B57600; break;
//     case 115200 :   Speed=B115200; break;
//     default : exit(-4);
// }
// cfsetispeed(&options, Speed);                   // Set the baud rate at 115200 bauds
// cfsetospeed(&options, Speed);
// options.c_cflag |= ( CLOCAL | CREAD |  CS8);    // Configure the device : 8 bits, no parity, no control
// options.c_iflag |= ( IGNPAR | IGNBRK );
// options.c_cc[VTIME]=0;                          // Timer unused
// options.c_cc[VMIN]=0;                           // At least on character before satisfy reading
// tcsetattr(fd, TCSANOW, &options);

#ifdef MAXIMILIAN_PORTAUDIO
	PaStream *stream;
	PaError err;
	err = Pa_Initialize();
	if( err != paNoError )
		std::cout <<   "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;

	double data[maxiSettings::channels];

	err = Pa_OpenDefaultStream( &stream,
							   0,          /* no input channels */
							   maxiSettings::channels,          /* stereo output */
							   paFloat32,  /* 64 bit floating point output */
							   maxiSettings::sampleRate,
							   maxiSettings::bufferSize,        /* frames per buffer, i.e. the number
												   of sample frames that PortAudio will
												   request from the callback. Many apps
												   may want to use
												   paFramesPerBufferUnspecified, which
												   tells PortAudio to pick the best,
												   possibly changing, buffer size.*/
							   &routing, /* this is your callback function */
							   &data ); /*This is a pointer that will be passed to
										 your callback*/

	//PaAlsa_EnableRealtimeScheduling(stream,true);

	err = Pa_StartStream( stream );
	if( err != paNoError )
		std::cout <<   "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;


	char input;
	std::cout << "\nMaximilian is playing ... press <enter> to quit.\n";
	std::cin.get( input );



	err = Pa_Terminate();
	if( err != paNoError )
		std::cout <<  "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;

#elif defined(MAXIMILIAN_RT_AUDIO)
	RtAudio dac(RtAudio::WINDOWS_DS);
	std::cout << dac.getDeviceCount();
	if ( dac.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found! rt\n";
		char input;
		std::cin.get( input );
		exit( 0 );
	}



	RtAudio::StreamParameters parameters;
	//parameters.deviceId = 2;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = maxiSettings::channels;
	parameters.firstChannel = 0;
	unsigned int sampleRate = maxiSettings::sampleRate;
	unsigned int bufferFrames = maxiSettings::bufferSize;
	//double data[maxiSettings::channels];
	vector<double> data(maxiSettings::channels,0);

	try {
		dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
					   sampleRate, &bufferFrames, &routing, (void *)&(data[0]));

		dac.startStream();
	}
	catch ( RtError& e ) {
		e.printMessage();
		exit( 0 );
	}

	char input;
	std::cout << "\nMaximilian is playing ... press <enter> to quit.\n";
	std::cin.get( input );

	try {
		// Stop the stream
		dac.stopStream();
	}
	catch (RtError& e) {
		e.printMessage();
	}

	if ( dac.isStreamOpen() ) dac.closeStream();
#endif

	return 0;
}
