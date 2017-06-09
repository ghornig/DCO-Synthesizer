#include <avr/sfr_defs.h>
#define NOTE0 2
#define NOTE1 3
#define NOTE2 4
#define NOTE3 7
#define NOTE4 6
#define NOTE5 5

#define C1_COUNT0 0xb00000
#define C1_COUNT1 0xb00001
#define C1_COUNT2 0xb00010
#define C1_WORD 0xb00011
#define C2_COUNT0 0xb00100
#define C2_COUNT1 0xb00101
#define C2_COUNT2 0xb00110
#define C2_WORD 0xb00111
#define KEYSR 0xb01000
#define KEYSRRST 0xb01001
#define CV0 0xb01010
#define CV1 0xb01011
#define CV2 0xb01100
#define CV3 0xb01101
#define CV4 0xb01110
#define CV5 0xb01111
#define GATES 0xb10000

/* Relevant Parts List:

ATMEGA8-P
Intersil 82C54 (Digikey#: CP82C54Z-ND) - 3 in 1 counters
8 MHz synthesizer clock (seperate from microprocessor crystal) - (Digikey#: 535-9184-5-ND)
8 Bit Transparent D Latch - (Digikey#: SN74HC373N)
All of the basic digital stuff (shift registers, logic, memory) are just the texas instruments SN74XXXXX series. You can look these up. They're pretty standard. Same stuff from ECE 210.
TI DAC0800


/* Address table. Empty addresses may be used for future devices (there are some functions that could be added later that I didn't discuss but if added must be handled by this 
processor ie tuning etc.)

Address					Device/Function
A4	A3  A2  A1  A0
0	0	0	0	0		C1_Count0 (Osc. 0)
0	0	0	0	1		C1_Count1 (Osc. 1)
0	0	0	1	0		C1_Count2 (Osc. 2)
0	0	0	1	1		C1_Word
0	0	1	0	0		C2_Count0 (Osc. 3)
0	0	1	0	1		C2_Count1 (Osc. 4)
0	0	1	1	0		C2_Count2 (Osc. 5)
0	0	1	1	1		C2_Word
0	1	0	0	0		Shift Register
0	1	0	0	1		Shift Register Reset
0	1	0	1	0		CV0 (Osc. 0)
0	1	0	1	1		CV1 (Osc. 1)
0	1	1	0	0		CV2 (Osc. 2)
0	1	1	0	1		CV3 (Osc. 3)
0	1	1	1	0		CV4 (Osc. 4)
0	1	1	1	1		CV5 (Osc. 5)
1	0	0	0	0		Gates 1-6 (there are actually 8 with two unused.)
1	0	0	0	1	
1	0	0	1	0	
1	0	0	1	1	
1	0	1	0	0	
1	0	1	0	1	
1	0	1	1	0	
1	0	1	1	1	
1	1	0	0	0	
1	1	0	0	1	
1	1	0	1	0	
1	1	0	1	1	
1	1	1	0	0	
1	1	1	0	1	
1	1	1	1	0	
1	1	1	1	1	


*/

// Pre generated table of frequencies corresponding to notes:

/*

Keyboard Note Num	Note	Freq (Hz)
0	C2			65.41	
1	C#2/Db2 	69.3	
2	D2			73.42	
3	D#2/Eb2 	77.78	
4	E2			82.41	
5	F2			87.31	
6	F#2/Gb2 	92.5	
7	G2			98		
8	G#2/Ab2 	103.83	
9	A2			110		
10	A#2/Bb2 	116.54	
11	B2			123.47	
12	C3			130.81	
13	C#3/Db3 	138.59	
14	D3			146.83	
15	D#3/Eb3 	155.56	
16	E3			164.81	
17	F3			174.61	
18	F#3/Gb3 	185		
19	G3			196		
20	G#3/Ab3 	207.65	
21	A3			220		
22	A#3/Bb3 	233.08	
23	B3			246.94	
24	C4			261.63	
25	C#4/Db4 	277.18	
26	D4			293.66	
27	D#4/Eb4 	311.13	
28	E4			329.63	
29	F4			349.23	
30	F#4/Gb4 	369.99	
31	G4			392		
32	G#4/Ab4 	415.3	
33	A4			440		
34	A#4/Bb4 	466.16	
35	B4			493.88	
36	C5			523.25	
37	C#5/Db5 	554.37	
38	D5			587.33	
39	D#5/Eb5 	622.25	
40	E5			659.25	
41	F5			698.46	
42	F#5/Gb5 	739.99	
43	G5			783.99	
44	G#5/Ab5 	830.61	
45	A5			880		
46	A#5/Bb5 	932.33	
47	B5			987.77	
48	C6			1046.5	
49	C#6/Db6 	1108.73	
50	D6			1174.66	
51	D#6/Eb6 	1244.51	
52	E6			1318.51	
53	F6			1396.91	
54	F#6/Gb6 	1479.98	
55	G6			1567.98	
56	G#6/Ab6 	1661.22	
57	A6			1760	
58	A#6/Bb6 	1864.66	
59	B6			1975.53	
60	C7			2093	

*/

