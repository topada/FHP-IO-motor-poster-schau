#include <Servo.h> //servo lib
#define ir A0
#define servo 9

#define arr_len( x ) ( sizeof( x ) / sizeof( *x ) ) // array_len function


Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position

int ir_mem[10]; // messuarements for ir average
int m = arr_len(ir_mem); //messuarement values of ir_mem

int d_smooth = 0; //smoothed distance d from ir sensor
int d_old = 0;
int servo_delay = 130;

void setup () {
        Serial.begin (9600);
        pinMode(ir, INPUT);
        
        myservo.attach(servo);  // attaches the servo on pin 9 to the servo object
        myservo.write(pos); // clean start for servo
        delay(servo_delay); // let him do this thing
}

void loop () {
        
        delay (servo_delay);
        
        int smooth_d = smooth_ir();
        //Serial.println(smooth_d);
        
        if(smooth_d != d_old)
        {
            if(smooth_d >= 25)
            {
              pos = 0; 
            }
            else if(smooth_d <= 5)
            {
              pos = 110;
    
            }
            else{
              pos = map(smooth_d, 25, 5, 0, 110);
              //Serial.print("else ");
              //Serial.println(d_smooth);
            }
            
            Serial.println(pos);
            myservo.write(pos);
        }
        
        d_old = smooth_d;
               
}

//return distance in (cm)
int get_IR (uint16_t value) {
        if (value < 16)  value = 16;
        return int(2076.0 / (value - 11.0));
}


/*
smooth_ir
this function is my attempt to smooth the IR signal digitally by
calculating the average value of multiple meassurements

require
int array ir_mem[] (array storage for the messuarement values)
int m (amount of values in ir_mem[])

return
int smooth_d (average from m values)

*/
int smooth_ir()
{
   //reset d_smooth
   d_smooth = 0;
  
   //update ir_mem
   for (int i = 0; i <= m-1; i++) {
     //shift value
     if(i < m-1){
       ir_mem[i] = ir_mem[i+1];  
       //Serial.print(ir_mem[i]);    //debuggatory
       //Serial.print(" ");          //debuggatory
     }
     //add new value
     else{
       uint16_t value = analogRead (ir);
       int d = get_IR (value); //Convert the analog voltage to the distance
       
       ir_mem[i] = d;
       //Serial.print(ir_mem[i]);  //debuggatory
     }
     //cumulate values from new ir_mem Array
     d_smooth = d_smooth + ir_mem[i];

   }
   //average from d_smooth cumulation and messuarments amounts
   d_smooth = int(d_smooth / m);
   return d_smooth;
}
