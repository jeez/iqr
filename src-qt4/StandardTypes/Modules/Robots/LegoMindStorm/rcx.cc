/******************************************************************************

rcx module
kynan Jul 2001

Interface to Lego RCX brick

******************************************************************************/


#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <pthread.h>

#include "rcx.h"


// This macro checks the module version at runtime
#ifdef IQR_MODULE_VERSION
IQR_MODULE_VERSION
#endif


//#define TEST_COND 1

#ifdef TEST_COND
#define TEST_COND_TIMEOUT 5
extern pthread_cond_t next_cycle;
extern pthread_mutex_t next_cycle_mutex;
struct timeval now;
struct timezone tz = { 0, 0 };
struct timespec timeout;
int retcode;
#endif

extern int simCycles;
extern cpt *cell;

int fd;                                   // Serial port
int sensor_type[MAX_SENSOR];              // Sensor types
int sensor_mode[MAX_SENSOR];              // Sensor modes


// Returns a sensor type number depending on the input parameter string
// If unknown, then raw is returned


rcx::rcx()
{
  int i = 0;
  cycles = 0;
  for (i = 0; i < MAX_SENSOR; i++)
  {
    sensor_type[i] = SENSOR_RAW_ID;
    sensor_mode[i] = MODE_RAW_ID;
  }
}


rcx::~rcx()
{
  fprintf(stderr, "RCX: Setting all motors to float\n"); 
  rcx_set_motors_state(fd, PAR_SET_MOTORS_FLOAT | 0x07);
  fprintf(stderr, "RCX: Stopping RCX...\n");
  rcx_close(fd);
  cancel();
}