With a 4 MHz master clock

static int notefreqs[] = {
61152,
57720,
54481,
51427,
48537,
45813,
43243,
40816,
38524,
36363,
34322,
32396,
30578,
28862,
27242,
25713,
24270,
22908,
21621,
20408,
19263,
18181,
17161,
16198,
15288,
14431,
13621,
12856,
12134,
11453,
10811,
10204,
9631,
9090,
8580,
8099,
7644,
7215,
6810,
6428,
6067,
5726,
5405,
5102,
4815,
4545,
4290,
4049,
3822,
3607,
3405,
3214,
3033,
2863,
2702,
2551,
2407,
2272,
2145,
2024,
1911,
1333,
 };
 
int main(void) {
	
	// Set global interrupts
	sei();
	
	/* Set up pins according to:
	
	PortB 5 - Data bus 0
	PortB 4 - Data bus 1
	PortB 3 - Data bus 2
	PortB 2 - Data bus 3
	PortB 1 - Data bus 4
	PortB 0 - Data bus 5
	PortD 7 - Data bus 6
	PortD 6 - Data bus 7
	
	PortD 5 - Address Bus SR Data
	PortD 4 - Address Bus SR Clock
	PortD 3 - Master "Control" Clock
	
	*/
	
	// Even though it will switch throughout process, set default state of all data lines as output
	
	// Output
	DDRB |= (1<<5);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB5);	
	// Output
	DDRB |= (1<<4);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB4);		
	// Output
	DDRB |= (1<<3);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB3);		
	// Output
	DDRB |= (1<<2);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB2);
	// Output
	DDRB |= (1<<1);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB1);	
	// Output
	DDRB |= (1<<0);
	// Initialize it as 0 (even though it does start as 0)
	PORTB &= ~(1<<PORTB0);	
	// Output
	DDRD |= (1<<7);
	// Initialize it as 0 (even though it does start as 0)
	PORTD &= ~(1<<PORTD7);
	// Output
	DDRD |= (1<<6);
	// Initialize it as 0 (even though it does start as 0)
	PORTD &= ~(1<<PORTD6);	
	
	// Output
	DDRD |= (1<<5);
	// Initialize it as 0 (even though it does start as 0)
	PORTD &= ~(1<<PORTD5);
	// Output
	DDRD |= (1<<4);
	// Initialize it as 0 (even though it does start as 0)
	PORTD &= ~(1<<PORTD4);
	// Output
	DDRD |= (1<<3);
	// Initialize it as 0 (even though it does start as 0)
	PORTD &= ~(1<<PORTD3);	
	
	while(1) {
		
		readKeys();
		setOSC();
		setOSCCV();
		setOSCGate();
		
	}
	
		
		
	
}

int readKeys(void) {
	// This function reads the keys, and sets what keys are pressed to a global variable known as newKeys.
		
	/*
	The keyboard is divided into sets of six consecutive notes (including the black keys,) which I'll call tritones.
	
	Starting from the low end of the keyboard (left hand side,) there are eleven tritones. The last tritone is only a partial tritone and doesn't contain all six notes.
	
	The keyboard interface therefore has a total of 17 pins as follows:
	1 - Tritone 0
	2 - Tritone 1
	3 - Tritone 2
	4 - Tritone 3
	5 - Tritone 4
	6 - Tritone 5
	7 - Note 3 (D#/A)
	8 - Note 4 (E/A#)
	9 - Note 5(F/B#)
	10 - Note 2 (D/G#)
	11 - Note 1 (C#/G)
	12 - Note 0 (C/F#)
	13 - Tritone 6
	14 - Tritone 7
	15 - Tritone 8
	16 - Tritone 9
	17 - Tritone 10
	
	There are actually two musical notes associated with each 'note,' since a tritone is only half of a full octave.
	
	To make the readings, you apply 5V or high to the tritone pin, and read the corresponding note pins. If a note reads high, then that means it is pressed.
	
	The keyboard is actually wired up as follows:
	-> 7 through 12 (notes) - attached to Data bus 7 through 2 respectively.
	-> Starting from pin 17 (tritone 10,) the tritones are wired into two chained shift registers. Only 3 outputs on the second shift register is being used.
	This shift register is located on address 0xb01000 and its reset on 0xb01001. What this means is that you must select the address of the shift register, and then the final
	bits of the data bus, data 0 and data 1, become the shift register data and clock respectively. When reset is selected, all that is needed is a selection and the control clock being set high to reset the shift registers.

	*/
	
	// Clear SR (may be able to be placed at the very beginning and doesn't have to be reset each iteration.)
	setAddress(KEYSRRST);
	setCTRLCLK(1);
	setCTRLCLK(0);
	
	setAddress(KEYSR);
	[make data 7 through 2 inputs, since this should be default state might not be neccesary]
	[make data 1 through 0 outputs.]

	setCTRLCLK(1);
	
	// Index for newKeys
	j = 0;
	
	// Actually go through the keys here:
	for (int i=0;i<10;i++) {
		setTritone(i);
		// Let new keys be a variable containing all of the keys that have been pressed. Lets number all of the keys from the bottom C (0) to the top C (61) (10 tritones x 6 notes + 1 extra C at high end.)
		for (int k=0;k<5;k++) {
			if (readData(NOTE0)) {
				newKeys[j++] = i*6 + k;
			}
		}
	}
	
	setCTRLCLK(0);
	
}

