/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our robot, Yamaraz.
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the COPYING file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * This particular chunk of code (basically the comments
 * and the variables) is a fork of
 * Thomas Miller's experiment with Tick Tracers
 * here at the UNO Robotics Laboratory.

 * Sincerely,
 * Nishan Rayamajhee
 */

#define ARM_MATH_CM4
#include <arm_math.h>

int SAMPLE_RATE_HZ = 1000;
const int FFT_SIZE = 256;

//Analog Input Signal Read Settings
const int ANALOG_READ_RESOLUTION = 10;                          //This is the ADC reading resolution. I.E.: 10-bit=0-1024 (UNO/MEGA maximum), 13-bit=0-8192 (Teensy Usable Maximum), 16-bit=0-65536 (Teensy Actual Maximum Using Precision AREF Resistor)
const int ANALOG_READ_AVERAGING = 100;                          //If you want to average X number of readings together PER analog read, before putting that number into the array that will be used to perform the FFT, this is where you adjust raw analog sample reading averaging.

IntervalTimer samplingTimer;                                    //Initializes the Teensy's IntervalTimer function as samplingTimer. You can have up to 4 of these interrupt based timers on a Teensy. 
                                                                //IntervalTimer uses interrupts to call a function at a precise time. It is initialized (in pseudocode) as IntervalTimer.begin(function-to-run,microseconds-between-runs).
float samples[FFT_SIZE*2];                                      //This is the sample array of analog data you're pumping through the FFT algorithm. For Nyquist-compliant sampling, sample count is 2x the samples you're trying to discretize.
float magnitudes[FFT_SIZE];                                     //This is the array of magnitude values output by the FFT algorithm.
                                                                //These magnitudes are the magnitudes in each 'bin'. If 60Hz falls in Bin 15, you should call on magnitudes[15]
                                                                //yourbinmagnitudevalue = 20.0*log10(magnitudes[##]) can convert it to decibels if you want. 
int sampleCounter = 0;                                          //sampleCounter global variable says how many analog samples have been logged to the samples array, resets each time sampling completes.
int sampleSource = tickPin;                                     //This tells the sampleBegin() function which source to sample from. By default I set it to the TIC1_INPUT_PIN (Digital 14)

// begin sampling
void samplingBegin() {
  sampleCounter = 0;                                            // Reset sample buffer position and start callback at necessary rate.
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}
// sampling callback
void samplingCallback() {
  samples[sampleCounter] = (float32_t)analogRead(sampleSource); // Read from the ADC and store the sample data
  samples[sampleCounter+1] = 0.0;                               // Complex FFT functions require a coefficient for the imaginary part of the input.
                                                                // Since we only have real data, set this coefficient to zero.
  sampleCounter += 2;                                           // Update sample buffer position and stop after the buffer is filled
  if (sampleCounter >= FFT_SIZE*2) samplingTimer.end();
}

void initTick(){
  Serial.begin(9600);                                          //Set up serial port and set communication baud rate to 38400
  pinMode(tickPin, INPUT);                                      //initialize the I/O pin as input
  pinMode(tickButton, OUTPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);                 //Tells the Teensy to perform analog reads at the resolution you set above.
  analogReadAveraging(ANALOG_READ_AVERAGING);                   //Tells the Teensy to average X number of samples per each sample.
}

void initThump() {
  pinMode(thumper, OUTPUT);
  pinMode(mic, INPUT);
}

void turnItOn() {
  // Turn the Tick Tracer On
  digitalWrite(tickButton, HIGH);
  delay(500);
  digitalWrite(tickButton, LOW); 
  delay(1000);
}

int getTick(){
  sampleSource = tickPin;
  // Start Sampling
  samplingBegin();
  delay(500);

  // When it's done
  if(sampleCounter >= FFT_SIZE*2) {
    arm_cfft_radix4_instance_f32 fft_inst;                      //Calls the ARM FFT Library, sets up fft_inst instance for a CFFT of radix4 (base4) at single precision floating point 32-bit numbers.
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);        //Tells the FFT Library you're using instance 'fft_inst', the FFT Bin Size is 'FFT_SIZE', ifftFlag '0' tells it to perform a forward direction FFT (as opposed to inverse '1'), and bitReverseFlag '1' says we want the output bits to stream in the reverse direction.
    arm_cfft_radix4_f32(&fft_inst, samples);                    //Control structure initialization for FFT function
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);           // Calculate magnitude of complex numbers output by the FFT. 'samples' is interleaved as [real, imag, real, imag, real, imag,...] and the first bin is the total bin magnitude (basically ignore it).
  }

   return (int)magnitudes[15];
}

bool tickIt() {
  // ignore first reading
  getTick();
  
  if (getTick() > 300)
    return true;
  else
    return false;
  
}

int thump() {
  delay(250);
  digitalWrite(thumper, HIGH);
  delay(25);
  
  sampleSource = mic; 
  
  // Start Sampling
  samplingBegin();
  delay(500);

  // When it's done
  if(sampleCounter >= FFT_SIZE*2) {
    arm_cfft_radix4_instance_f32 fft_inst;                      //Calls the ARM FFT Library, sets up fft_inst instance for a CFFT of radix4 (base4) at single precision floating point 32-bit numbers.
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);        //Tells the FFT Library you're using instance 'fft_inst', the FFT Bin Size is 'FFT_SIZE', ifftFlag '0' tells it to perform a forward direction FFT (as opposed to inverse '1'), and bitReverseFlag '1' says we want the output bits to stream in the reverse direction.
    arm_cfft_radix4_f32(&fft_inst, samples);                    //Control structure initialization for FFT function
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);           // Calculate magnitude of complex numbers output by the FFT. 'samples' is interleaved as [real, imag, real, imag, real, imag,...] and the first bin is the total bin magnitude (basically ignore it).
  }


  int sum = 0;
  int intervalSum = 0;
  
  for(int i = 20; i < 60; i++){
   sum += magnitudes[i];
  }


//  /* DEBUG displays the entire bin*/
//  for (int i = 1; i < 256; i++) {
//    intervalSum += magnitudes[i];
//    if(i % 10 == 0) {
//      Serial.print("Sum to ");
//      Serial.print(i);
//      Serial.print(" : ");
//      Serial.println(intervalSum);
//      intervalSum = 0;
//    }
//  }
  
  Serial.println();
  Serial.println(sum);
  Serial.println();
  
  digitalWrite(thumper,LOW); 
  delay(1000);

  if (sum < 42000) {
    
    Serial.println("Its solid!");
    return SOLID;
    
  } else {
    
     Serial.println("It's hollow!");
     return HOLLOW;
  }
}

int thumpAnalog() {
  int counter = 0;
  int arrayIndex = 20000;
  int getData[arrayIndex];

  digitalWrite(thumper, HIGH);
  delay(25);

  for (int i = 0; i < arrayIndex; i++) {
    getData[i] = analogRead(9);       //sample data
    if (getData[i] > 900|| getData[i] < 100)         //if the sampled data is greater than threshold increase counter
    counter++;
  }
  delay(100);
  
  digitalWrite(thumper,LOW);     //take it up
  delay(500);
  Serial.print("Counter = ");
  Serial.println(counter);          //Display counter. It is different for hollow and foam

  // Threshhold for cell
  if (counter < 2000) {
    
    Serial.println("Solid!");
    return SOLID;
    
  } else {
    
     Serial.println("Hollow!");
     return HOLLOW;
  }
}

