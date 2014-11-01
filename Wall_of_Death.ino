// PINs
const int cNumberOfLeds = 8;
const int cLed[] = {2,3,4,5,6,7,8,9}; 
const int cTrigger = A0;
const int cEcho = A1;

// Grenzwerte
const unsigned int cHoldDuration = 500; // LEDs bleiben 500ms an
const unsigned int cDurationMax = 1176; // ca. 200mm
unsigned int duration_min = cDurationMax - 1;
unsigned long start_time = 0;
byte hold_led = 0;

// Messwerte
const unsigned int cBufferSize = 3; // Für die Glättung der Eingangsmesswerte
unsigned int buffer_[cBufferSize];
byte buffer_ptr_ = 0;

unsigned int duration = 0; // Dauer des Impulses vom Ultraschallsensor

void setup() 
{
  // initialize serial output
  Serial.begin(115200);  
  Serial.println("");

  pinMode(cTrigger,OUTPUT);
  pinMode(cEcho,INPUT);
  digitalWrite(cTrigger, HIGH);

  for(int i = 0; i < cNumberOfLeds; i++)
  {
    pinMode(cLed[i],OUTPUT);
  }
  
}

void loop()
{
// Ultaschallsensor einlesen
  digitalWrite(cTrigger, LOW);
  duration = pulseIn(cEcho, HIGH);
  digitalWrite(cTrigger, HIGH);
  if ((duration < duration_min) && (duration > 250))
  {
    duration_min = duration;
  }
// Messwert begrenzen  
  duration = constrain(duration, duration_min, cDurationMax);
// Messwert glätten
  buffer_[buffer_ptr_] = duration;
  buffer_ptr_++;
  buffer_ptr_ %= cBufferSize;
  duration = 0;
  for (byte i = 0; i < cBufferSize; i++)
  {
    duration += buffer_[i];
  }
  duration /= cBufferSize;

// Wieviele LEDs müssen leuchten
  unsigned int value = map(duration, cDurationMax, duration_min + 20, 0, cNumberOfLeds);
  
// Hold berechnen  
  if (value >= hold_led)
  {
    hold_led = value;
    start_time = millis(); 
  }
  if ((start_time + cHoldDuration) < millis())
  {
    hold_led = 0; 
  }
  
// Infos auf die serielle Schnittstelle schreiben  
  Serial.print("Duration: ");
  Serial.print(duration);
  Serial.print(" \tDuration Min: ");
  Serial.print(duration_min);
  Serial.print(" \tDuration Max: ");
  Serial.print(cDurationMax);
  Serial.print(" \tstart_time: ");
  Serial.print(start_time);
  Serial.print(" \thold_led: ");
  Serial.print(hold_led);
  Serial.print("\t LED: ");
  Serial.println(value);
    
// LEDs ansteuern
  for (int i = 0; i < cNumberOfLeds; i++)
  {
    digitalWrite(cLed[i], hold_led > i);
  }

// Pause bis zur nächsten Messung
  delay(100);
}




