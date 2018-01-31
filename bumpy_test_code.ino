/*
   Bumpy the 3d printed Arduino obstacle avoider.

   Goal in life...
      Bumps around trying to stay out of the way :-)

   Written by Scott Beasley - 2018
   Free to use or modify. Enjoy.
*/

/*
 * Connect the left servo to pin 7, the right on pin 9
 * Connect the sonar trigger to pin 3 and echo to pin 8.
 */

/*
   Uses the Arduino Servo library.
*/

#include <Servo.h>

#define microsecondsToCentimeters(microseconds) (unsigned long)microseconds / 29.1 / 2.0
#define MIN_ACTION_DIST 20 // 20 cm

// Change these defines if you use differnt pins
#define TRIGGER_PIN 3  // Digital pin 3.
#define ECHO_PIN 8 // Digital pin 8
#define LEFT_SERVO 7 // Pin used for the left servo
#define RIGHT_SERVO 9 // Pin used for the right servo

// Speed defines
#define MAXFORWARDSPEED 120 // Max speed we want moving forward
#define MAXBACKWARDSPEED 60 // Max reverse speed
#define STOP 90

// Various time delays used for driving and servo
#define TURNDELAY 450
#define BACKUPDELAY 400

/*
   Globals area. Try to keep them to a minimum :-)
*/

// Create the servo objects with use to interface with
Servo motor_left; // Create Left servo motor object
Servo motor_right; // Create Right servo motor object

unsigned long left_dist, right_dist; // Distance measured left and right

void setup ( )
{
   motor_left.attach (LEFT_SERVO); // Attach the servo to the digital pin
   motor_left.write (STOP); // Set the servo to the middle (neutral) pos
   motor_right.attach (RIGHT_SERVO); // Attach the servo to the digital pin
   motor_right.write (STOP); // Set the servo to the middle (neutral) pos

   // Set modes for proximity sensor pins
   pinMode (ECHO_PIN, INPUT);
   pinMode (TRIGGER_PIN, OUTPUT);
}

void loop ( )
{
   unsigned long dist_fwd;

   // Get a reading from the sonar sensor
   dist_fwd = ping ( );
   //Serial.print ("Distance sensor reading: ");
   //Serial.println (dist_fwd);

   // Go forward while nothing is in the distance sensors read area
   if (dist_fwd > MIN_ACTION_DIST || dist_fwd == 0)
   {
      go_forward ( );
   }
   else // There is something in the sensors read area
   {
      halt ( ); // Stop!
      go_backward ( ); // Back up a bit
      delay (BACKUPDELAY);
      halt ( ); // Stop!

      // Look left and right and see which is more open.
      go_left ( );
      delay (TURNDELAY/2);
      halt ();
      left_dist = ping ();
      
      go_right ();
      delay (TURNDELAY);
      halt ( );
      right_dist = ping ();

      // Turn back left if better, else stay right.
      if (left_dist >= right_dist) {
         go_left ();
         delay (TURNDELAY);
         halt ();
      }
      
      delay (TURNDELAY);
   }
}

// Read the HC-SR04 uSonic sensor
unsigned long ping ( )
{
   // Trigger the uSonic sensor (HC-SR04) to send out a ping
   digitalWrite (TRIGGER_PIN, LOW);
   delayMicroseconds (5);
   digitalWrite (TRIGGER_PIN, HIGH);
   delayMicroseconds (10);
   digitalWrite (TRIGGER_PIN, LOW);

   // Measure how long the ping took and convert to cm's
   return (microsecondsToCentimeters (pulseIn (ECHO_PIN, HIGH)));
}

void go_forward ( )
{
   motor_left.write (MAXFORWARDSPEED);
   motor_right.write (MAXBACKWARDSPEED);
}

void go_backward ( )
{
   motor_left.write (MAXBACKWARDSPEED);
   motor_right.write (MAXFORWARDSPEED);
}

void go_left ( )
{
   motor_left.write (MAXFORWARDSPEED);
   motor_right.write (MAXFORWARDSPEED);
}

void go_right ( )
{
   motor_left.write (MAXBACKWARDSPEED);
   motor_right.write (MAXBACKWARDSPEED);
}

void halt ( )
{
   motor_left.write (STOP);
   motor_right.write (STOP);
}