void
rcx::process()
{
  int i, j, k;                                    // Loop counters
  static int first_time = 1;                      // First invocation flag
  static int this_control[MAX_MOTOR][MIN_CONTROL_SIZE];    // This control mask
  static int prev_control[MAX_MOTOR][MIN_CONTROL_SIZE];    // Prev control mask
  static int this_power[MAX_MOTOR];               // Current power level
  static int prev_power[MAX_MOTOR];               // Prev power level

#ifdef TEST_COND
  pthread_mutex_lock(&next_cycle_mutex);
  gettimeofday(&now, &tz);
  timeout.tv_sec = now.tv_sec + TEST_COND_TIMEOUT;
  timeout.tv_nsec = now.tv_usec * 1000; 
  retcode = pthread_cond_timedwait(&next_cycle, &next_cycle_mutex, &timeout);
  // Code should go in here to check for (retcode == ETIMEDOUT) but it doesn't
  // exist in RedHat yet
  pthread_mutex_unlock(&next_cycle_mutex);
  cycles = simCycles - 1;
#endif

  if (cycles < simCycles)
  {
    // Initialisation
    if (first_time)
    {
      for (i = 0; i < MAX_MOTOR; i++)
      {
        prev_power[i] = 0;
        for (j = 0; j < MIN_CONTROL_SIZE; j++)
        {
          prev_control[i][j] = 0;
        }
      }
      first_time = 0;
    }


    // Update status


    // Update motor control
    int motor_command = 0;
    int motor_dir = 0;
    for (i = 0; i < MAX_MOTOR; i++) {
      for (j = 0; j < MIN_CONTROL_SIZE; j++) {     // No initial control signal
        this_control[i][j] = 0;
      }
      for (j = 0; j < cgPtr[i]->size; j++) {        // Update control activities
        csPtr[i][j] = (cgPtr[i]->pot[j] > ccPtr[i]->th) ? ccPtr[i]->slope : 0.0;
      }

      // Turn ON motor if on cell above threshold
      if (cgPtr[i]->pot[CONTROL_ON] > ccPtr[i]->th) {
        motor_command |= (PAR_SET_MOTORS_ON | (0x01 << i));
        this_control[i][CONTROL_ON] = 1;
      }
      // Turn OFF motor if off cell above threshold
      if (cgPtr[i]->pot[CONTROL_OFF] > ccPtr[i]->th) {
        motor_command |= (PAR_SET_MOTORS_OFF | (0x01 << i));
        this_control[i][CONTROL_OFF] = 1;
      }
      // Float motor if float cell above threshold
      if (cgPtr[i]->pot[CONTROL_FLOAT] > ccPtr[i]->th) {
        motor_command |= (PAR_SET_MOTORS_FLOAT | (0x01 << i));
        this_control[i][CONTROL_FLOAT] = 1;
      }

      // Fwd direction if fwd cell above threshold
      if (cgPtr[i]->pot[CONTROL_FWD] > ccPtr[i]->th) {
        motor_dir |= (PAR_SET_MOTORS_FWD | (0x01 << i));
        this_control[i][CONTROL_FWD] = 1;
      }
      // Rev direction if rev cell above threshold
      if (cgPtr[i]->pot[CONTROL_REV] > ccPtr[i]->th) {
        motor_dir = (PAR_SET_MOTORS_REV | (0x01 << i));
        this_control[i][CONTROL_REV] = 1;
      }
      // Flip direction if flip cell above threshold
      if (cgPtr[i]->pot[CONTROL_FLIP] > ccPtr[i]->th) {
        motor_dir |= (PAR_SET_MOTORS_FLIP | (0x01 << i));
        this_control[i][CONTROL_FLIP] = 1;
      }

      // Set motor control
      int motor_control_changed = 0;
      int motor_dir_changed = 0;
      if ((this_control[i][CONTROL_ON]    != prev_control[i][CONTROL_ON])   ||
          (this_control[i][CONTROL_OFF]   != prev_control[i][CONTROL_OFF])  ||
          (this_control[i][CONTROL_FLOAT] != prev_control[i][CONTROL_FLOAT]))
      {
        motor_control_changed = 1;
      }
      if ((this_control[i][CONTROL_FWD]   != prev_control[i][CONTROL_FWD])  ||
          (this_control[i][CONTROL_REV]   != prev_control[i][CONTROL_REV])  ||
          (this_control[i][CONTROL_FLIP]  != prev_control[i][CONTROL_FLIP]))
      {
        motor_dir_changed = 1;
      }
      if (motor_control_changed) {
        fprintf(stderr, "RCX: Motor command 0x%X\n", motor_command);
        rcx_set_motors_state(fd, motor_command);
      }
      if (motor_dir_changed) {
        fprintf(stderr, "RCX: Motor direction 0x%X\n", motor_dir);
        rcx_set_motors_direction(fd, motor_dir);
      }
      // Remember previous values of motor control
      if (motor_control_changed || motor_dir_changed) {
        for (j = 0; j < MIN_CONTROL_SIZE; j++)
        {
          prev_control[i][j] = this_control[i][j];
        }
      }
    }


    // Update motor power
    for (i = 0; i < MAX_MOTOR; i++) {
      for (j = 0; j < cgPtr[i]->size; j++) {         // Update power activities
        psPtr[i][j] = (pgPtr[i]->pot[j] > pcPtr[i]->th) ? pcPtr[i]->slope : 0.0;
      }
 
      // Do WTA on power cell group
      this_power[i] = 0;
      for (j = 0; j < pgPtr[i]->size; j++) {          // Find highest input cell
        if (psPtr[i][j] > pcPtr[i]->th) {            // Must be above threshold
          if (pgPtr[i]->pot[j] > pgPtr[i]->pot[this_power[i]]) {  // Compare
            this_power[i] = j;
          }
        }
      }
    }

    // Update power for each motor if it has changed and remember it
    for (i = 0; i < MAX_MOTOR; i++) {
      if ((this_power[i] != prev_power[i]) && (this_power[i] <= MAX_MOTORPOWER)) {
        int motor_id = 0x01 << i;
        fprintf(stderr, "RCX: Motor power 0x%X 0x%X\n", motor_id, this_power[i]);
        rcx_set_motors_power(fd, motor_id, 2, this_power[i]);
        prev_power[i] = this_power[i];
      }
    }

    // Update sensors
    for (i = 0; i < MAX_SENSOR; i++) {
      int raw_value = (int) rcx_get_value(fd, sensor_mode[i], i);
      for (j = 0; j < sgPtr[i]->size; j++) {         // Set sensor values to 0
        ssPtr[i][j] = 0.0;
      }
      ssPtr[i][raw_value] = scPtr[i]->slope;
    }

    cycles = simCycles;
  }
}


