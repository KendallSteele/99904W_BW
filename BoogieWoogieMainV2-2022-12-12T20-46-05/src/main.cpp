


#include "vex.h"
using namespace vex;
competition Competition;
///////////////////////////////////
bool c1 = true;
bool c2 =true;
bool tog1 = false;
bool tog2=false;
bool loaded = true; //false=unloaded//true=loaded
bool  active = false;


///////////////////////////////////


//██████████████████████████████████████████
//█░░░░░░░░░░░░░░█░░░░░░░░░░█░░░░░░░░░░░░███
//█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀░░░░█
//█░░▄▀░░░░░░▄▀░░█░░░░▄▀░░░░█░░▄▀░░░░▄▀▄▀░░█
//█░░▄▀░░██░░▄▀░░███░░▄▀░░███░░▄▀░░██░░▄▀░░█
//█░░▄▀░░░░░░▄▀░░███░░▄▀░░███░░▄▀░░██░░▄▀░░█
//█░░▄▀▄▀▄▀▄▀▄▀░░███░░▄▀░░███░░▄▀░░██░░▄▀░░█
//█░░▄▀░░░░░░░░░░███░░▄▀░░███░░▄▀░░██░░▄▀░░█
//█░░▄▀░░███████████░░▄▀░░███░░▄▀░░██░░▄▀░░█
//█░░▄▀░░█████████░░░░▄▀░░░░█░░▄▀░░░░▄▀▄▀░░█
//█░░▄▀░░█████████░░▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀░░░░█
//█░░░░░░█████████░░░░░░░░░░█░░░░░░░░░░░░███
//██████████████████████████████████████████


//reset Motor Rotaions
void resetRotaions(){
  fLDrive.resetPosition();
  fRDrive.resetPosition();
  mLDrive.resetPosition();
  mRDrive.resetPosition();
  bLDrive.resetPosition();
  bRDrive.resetPosition();
}
//makes the left and right motors spin (mainly for abstracion)
void move(int left,int right){
    fLDrive.spin(fwd,left,volt);
    fRDrive.spin(fwd,right,volt);
    mLDrive.spin(fwd,left,volt);
    mRDrive.spin(fwd,right,volt);
    bLDrive.spin(fwd,left,volt);
    bRDrive.spin(fwd,right,volt);
}
//gets the percent of the number
float getPercentSpeed( int num ){
	float percent;
	if(num < 10){
		percent = num * 0.1;
	} else {
		percent = 1.0;
	}
  return (percent);
}
//converts input into msec
int secondsToMsec(float seconds){
	return (seconds*1000);
}


void drive(int inches, float timeOut,int speed){
  //coverting input in inches to degrees
  const int ticks = inches*(180/(3.25*3.141592653));
  //defining timer
  float T1 = vex::timer::system();
  //setting minimal speed
  const int MIN_SPEED = 10; 
  //defining constants for distance driving
  float kP_dist = 0;
	float kI_dist = 0;
	float kD_dist = 0;
  //defining constants for driving straight
	float kP_diff = 0;
	float kI_diff = 0;
	float kD_diff = 0;
  //setting error to 0 
  int error_dist = 0;
	int error_diff = 0;
  //setting the last error to 0
	int lastError_dist = 0;
	int lastError_diff = 0;
  //setting the intergral  of the distance driving to 0 derivitve can't be 0 at the start cause it wont move other wise 
  float integral_dist = 0;
	int derivative_dist = 2;
  //setting the intergral and derivite of the driving straight to 0
	float integral_diff;
	int derivative_diff;
  //covertingThe time out into the timout in milliseconds
  int timeOutMsec = secondsToMsec(timeOut);
  //defining the power of the motors
	int power_dist;
	int power_diff;
  //defining percent speed
	float percentSpeed;
  //setting iteration to 1 so motors will slowy ramp up 
	int iteration = 1;
  //setting errors to 0 cause they shouldn't have a value yet
	float	tot_error_diff = 0;
	float averageError_diff = 0;
  //reset motor postions
  resetRotaions();
  //will exit loop after time limit is over
  while(vex::timer::system()-T1<timeOutMsec){

    //defining error by doing expexed - actual 
    error_dist= ticks - ((bLDrive.position(deg)+bRDrive.position(deg))/2);
		error_diff= bLDrive.position(deg) - bRDrive.position(deg);
    
    percentSpeed = getPercentSpeed( iteration );
		iteration ++;
    //making sure that the intergral won't shoot up super fast
    if( error_diff != 0){
			integral_diff = integral_diff + error_diff;
		}else{
			integral_diff = 0;
		}
    //defining derivitive by doing error - previous error
    derivative_dist = error_dist - lastError_dist;
		derivative_diff = error_diff - lastError_diff;
    //setting last error to error now cause the error now has already been used
		lastError_dist = error_dist;
		lastError_diff = error_diff;
    //comibining everything together
		power_dist = (error_dist * kP_dist) + (integral_dist * kI_dist) + (derivative_dist * kD_dist);
		power_diff = (error_diff * kP_diff) + (integral_diff * kI_diff) + (derivative_diff * kD_diff);

    /*if( abs(power_dist) > speed )
		{
			power_dist = speed * abs(power_dist)/power_dist;
		}*/
  //adding it to the min speed then turning it into a percent
    int leftMotorSpeed =  MIN_SPEED + (power_dist - power_diff) * percentSpeed;
		int rightMotorSpeed = MIN_SPEED + (power_dist + power_diff) * percentSpeed;
    //telling the motors to move
    move(leftMotorSpeed,rightMotorSpeed);
    //adding to the total error
    tot_error_diff = tot_error_diff + error_diff;
    //re doing the average cause new total
		averageError_diff = tot_error_diff / iteration;

    wait(40,msec);
  }
  //stopping motors
  move(0, 0);
    fLDrive.setBrake(brake);
    fRDrive.setBrake(brake);
    mLDrive.setBrake(brake);
    mRDrive.setBrake(brake);
    bLDrive.setBrake(brake);
    bRDrive.setBrake(brake);

}

