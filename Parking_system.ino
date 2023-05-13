
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows

#define ir_1 2
#define ir_2 3
#define trig 4
#define echo 5
#define fire_pin A0
#define ir_num 2

int ir_array[2] = {ir_1, ir_2};

long duration = 0;
float distance = 0;
float gate = 4;

int tolerance = 30;

bool fire = false;

bool Park[ir_num];

void setup()
{
  
  Serial.begin(9600);
  
  lcd.init(); // initialize the lcd
  lcd.backlight();
  
  ir_initalize(ir_num, ir_array); //Intialize IR to read
  
  ultrasonic_initalize(trig, echo); // Intialize Ultrasonic Distant sensor
  
  fire_initialize(fire_pin);   //Intialize the gas sensor
}



void loop()
{
  IR_read();                        //Read the IR sensors
  
  ultrasonic_read(trig, echo);      //Read the Sonic Sensor
  
  fire_B(tolerance);                //Check is there is fire avialable
  
  parking_system();                 // main programme where the algo will decide to take next course of action.
}






void ir_initalize(int num, int arry[])      // Will go through the loop and set the pins their respective Input mode
{
 
  for (int i = 0; i < num; i++)
  { 
    pinMode(arry[i], INPUT);
    Park[i] = false;
  }
  
} 


void IR_read()                              //Will read the IR and set the Park variable to t or f indicating occupancy 
{                                           // Count not used
  int count = 0;
  
  for (int i = 0; i < ir_num; i++)
  {
    if (digitalRead(ir_array[i])) 
    {
     count++;
     Park[i] = true;
    }
    else Park[i] = false;
  }
  
}


void ultrasonic_initalize(int trig_pin,int echo_pin)
{                                                           // Intialazing the sonar pins
  pinMode(trig_pin,OUTPUT);
  
  pinMode(echo_pin, INPUT);
}


void ultrasonic_read(int trig_pin, int echo_pin)
{                                                           // Calculting the distance s = vt => 0.034*duration /2 => duration -> travel time
  
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  duration = pulseIn(echo_pin, HIGH);
  
  distance = (duration*0.034)/2; // CM
  
}

void fire_initialize(int pin)
{
  pinMode(pin, INPUT);      //initialize the gas sensor as input 
}

void fire_B (int tolerence)
{
  if (tolerence < analogRead(fire_pin)) fire = true;          // Checks is there is Gas in close proximity if there exist toggle fire variable
    else fire = false;
}

void parking_system()
{
  if (fire)
    {                                                         // main Algo checks for fire if there is fire then exit if there is not then check if gate is false then print 
      lcd_write("Fire !!!", 0, 0);                            // the occupancy is the is not "not available" and normally print parking status
      lcd_write("Gates are OPEN", 0, 1);
    }
    else if (gate < distance) 
    {
      parking_status();
    }
    else {
        parking_status();  
    }
}
void lcd_write (String w, int column, int row)
{
  lcd.clear();        
  lcd.setCursor(column, row);        
  lcd.print(w);       
  delay(1000);
  lcd.clear();
}

void parking_status()
{
  if (Park[0] && Park[1])
      {
        lcd_write("Not Avialable", 0, 0);
        lcd_write("Both Parking", 2, 2);
        lcd_write("Are Occupied", 3, 3);
      }
      else if (Park[1])
      {
        lcd_write("Parking 1: Empty", 0, 0);
        lcd_write("Parking 2: Full", 0, 1);
      }
      else if (Park[0])
      {
        lcd_write("Parking 1: Full", 0, 0);
        lcd_write("Parking 2: Empty", 0, 1);
      }
      else 
      {
        lcd_write("Parking 1: Empty", 0, 0);
        lcd_write("Parking 2: Empty", 0, 1);
      };
}
