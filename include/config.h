#ifndef CONFIG_H
#define CONFIG_H

// constants
#define KB_IN_MB 1000.0
#define MS_IN_SEC 1000.0

// configurable parameters
#define UPDATE_INTERVAL 1000 // integral number of milliseconds; must be > 0
#define PROCESSOR_UTILIZATION_DELAY (UPDATE_INTERVAL) // integral number of milliseconds; must be > 0
#define USER_INPUT_DELAY (UPDATE_INTERVAL/2) // integral number of milliseconds; recommend that this is < UPDATE_INTERVAL (with ample margin) but that is not required...; must be > 0

#endif
