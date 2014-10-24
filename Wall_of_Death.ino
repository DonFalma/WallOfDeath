// PINs
const int number_of_leds = 8;
const int led[] = {2,3,4,5,6,7,8,9}; 
const int trigger = A0;
const int echo = A1;

// Grenzwerte
const unsigned int duration_max = 1176; // ca. 200mm
unsigned int duration_min = duration_max - 1;
const unsigned int hold_duration = 500; // LEDs bleiben 500ms an
unsigned long start_time = 0;
byte hold_led = 0;

// Messwerte
const unsigned int buffer_size_ = 3; // Für die Glättung der Eingangsmesswerte
unsigned int buffer_[buffer_size_];
byte buffer_ptr_ = 0;

unsigned int duration = 0; // Dauer des Impulses vom Ultraschallsensor

void setup() 
{
  // initialize serial output
  Serial.begin(115200);  
  Serial.println("");

  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  digitalWrite(trigger, HIGH);

  for(int i = 0; i < number_of_leds; i++)
  {
    pinMode(led[i],OUTPUT);
  }
  
}

void loop()
{
// Ultaschallsensor einlesen
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  digitalWrite(trigger, HIGH);
  if ((duration < duration_min) && (duration > 250))
  {
    duration_min = duration;
  }
// Messwert begrenzen  
  duration = constrain(duration, duration_min, duration_max);
// Messwert glätten
  buffer_[buffer_ptr_] = duration;
  buffer_ptr_++;
  buffer_ptr_ %= buffer_size_;
  duration = 0;
  for (byte i = 0; i < buffer_size_; i++)
  {
    duration += buffer_[i];
  }
  duration /= buffer_size_;

// Wieviele LEDs müssen leuchten
  unsigned int value = map(duration, duration_max, duration_min + 20, 0, number_of_leds);
  
// Hold berechnen  
  if (value >= hold_led)
  {
    hold_led = value;
    start_time = millis(); 
  }
  if ((start_time + hold_duration) < millis())
  {
    hold_led = 0; 
  }
  
// Infos auf die serielle Schnittstelle schreiben  
  Serial.print("Duration: ");
  Serial.print(duration);
  Serial.print(" \tDuration Min: ");
  Serial.print(duration_min);
  Serial.print(" \tDuration Max: ");
  Serial.print(duration_max);
  Serial.print(" \tstart_time: ");
  Serial.print(start_time);
  Serial.print(" \thold_led: ");
  Serial.print(hold_led);
  Serial.print("\t LED: ");
  Serial.println(value);
    
// LEDs ansteuern
  for (int i = 0; i < number_of_leds; i++)
  {
    digitalWrite(led[i], hold_led > i);
  }

// Pause bis zur nächsten Messung
  delay(100);
}