void runintake(){
  intakeL.spin(reverse,100,pct);
  intakeR.spin(reverse,100,pct);
}

void intakeStop(){
  intakeL.stop();
  intakeR.stop();
}

  void gyroturn (float target, float timeout) {
    float timeint = vex::timer::system();
    float speed, err; //rot;
    double p, k, frac;
    while (vex::timer::system() - timeint < timeout * 1000) {
      k = (target - christianPulisic.heading() + 540)/360;
      frac = modf(k, &p);
      err = (frac * 360) - 180;
      
      speed = (10 / cbrt(180)) * cbrt((frac * 360 - 180));

      fLDrive.spin(fwd,speed,percent);
      fRDrive.spin(fwd,speed,percent);
      mLDrive.spin(fwd,speed,percent);
      mRDrive.spin(fwd,speed,percent);
      bLDrive.spin(fwd,speed,percent);
      bRDrive.spin(fwd,speed,percent);

    }
      fLDrive.stop();
      fRDrive.stop();
      mLDrive.stop();
      mRDrive.stop();
      bLDrive.stop();
      bRDrive.stop();
  }

  void leftRedRoller(){
    tylerAdams.setLightPower(100, percent); 
    tylerAdams.setLight(ledState::on); 
    move(-290,100);
    wait(250, msec);
    while (1) {
   if(tylerAdams.color() == blue){
    runintake();
     }else{
      runintake();
      wait(100,msec);
      intakeStop();
    }
  }
}

void skills(){
    intakeL.setVelocity(100,pct);
    intakeR.setVelocity(100, pct);

    runintake();
  
    fLDrive.spin(reverse,100,pct);
    fRDrive.spin(reverse,100,pct);
    mLDrive.spin(reverse,100,pct);
    mRDrive.spin(reverse,100,pct);
    bLDrive.spin(reverse,100,pct);
    bRDrive.spin(reverse,100,pct);
    wait(200, msec);

    fLDrive.spin(fwd,0,pct);
    fRDrive.spin(fwd,0,pct);
    mLDrive.spin(fwd,0,pct);
    mRDrive.spin(fwd,0,pct);
    bLDrive.spin(fwd,0,pct);
    bRDrive.spin(fwd,0,pct);
     wait(1750,msec);

     move(300, 100);

     gyroturn(90, 1);

}

void pre_auton(void) {

}


//████████████████████████████████████████████████████████████████████████████████████
//█░░░░░░░░░░░░░░█░░░░░░██░░░░░░█░░░░░░░░░░░░░░█░░░░░░░░░░░░░░█░░░░░░██████████░░░░░░█
//█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░░░░░░░░░██░░▄▀░░█
//█░░▄▀░░░░░░▄▀░░█░░▄▀░░██░░▄▀░░█░░░░░░▄▀░░░░░░█░░▄▀░░░░░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░██░░▄▀░░█
//█░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░█████░░▄▀░░█████░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░▄▀░░██░░▄▀░░█
//█░░▄▀░░░░░░▄▀░░█░░▄▀░░██░░▄▀░░█████░░▄▀░░█████░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░██░░▄▀░░█
//█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀░░█████░░▄▀░░█████░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░██░░▄▀░░█
//█░░▄▀░░░░░░▄▀░░█░░▄▀░░██░░▄▀░░█████░░▄▀░░█████░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░██░░▄▀░░█
//█░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░█████░░▄▀░░█████░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░░░░░▄▀░░█
//█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░▄▀░░█████░░▄▀░░█████░░▄▀░░░░░░▄▀░░█░░▄▀░░██░░▄▀▄▀▄▀▄▀▄▀░░█
//█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█████░░▄▀░░█████░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░░░░░░░░░▄▀░░█
//█░░░░░░██░░░░░░█░░░░░░░░░░░░░░█████░░░░░░█████░░░░░░░░░░░░░░█░░░░░░██████████░░░░░░█
//████████████████████████████████████████████████████████████████████████████████████fortiteknadjkdbSJvlh

