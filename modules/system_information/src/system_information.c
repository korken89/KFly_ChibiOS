/* *
 *
 *
 *
 * */

#include "ch.h"
#include "hal.h"
#include "system_information.h"
#include "flash_save.h"
#include "arming.h"
#include "computer_control.h"
#include <string.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/** @brief  Location for the unique hardware ID. */
#define UNIQUE_ID (uint8_t *)(0x1fff7a10)

/** @brief  Base address for the bootloader in flash. */
#define BOOTLOADER_BASE 0x08000000

/** @brief  Offset from the FIRMWARE_BASE to the software version string. */
#define SW_VERSION_OFFSET 0x1c0

/** @brief  Date definition. */
#ifndef DATE
#define DATE "no timestamp"
#endif

/** @brief  Version definition. */
#ifndef GIT_VERSION
#define GIT_VERSION "no version"
#endif

#define FUNCTION_NAME(name) #name
#define STR(macro) FUNCTION_NAME(macro)
#define GIT_DATE_NAME STR(DATE)
#define GIT_VERSION_NAME STR(GIT_VERSION)

/** @brief  Definition of the version string. */
#define KFLY_VERSION "v" GIT_VERSION_NAME ", Build date: " GIT_DATE_NAME "\0"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/** @brief  Save location for the KFly Version string for bootloader access. */
__attribute__((used, section(".sw_version"))) const char kfly_build_version[] =
    KFLY_VERSION;

/*===========================================================================*/
/* Module local variables and types.                                         */
/*===========================================================================*/

/** @brief   System information structure. */
static system_information_t system_information;

/** @brief  Work area for the flash save. */
static THD_WORKING_AREA(waThreadSysInfoFlashSave, 256);

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief           Thread for the flash save operation.
 *
 * @param[in] arg   Unused.
 * @return          Unused.
 */
static THD_FUNCTION(ThreadSysInfoFlashSave, arg)
{
  (void)arg;

  /* Event registration for new estimation */
  event_listener_t el;

  /* Set thread name */
  chRegSetThreadName("SysInfo FlashSave");

  /* Register to new estimation */
  chEvtRegisterMask(ptrGetFlashSaveEventSource(), &el,
                    FLASHSAVE_SAVE_EVENTMASK);

  while (1)
  {
    /* Wait for new estimation */
    chEvtWaitOne(FLASHSAVE_SAVE_EVENTMASK);

    /* Save settings to flash */
    FlashSave_Write(FlashSave_STR2ID("SIVN"), true,
                    (uint8_t *)system_information.vehicle_name,
                    VEHICLE_NAME_SIZE);

    FlashSave_Write(FlashSave_STR2ID("SIVT"), true,
                    (uint8_t *)system_information.vehicle_type,
                    VEHICLE_TYPE_SIZE);
  }
}

/**
 * @brief   Sets the system information structure to sane values.
 */
static void SystemInformationReset(void)
{
  /*========================*/
  /* Identification         */
  /*========================*/

  strncpy(system_information.vehicle_name, "no name", VEHICLE_NAME_SIZE);
  strncpy(system_information.vehicle_type, "no type", VEHICLE_TYPE_SIZE);
  memcpy(system_information.unique_id, UNIQUE_ID, UNIQUE_ID_SIZE);
  strncpy(system_information.kfly_version, KFLY_VERSION, KFLY_VERSION_SIZE);

  /*========================*/
  /* General info           */
  /*========================*/

  system_information.flight_time = -1;
  system_information.up_time     = -1;
  system_information.cpu_usage   = -1;

  /*========================*/
  /* Hardware info          */
  /*========================*/

  system_information.battery_voltage                = -1;
  system_information.motors_armed.value             = false;
  system_information.in_air.value                   = false;
  system_information.serial_interface_enabled.value = false;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initialized the system information structures.
 */
void SystemInformationInit()
{
  /* Start with safe values */
  SystemInformationReset();

  /* Read settings from flash */
  FlashSave_Read(FlashSave_STR2ID("SIVN"),
                 (uint8_t *)system_information.vehicle_name, VEHICLE_NAME_SIZE);

  FlashSave_Read(FlashSave_STR2ID("SIVT"),
                 (uint8_t *)system_information.vehicle_type, VEHICLE_TYPE_SIZE);

  /* Start the Flash Save thread */
  chThdCreateStatic(waThreadSysInfoFlashSave, sizeof(waThreadSysInfoFlashSave),
                    NORMALPRIO, ThreadSysInfoFlashSave, NULL);
}

/**
 * @brief             Copies the system information structure to a destination.
 *
 * @param[out] dest   Destination address.
 */
void GetSystemInformation(system_information_t *dest)
{
  osalSysLock();

  /* Fill in system parameters. */

  system_information.flight_time = -1;  // For future use
  system_information.up_time = ((float)ST2US(chVTGetSystemTime())) / 1000000.0f;
  system_information.cpu_usage = -1;  // For future use

  system_information.battery_voltage    = -1;  // For future use
  system_information.motors_armed.value = bIsSystemArmed();
  system_information.in_air.value       = true;  // For future use
  system_information.serial_interface_enabled.value =
      ComputerControlLinkActive();

  /* Copy the system information structure to its destination. */
  memcpy(dest, &system_information, sizeof(system_information_t));

  osalSysUnlock();
}

/**
 * @brief           Sets the vehicle name and vehicle type in the system
 *                  information structure.
 *
 * @param[in] name  New name array.
 * @param[in] type  New type array.
 */
void SetSystemNameType(const char name[VEHICLE_NAME_SIZE],
                       const char type[VEHICLE_TYPE_SIZE])
{
  osalSysLock();

  strncpy(system_information.vehicle_name, name, VEHICLE_NAME_SIZE);
  strncpy(system_information.vehicle_type, type, VEHICLE_TYPE_SIZE);

  osalSysUnlock();
}