void
rcx::run()
{
  Module::run();
}


// Initialise module parameters
void
rcx::setParameters(char *parms)
{
  int  argc    = 0;
  char **argv  = NULL;
  char s[100];
  char *ps     = parms;
  int  i, j, k;
  int  io_set;
     
  if (!parms) return;

  fprintf(stderr, "rcx::setParameters\n");

  j = 0;
  k = 0;
  for (i = 0; i <= strlen(parms); i++)
  {
    if ((parms[i] == '&') || (i == strlen(parms)))
    {
      strncpy(s, &parms[j], i-j);
      s[i-j] = 0;
      fprintf(stderr, "rcx::setParameters %i %s\n", k, s);
      j = i + 1;
      strcpy(grpName, s);

      switch(k)
      {
        // Status cell group
        case 0:
          if (tgPtr = grp_name2grp_ptr(grp, grpName, nGroups))
          {
            tcPtr = &(cell[tgPtr->cell_t]);
            tsPtr = tgPtr->a;
            fprintf(stderr, "rcx: Status group connected to [%s-%s]\n",
                 tgPtr->name, tcPtr->name);
          }
          else
          {
            fprintf(stderr, "rcx: Error connecting status group [%s]\n", grpName);
          }
          break;

        // Control cell groups
        case 3:
          io_set = k - 1;
          if (cgPtr[io_set] = grp_name2grp_ptr(grp, grpName, nGroups))
          {
            ccPtr[io_set] = &(cell[cgPtr[io_set]->cell_t]);
            csPtr[io_set] = cgPtr[io_set]->a;
            fprintf(stderr, "rcx: Control group %d connected to [%s-%s]\n",
                 io_set, cgPtr[io_set]->name, ccPtr[io_set]->name);
          }
          else
          {
            fprintf(stderr, "rcx: Error connecting control group [%s]\n", grpName);
          }
          break;

        // Power cell groups
        case 6:
          io_set = k - 4;
          if (pgPtr[io_set] = grp_name2grp_ptr(grp, grpName, nGroups))
          {
            pcPtr[io_set] = &(cell[pgPtr[io_set]->cell_t]);
            psPtr[io_set] = pgPtr[io_set]->a;
            fprintf(stderr, "rcx: Power group %d connected to [%s-%s]\n",
                 io_set+1, pgPtr[io_set]->name, pcPtr[io_set]->name);
          }
          else
          {
            fprintf(stderr, "rcx: Error connecting control group [%s]\n", grpName);
          }
          break;

        // Sensor cell groups
        case 9:
          io_set = k - 7;
          if (sgPtr[io_set] = grp_name2grp_ptr(grp, grpName, nGroups))
          {
            scPtr[io_set] = &(cell[sgPtr[io_set]->cell_t]);
            ssPtr[io_set] = sgPtr[io_set]->a;
            fprintf(stderr, "rcx: Sensor group %d connected to [%s-%s]\n",
                 io_set+1, sgPtr[io_set]->name, scPtr[io_set]->name);
          }
          else
          {
            fprintf(stderr, "rcx: Error connecting control group [%s]\n", grpName);
          }
          break;

        // Sensor type parameters
        case 12:
          io_set = k - 10;
          sensor_type[io_set] = iSensorType;
          fprintf(stderr, "rcx: Parse sensor %d type %d [%s]\n", io_set+1, sensor_type[io_set], grpName);
          break;

        // Sensor mode parameters
        case 15:
          io_set = k - 13;
          sensor_mode[io_set] = iSensorMode;
          fprintf(stderr, "rcx: Parse sensor %d mode %d [%s]\n", io_set+1, sensor_mode[io_set], grpName);
          break;

        default:
          fprintf(stderr, "rcx: Invalid cell group %d [%s]\n", k, grpName);
          break;
      }
      k++;
    }
  }

}


extern "C" Module *createModule()
/* extern "C" prevents C++ name mangling */
{
  return (Module*) new rcx();
}