void autonomous(void) {
  leftRedRoller();
    /* intakeL.setVelocity(100,pct);
    intakeR.setVelocity(100, pct);

    runintake();
  
    fLDrive.spin(reverse,100,pct);
    fRDrive.spin(reverse,100,pct);
    mLDrive.spin(reverse,100,pct);
    mRDrive.spin(reverse,100,pct);
    bLDrive.spin(reverse,100,pct);
    bRDrive.spin(reverse,100,pct);
    wait(200, msec);
  

  

    fLDrive.spin(fwd,0,pct);
    fRDrive.spin(fwd,0,pct);
    mLDrive.spin(fwd,0,pct);
    mRDrive.spin(fwd,0,pct);
    bLDrive.spin(fwd,0,pct);
    bRDrive.spin(fwd,0,pct);
     wait(1750,msec);
    intakeStop();*/
}



//███████████████████████████████████████████████████████████████████████████████████████████████
//█░░░░░░░░░░░░███░░░░░░░░░░░░░░░░███░░░░░░░░░░█░░░░░░██░░░░░░█░░░░░░░░░░░░░░█░░░░░░░░░░░░░░░░███
//█░░▄▀▄▀▄▀▄▀░░░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░███░░▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░███
//█░░▄▀░░░░▄▀▄▀░░█░░▄▀░░░░░░░░▄▀░░███░░░░▄▀░░░░█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░░░░░░░▄▀░░███
//█░░▄▀░░██░░▄▀░░█░░▄▀░░████░░▄▀░░█████░░▄▀░░███░░▄▀░░██░░▄▀░░█░░▄▀░░█████████░░▄▀░░████░░▄▀░░███
//█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░▄▀░░█████░░▄▀░░███░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░░░░░░░▄▀░░███
//█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░█████░░▄▀░░███░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░███
//█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░▄▀░░░░█████░░▄▀░░███░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░░░░░▄▀░░░░███
//█░░▄▀░░██░░▄▀░░█░░▄▀░░██░░▄▀░░███████░░▄▀░░███░░▄▀▄▀░░▄▀▄▀░░█░░▄▀░░█████████░░▄▀░░██░░▄▀░░█████
//█░░▄▀░░░░▄▀▄▀░░█░░▄▀░░██░░▄▀░░░░░░█░░░░▄▀░░░░█░░░░▄▀▄▀▄▀░░░░█░░▄▀░░░░░░░░░░█░░▄▀░░██░░▄▀░░░░░░█
//█░░▄▀▄▀▄▀▄▀░░░░█░░▄▀░░██░░▄▀▄▀▄▀░░█░░▄▀▄▀▄▀░░███░░░░▄▀░░░░███░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀▄▀▄▀░░█
//█░░░░░░░░░░░░███░░░░░░██░░░░░░░░░░█░░░░░░░░░░█████░░░░░░█████░░░░░░░░░░░░░░█░░░░░░██░░░░░░░░░░█
//███████████████████████████████████████████████████████████████████████████████████████████████

double exponetial(double controllerValue){
  return pow(controllerValue, 3) / pow(100,2);
}

