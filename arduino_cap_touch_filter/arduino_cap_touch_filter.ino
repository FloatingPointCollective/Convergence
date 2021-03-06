#include <Wire.h>
#include "Adafruit_MPR121.h"
#define NBOARDS 3

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap[ NBOARDS ] = Adafruit_MPR121();

bool debug = 1;
/*int false_touch_timeout = 200;
int current_time = 0;
int touch_time[ NBOARDS ][ 12 ] = {{ 0 }};
bool pos_touch[ NBOARDS ][ 12 ] = {{ false }};
bool touch[ NBOARDS ][ 12 ] = {{ false }};*/

int cap_reading[ NBOARDS * 12 ][ 10 ] = {{ 0 }};
int cap_total[ NBOARDS * 12 ] = { 0 };
float cap_avg[ NBOARDS * 12 ] = { 0 };
int num_readings = 10;
//int avg_base_difference[ NBOARDS * 12 ] = { 0 };

//int read_index = 0;

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched[ NBOARDS ];
uint16_t currtouched[ NBOARDS ];

void setup() {
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!

  Serial.begin(9600);
  if ( debug ) {
    Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  }
  
  for ( int j = 0; j < NBOARDS; j++ ) {
    lasttouched[ j ] = 0;
    currtouched[ j ] = 0;
  }
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
 
  if (!cap[ 0 ].begin(0x5A)) {
    if ( debug ) { Serial.println("MPR121 1 not found, check wiring?"); }
    while (1);
  }
  if ( debug ) { Serial.println("MPR121  1 found!"); }
  
  //2nd board cap[ 1 ] is tied to 3.3V
  if (!cap[ 1 ].begin(0x5B)) {
    if ( debug ) { Serial.println("MPR121 2 not found, check wiring?"); }
    while (1);
  }
  Serial.println("MPR121  2 found!");
  
   //3rd board cap[ 2 ] is tied to SDA
  if (!cap[ 2 ].begin(0x5C)) {
    if ( debug ) { Serial.println("MPR121 3 not found, check wiring?"); }
    while (1);
  }
  if ( debug ) { Serial.println("MPR121  3 found!"); }

  //current_time = millis();

}

void loop() {
  // Get the currently touched pads
  /*for ( int j = 0; j < NBOARDS; j ++ ) {
      //currtouched[ j ] = cap[ j ].touched();
  
    for (uint8_t i=0; i<12; i++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched[ j ] & _BV(i)) && !(lasttouched[ j ] & _BV(i)) ) {

        touch_time[ j ][ i ] = millis();
        pos_touch[ j ][ i ] = true;
        //Serial.println( j * 24 + i * 2 ); //even number to indicate on
        }
        
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched[ j ] & _BV(i)) && (lasttouched[ j ] & _BV(i)) ) {
          pos_touch[ j ][ i ] = false;
        
        }*/


    

  for ( int j = 0; j < NBOARDS; j ++ ) {
    for (uint8_t i=0; i<12; i++) {
        
        for ( int iter = 1; iter < num_readings; iter ++ ) {
          cap_reading[ (j + 1 ) * i][ iter - 1 ] = cap_reading[ (j + 1 ) * i][ iter ];
        }

        if ( cap[ j ].filteredData( i ) > cap[ j ].baselineData( i ) + 15 || cap[ j ].filteredData( i ) > cap[ j ].baselineData( i ) - 15 ) {
          cap_reading[ (j + 1 ) * i][ num_readings - 1 ] = cap[ j ].baselineData( i );
        }
        else {
          cap_reading[ (j + 1 ) * i][ num_readings - 1 ] = cap[ j ].filteredData( i );
        }

        for ( int iter = 0; iter < num_readings; iter ++ ) {
          cap_total[ (j + 1) * i ] += cap_reading[ (j + 1 ) * i][ iter ];
        }
        cap_avg[ (j + 1) * i ] = cap_total[ (j + 1) * i ] / num_readings;
        if ( cap_avg[ (j + 1) * i ] == 0 ) {
          cap_avg[ (j + 1) * i ] = 1;
        }

        cap_total[ (j + 1) * i ] = 0;

        //avg_base_difference[ (j + 1) * i ] = (int) ( cap_avg[ (j + 1) * i ] - cap[ j ].baselineData( i ));
        

        
        /*if ( debug ) {
          Serial.print((j + 1 ) * i ); Serial.print("\t");
          Serial.print(  cap_avg[ (j + 1) * i ]);  Serial.print("\t");
          //Serial.print( baselineData( i );
          Serial.print( 0 );  Serial.print("\t");
          Serial.println();

          /*Serial.print(0); Serial.print("\t");
          Serial.print(  avg_base_difference[ 0]);  Serial.print("\t");
          //Serial.print( baselineData( i );
          Serial.print( 0 );  Serial.print("\t");
          Serial.println();
        }
        else {
          Serial.write( (j + 1 ) * i );
          Serial.write( (int) cap_avg[ (j + 1) * i ] );
          Serial.write( 0 );
        }*/
     
      
      //lasttouched[ j ] = currtouched [ j ];
    }

   /*for ( int j = 0; j < NBOARDS; j ++ ) {
    for (uint8_t i=0; i<12; i++) {
       cap_avg[ (j + 1) * i ] = cap_total[ (j + 1) * i ] / num_readings;
    }
   }*/
 

  //touch_filter();
  
  // reset our state
  //current_time = millis();

  


  delay(100);
  }
  // comment out this line for detailed data from the sensor!
  //return;
  
  for ( int j = 1; j < 2; j ++ ) {
    // debugging info, what
    Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap[ j ].touched(), HEX);
    Serial.println();
    
    Serial.print("Filt: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap[ j ].filteredData(i)); Serial.print("\t");
      //Serial.print(cap_reading[ (j + 1) * i ][ 0 ]); Serial.print("\t");

    }
    
    Serial.println();
    Serial.print("read: ");
    for (uint8_t i=0; i<12; i++) {
      //Serial.print(cap[ j ].filteredData(i)); Serial.print("\t");
      Serial.print(cap_reading[ (j + 1) * i ][ num_readings ]); Serial.print("\t");

    }
    
    Serial.println();
    Serial.print("Avg: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap_avg[ (j + 1) * i ]); Serial.print("\t");
    }
    
    Serial.println();
    Serial.print("Base: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap[ j ].baselineData(i)); Serial.print("\t");
    }
    Serial.println();
    
    // put a delay so it isn't overwhelming
    delay(100);
   }
}

/*void touch_filter() {

  for ( int j = 0; j < NBOARDS; j ++ ) {
    for ( int i = 0; i < 12; i ++ ) {
      if ( pos_touch[ j ][ i ] ) {
        if ( current_time - touch_time[ j ][ i ] > false_touch_timeout ) {

          touch[ j ][ i ] = true;

          if(debug){        
            Serial.println( j * 24 + i * 2 ); //even number to indicate on
            Serial.print("//////////////////////////////////////////////////////////////////////////////////////////");
          }
          else {
            Serial.write( j * 24 + i * 2 ); //even number to indicate on
          }

        }
      }

      else if ( touch[ j ][ i ]) {

        if ( !pos_touch[ j ][ i ] ) {

          if(debug){
            Serial.print("************************************************************************************************");
            Serial.println( j * 24 + i * 2 + 1 ); //odd number to indicate off
          }

          else{
            Serial.write( j * 24 + i * 2 + 1 ); //odd number to indicate off
          }

        }
      }
    }
  }
}*/

