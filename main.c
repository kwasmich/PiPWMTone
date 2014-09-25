//
//  main.c
//  PiPWMTone
//
//  Created by Michael Kwasnicki on 27.08.14.
//  Copyright (c) 2014 Kwasi-ich. All rights reserved.
//


#include <wiringPi.h>

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


const char* TONE_NAME[] = {
    "C-",
    "C#",
    "D-",
    "D#",
    "E-",
    "F-",
    "F#",
    "G-",
    "G#",
    "A-",
    "A#",
    "B-",
    "--",
};

const float TONE_HZ[] = {
    4186.01f, // C-8
    4434.92f, // C#8
    4698.63f, // D-8
    4978.03f, // D#8
    5274.04f, // E-8
    5587.65f, // F-8
    5919.91f, // F#8
    6271.93f, // G-8
    6644.88f, // G#8
    7040.00f, // A-8
    7458.62f, // A#8
    7902.13f, // B-8 (H8)
    0         // --
};


static void destroy() {
    pinMode( 18, INPUT );
}


static void terminated( const int in_SIG ) {
	fprintf( stderr, "\nTERMINATING due to signal %i\n", in_SIG );
	exit( -1 );
}


int main( int argc, const char * argv[] ) {
    unsigned int timeBase = 3000000;
    char buffer[10];
    char prevLine[10];
    
    if ( geteuid() != 0 ) {
        // chown root <file>
        // sudo chmod u+s <file>
        char* exec = rindex( argv[0], '/' );
        exec++;
        fprintf( stderr, "You must be root to run \"%s\". Program should be suid root. This is an error.\n", exec ) ;
        return 1;
	}
    
	atexit( destroy );
	signal( SIGHUP, terminated );
	signal( SIGINT, terminated );
	signal( SIGKILL, terminated );
	signal( SIGPIPE, terminated );
	signal( SIGALRM, terminated );
	signal( SIGTERM, terminated );
    
    wiringPiSetupGpio();
    pinMode( 18, PWM_OUTPUT );
	pwmSetMode( PWM_MODE_MS );
    pwmWrite( 18, 0 );
    
    char tone[3] = "--";
    int octave;
    unsigned int duration;
    
    char* line = fgets( buffer, 10, stdin );
    assert( line != NULL );
    
    int parsed = sscanf( line, "%c%c%i %i", &tone[0], &tone[1], &octave, &duration );
    
    while ( parsed == 4 ) {
        printf( "%s%i, 1/%i\n", tone, octave, duration );
        fflush( stdout );
        int divisor = 1 << (8 - octave - 2);
        int toneIndex = 0;
        
        for ( toneIndex = 0; toneIndex < 13; toneIndex++ ) {
            if ( 0 == strncmp( tone, TONE_NAME[toneIndex], 2 ) ) {
                break;
            }
        }
        
        assert( toneIndex < 13 );
        
        if ( toneIndex < 12 ) {
            float frequency = TONE_HZ[toneIndex] / divisor;
            unsigned int range = (unsigned int)( 600000.0f / frequency );
            pwmSetRange( range );
            pwmWrite( 18, range >> 1 );
            usleep( timeBase / duration );
            pwmWrite( 18, 0 );
            usleep( 20000 );
        } else {
            pwmWrite( 18, 0 );
            usleep( timeBase / duration + 20000 );
        }
        
        strncpy( prevLine, line, 10 );
        line = fgets( buffer, 10, stdin );
        parsed = 0;
        
        if ( line ) {
            parsed = sscanf( line, "%c%c%i %i", &tone[0], &tone[1], &octave, &duration );
        }
    }
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