int setOSC(void) {
	// Figures out which of the counters (oscillators) need to be set to what and sets them.
	
	// Probably place this once at the very beginning
	counterInit();
	
	// Quick little potential bug note: The keyboard is read left to right, so if you press more than 6 keys in addition to ones you have, instead of not being heard,
	// they will replace the ones that were previously pressed, priority to the leftmost key. It should be priority in order that keys were pressed.
	
	tosetKeys = currentKeys;
	
	// Compare the currently pressed keys to the newKeys to find keys that have to go
	for (j=0;j<5;i++) 
	{
		for (i=0;i<5;j++)
		{
			if (newKeys[i] == currentKeys[j])
			{
				break;
			}
		}
		// If you don't break (the old key is no longer pressed):
		tosetKeys[j] = -1; //Use -1 as a placeholder for an 'off' key for now;
	}
	
	int m = 0;
	
	// Compare new keys to old keys to find which ones are new:
	for (i=0;i<5;i++) 
	{
		for (j=0;j<5;j++)
		{
			if (newKeys[i] == currentKeys[j])
			{
				break;
			}
		}
		// If you don't break (there is a new key):
		toaddKeys[i] = newKeys[i];
		// Index to keep track of length of toaddKeys:
		m++;
		
	}	
	
	// Place these new keys into the spaces in the tosetKeys:
	j = 0;
	for (i=0;i<5;i++)
	{
		// If you find a key that is unused and you've still got new keys to add, put it in from keys to add:
		if (tosetKeys[i] == -1)&&(j <= m) {
			tosetKeys[i] = toaddKeys[j++];
		}
	}
	
	currentKeys = tosetKeys;
	
	// Translate these now to an array of 8 bit words that correspond to each counters actual setting: (use -1 again to be an unused oscillator):
	for (i=0;i<5;i++) {
		currentFreqs[i] = keyfreqs(currentKeys[i]);
	}
	
	for (i=0;i<5;i++)
	{
		if (currentFreqs[i] != -1) {
			// See address listing to make sense of this:
			if (i>2) 
			{
				setAddress(C1_COUNT0+i+1);
			}
			else {
				setAddress(C1_COUNT0+i);
			}
			
			setData(-1,currentFreqs[i]);
			setCTRLCLK(1);
			setCTRLCLK(0);
		}
	}
	
	
	// Starting from the first oscillator, fill unused oscillators with new notes:
	
}

int setOSCCV(void) {
	// Sets all of the control voltages on each oscillator	
	
	currentCVs = keytoCV(currentKeys);
	
	for (i=0;i<5;i++) {
		setAddress(CV0+i);
		setData(-1,currentCVs[i]);
		setCTRLCLK(1);
		setCTRLCLK(0);
	}
	
}

int setOSCGate(void) {
	// Sets the gate memory to which oscillators are currently pressed. To be sent to the ADSR later etc.
	setCTRLCLK(1);
	// Other 2 bits of the memory are just don't cares I guess (it' an 8 bit D latch.)
	for (i=0;i<5;i++) {
		setAddress(GATES);
		
		if (currentFreqs[i] != -1) {
			setData(i,1);
		} else {
			setData(i,0);
		}
	}
	setCTRLCLK(0);
	
}

int keytoCV(keynum) {
	// Takes a key number and returns the proper binary setting for the control voltage on the chip
	
}

int keytoBinary(keynum) {
	// Takes a key number and returns the proper counter binary setting
	
	
}

int counterInit(void) {
	// Initialize the counters according to the datasheet.
	
	// I think we want mode 2 (rate generator.) I'm still slightly confused as to how you actually configure it using the word address.
}

int setAddress(addr) {
	// Sets the shift register to the appropriate binary address
	
	setSR(...);
	
}

int setSR(datapin,clockpin,value) {
	// General use function for a shift register, through defining which pins act as the data and the clock, attempts to blindly configure them with a particular value
	
	
}

int setTritone(trinum) {
	// Configures the SR to set a particular tritone
	
	setSR(...);
	
}

int readData(datapin) {
	// General use, Returns the value of a particular data pin. Should return an error if an attempt to read is made on a pin set to output.
}

int setData(bitbyte,data) {
	// General use, lets you set either a particular pin on the databus or all of them at once.
	// bitbyte = -1 for a whole byte, or the number of the bit for a particular pin (0-7)
}

int setCTRLCLK(val) {
	// General use, Sets the control clock pin either low or high.
	
}