void usercontrol(void) {

  double turnAmt;
  double driveAmt;
  
  while (1) {

  //Controller stuff

   // int left = con.Axis3.position()+con.Axis4.position();
  //  int right = con.Axis3.position()-con.Axis4.position();
    driveAmt = exponetial(con.Axis3.value());
   turnAmt = exponetial(con.Axis4.value());
    
    fLDrive.spin(fwd, driveAmt - turnAmt,percent);
    fRDrive.spin(fwd,driveAmt + turnAmt,percent);
    mLDrive.spin(fwd,driveAmt + turnAmt,percent);
    mRDrive.spin(fwd,driveAmt - turnAmt,percent);
    bLDrive.spin(fwd,driveAmt + turnAmt ,percent);
    bRDrive.spin(fwd,driveAmt - turnAmt ,percent);
   /* fLDrive.spin(fwd, left,percent);
    fRDrive.spin(fwd,right,percent);
    mLDrive.spin(fwd,left,percent);
    mRDrive.spin(fwd,right,percent);
    bLDrive.spin(fwd,left ,percent);
    bRDrive.spin(fwd,right,percent);*/
    fLDrive.setBrake(brake);
    fRDrive.setBrake(brake);
    mLDrive.setBrake(brake);
    mRDrive.setBrake(brake);
    bLDrive.setBrake(brake);
    bRDrive.setBrake(brake);
    
    //check for loaded
    if (!drew.pressing()) {
      loaded=!loaded;
    } else if (drew.pressing()) {
      loaded = true;
      active=true;

    } 
    
    //expantion
    if(con.ButtonX.pressing()){
      expantion1.open();
      expantion2.open();
    }

    //toggle Auto load
    if (con.ButtonR2.pressing()) {
      if (c1) {
        c1 = false;
        tog1 = !tog1;
      }
    } else {
      c1 = true;
    }
    //fail safe intake
     if (con.ButtonL1.pressing()) {
      if (c2) {
        c2 = false;
        tog2 = !tog2;
      }
    } else {
      c2 = true;
    }
    //fail safe intake
    if (tog2){
      intakeL.spin(reverse, 100,pct);
      intakeR.spin(reverse, 100,pct);     
    }else{
      intakeL.stop();
      intakeR.stop();
    }


    //Intake motor contro
    if (con.ButtonR1.pressing()) {
      intakeL.spin(fwd, 100,pct);
      intakeR.spin(fwd, 100, pct);
      active= false;
    } else {
      if (tog1) {
        if(!loaded){  //////
          if(!active){
            intakeL.spin(fwd, 100,pct);
            intakeR.spin(fwd, 100,pct);
          }
        } else{
          intakeL.spin(reverse, 100,pct);
          intakeR.spin(reverse, 100,pct);     
        }
      } else if(con.ButtonL1.pressing()){
      
        intakeL.stop();
        intakeR.stop();
      }
    }

    printf("%f\n", rota.position(deg));
    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  vexcodeInit();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
/*MMMMMMMMMMMMMMMMMMMMMNXK000KXNWWMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMNOl,'.....'';cokXWMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMNk;...''''''''....'cONMMMMMMMMMMMM
MMMMMMMMMMMMMMMXl...,,,,,,,,,,,,,'...c0WMMMMMMMMMM
MMMMMMMMMMMMMMXc..',,,,,'''.........  ,OWMMMMMMMMM
MMMMMMMMMMMMMNo..',,,'......'',,,;,,'..'l0WMMMMMMM
MMMMMMMMMMMMMk. .,,,'. .:odkkO0XNNNNXKkc'.oNMMMMMM
MMMMMMMMMWWN0; .',,,. .,coxOOO0KNNWWWWNKx'.xWMMMMM
MMMMMW0dl:,'.  .',,,. .,;;cxkOOOO00000OOx; :XMMMMM
MMMMWk. ..... ..',,,. .';;;:cooodddooolc:. :XMMMMM
MMMMK; .''''. ..',,,'. ..'',;;;;;;;;;;;,'..kWMMMMM
MMMWd...'...  ..',,,,,.................. .kWMMMMMM
MMMNc ......  ..',,,,,,,,''............. :XMMMMMMM
MMMX; ......  ..'',,,,,,,,,,,,,,,,,,,,,. cNMMMMMMM
MMMK; ...... ....',,,,,,,,,,,,,,,,,,,,,..oWMMMMMMM
MMMX; ...... ....'',,,,,,,,,,,,,,,,,,,'..dMMMMMMMM
MMMN: ...... ......'',,,,,,,,,,,,,,,,,...kMMMMMMMM
MMMWl ......  ......''',,,,,,,,,,,,,''. ,0MMMMMMMM
MMMMx.......  .........'''''''''''''... :XMMMMMMMM
MMMMK, ...... ......................... oWMMMMMMMM
MMMMWO;.....  ........................ .kMMMMMMMMM
MMMMMMNKOkxkc........... .,oc. ....... ,KMMMMMMMMM
MMMMMMMMMMMMO' ..........'xMX; ....... cNMMMMMMMMM
MMMMMMMMMMMMX: ........ .;kMNl ........xMMMMMMMMMM
MMMMMMMMMMMMWx. ......  ,l0MWx. ......:KMMMMMMMMMM
MMMMMMMMMMMMMN0kdooloodkKNWMMNOkkxxkk0XWMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMhi
*/
