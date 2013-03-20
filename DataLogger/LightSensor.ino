//
//    o------/\/\/\-----o-----[photocell]------o
//   gnd      10k       A1                     5v
//

//pin definitions
#define PHOTOCELL 1 //photocell analog in pin 1

//variable definitions
long val = 0;        //stores raw value from photocell
long maxread = 0;    //maximum value from calibration phase
long minread = 1000;  // minimum value from calibration phase
double f = 0;         // frequency of sound
double normf = 0;    // normalized frequency 
double logf = 0;      // logarithm of normalized frequency
int ilogf = 0;        // rounded logarithm
//int i = 0;            // loop dummy variable
double factor = 0;    // scaling factor for calibration
double shift = 0;     // shift for calibration
long maxfreq = 1048;  // maximum desired frequency after calibration
long minfreq = 131;   // minimum desired frequency after calibration


//magic numbers that make the intervals sound pleasing
//double gap = 1.148698355;  //ratio of consecutive notes (pentatonic)
// it's the 5th root of 2
double gap = 1.059463094;  //ratio of consecutive notes (chromatic)
// its the 12th root of 2


//void setup()
//{
//  pinMode(PHONES, OUTPUT);    // sets the digital pin as output
//  Serial.begin(9600);
//  calibrate();
//  
//  //Now we use the calibration to calculate scale and shift parameters
////  factor = (double)(maxfreq - minfreq) / (double)(maxread - minread); // scale parameter: it's like a slope
////  shift = factor * minread - minfreq;  //shift parameter: it's like an offset
//  factor = 255.0/(maxread-minread);
//  Serial.println(factor);
//}

//void loop()
//{
//  delay(500);
//  val = analogRead(PHOTOCELL);   // read photocell 
//  Serial.println("speed = " + String(int((val-minread)*factor)) + "(" + String(int(val-minread)) + ")");
//
////  f = factor * val - shift;     // this linearly maps the frequency to
////                                // a value between minfreq and maxfreq
////                                // according to the calibration result
////  
////  normf = f / (double) minfreq;  // Dividing an exponential function by the min value
////  logf = log(normf) / log(gap); // allows us to take the log (base gap) and the result 
////  ilogf = round(logf);           // is the number of notes above the lowest, once we round it.
////  f = minfreq * pow(gap,ilogf);  // we better "unlog" it.
//////  Serial.println(val);
////  
////  tone(PHONES, f);              // this produces the tone signal 
//}

// calibration loop to determine a rasonable range of light levels (minread to maxread)
// and map that to frequencies between minfreq and maxfreq
void calibrate() {
  maxread = 0;
  minread = 1000;

  Serial.println("calibrating light sensor...");
//  for (int i = 0; i< 500; i++) {  	// calibration loop runs for 5 seconds
//    val = analogRead(PHOTOCELL);	// read photocell 
//    maxread = max(maxread, val);
//    minread = min(minread, val);
//    delay(10);                  // reasonable delay
//  } 
  minread = 40;
  maxread = 960;
//  Serial.println("done: " + String(minread) + " to " + String(maxread));
}  

