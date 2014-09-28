PiPWMTone
=========

Using Raspberry Pi's PWM output to generate tones and melodies.

When launched, it reads tone information form stdin in a proprietary text format.
There are two sample files that can be used for a quick test.


Dependencies
------------

This command line tool depends on

``WiringPI``


Connecting a Speaker
--------------------

You need to connect your speaker to

```
pin 12 (GPIO 18 - PCM_CLK)
pin 14 (Ground)

           SPEAKER
             | |
 2  4  6  8 10 12 14 16 18 20 22 24 26
 1  3  5  7  9 11 13 15 17 19 21 23 25
```


Usage
=====

``PiPWMTone < NothingElseMatters.txt``

In interactive mode the tone is played after each new line.
