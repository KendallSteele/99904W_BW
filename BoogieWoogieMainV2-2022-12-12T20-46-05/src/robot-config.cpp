#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller con;
motor fLDrive = motor(PORT8,false);
motor fRDrive = motor(PORT7,true);
motor mLDrive = motor(PORT13,true);
motor mRDrive = motor(PORT16,false);
motor bLDrive = motor(PORT15,true);
motor bRDrive = motor(PORT20,false);
motor intakeL = motor(PORT9,true);
motor intakeR = motor(PORT3,false);
rotation rota = rotation(PORT6);
pneumatics expantion1 = pneumatics(Brain.ThreeWirePort.B);
pneumatics expantion2 = pneumatics(Brain.ThreeWirePort.A);
inertial christianPulisic = inertial(PORT10);
optical tylerAdams  = optical(PORT5, false);
limit drew = limit(Brain.ThreeWirePort.C);


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {

}