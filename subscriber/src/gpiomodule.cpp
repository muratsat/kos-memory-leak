#include <gpio/gpio.h>
#include <kos_net.h>
#include <platform/platform.h>
#include <stdbool.h>

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>

#if defined(__arm__) || defined(__aarch64__)
#include <bsp/bsp.h>
#endif
#include <rtl/countof.h>
#include <rtl/retcode_hr.h>

#include "gpiomodule.h"

#define GPIO_PIN_NUM RTL_UINT32_C(28)
#define DELAY_S 2
#define HW_MODULE_NAME "gpio0"
#define HW_MODULE_CFG "raspberry_pi4b.default"
#define GPIO_HIGH_LEVEL 1
#define GPIO_LOW_LEVEL 0

#define GPIO_PIN_UART_TX 14
#define GPIO_PIN_UART_RX 15

extern void print(const char* msg, const char* prefix = nullptr);

/**
 * ExceptionPinArr is an array of GPIO pin numbers that should be excluded
 * from the example. This may be necessary if some of the pins are already
 * used for other functions, for example, if the pins are used for UART
 * connections during debugging.
 */
static const rtl_uint32_t ExceptionPinArr[] = {GPIO_PIN_UART_TX,
                                               GPIO_PIN_UART_RX};

static bool IsExceptionPin(const rtl_uint32_t pin) {
  bool isExceptionPin = false;

  for (size_t i = 0; i < rtl_countof(ExceptionPinArr); i++) {
    if (ExceptionPinArr[i] == pin) {
      isExceptionPin = true;
      break;
    }
  }

  return isExceptionPin;
}

int initGpioModule() {
  Retcode rc = rcFail;

  /**
   * Initialize the board support package (BSP) driver and set configuration
   * for GPIO pins. It is required for stdout by UART.
   */
  rc = BspInit(NULL);
  if (rc != BSP_EOK) {
    print("Failed to initialize BSP, error code:\n");
    printf("Failed to initialize BSP rc = " RETCODE_HR_FMT "\n",
           RETCODE_HR_PARAMS(rc));
    fprintf(stderr, "Failed to initialize BSP, error code: %d.\n",
            RC_GET_CODE(rc));
    return rc;
  }

  if (rcOk == rc) {
    print("Try init mux:\n");

    rc = BspSetConfig(HW_MODULE_NAME, HW_MODULE_CFG);
    if (rcOk != rc) {
      print("Init MUX Fail\n");

      fprintf(stderr,
              "Failed to set mux configuration for %s module, "
              "error code: %d.\n",
              HW_MODULE_NAME, RC_GET_CODE(rc));
      return rc;
    } else {
      print("Init MUX OK\n");
    }
  }

  /* Initialize the GPIO. */
  if (rcOk == rc) {
    rc = GpioInit();
    if (rcOk != rc) {
      print("Try init GPIO..\n");
      fprintf(stderr, "GpioInit failed, error code: %d.\n", RC_GET_CODE(rc));
      return rc;
    } else {
      print("Init GPIO OK\n");
    }
  }
  return rcOk;
}

#define GPIO_PIN_NUM_IN2 12U
#define GPIO_PIN_NUM_IN1 13U
#define GPIO_PIN_NUM_ENA 6U
#define GPIO_PIN_NUM_IN4 20U
#define GPIO_PIN_NUM_IN3 21U
#define GPIO_PIN_NUM_ENB 26U
#define DELAY_S 2

#define BUF_SIZE 1024
#define HIGH 1
#define LOW 0

void forward(GpioHandle* handle) {
  fprintf(stderr, "forward\n");
  GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void stop(GpioHandle* handle) {
  GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, LOW);
}

void backward(GpioHandle* handle) {
  fprintf(stderr, "backward\n");
  GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void left(GpioHandle* handle) {
  fprintf(stderr, "left\n");
  GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void right(GpioHandle* handle) {
  fprintf(stderr, "right\n");
  GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void gpioSetPortMode(GpioHandle* handle) {
  GpioSetMode(handle, GPIO_PIN_NUM_IN1, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN2, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN3, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN4, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_ENA, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_ENB, GPIO_DIR_OUT);
}

void trajectory(GpioHandle* handle) {
  forward(handle);
  sleep(3);
  usleep(100000);
  left(handle);
  usleep(350000);
  forward(handle);
  sleep(1);
  usleep(700000);
  left(handle);
  usleep(350000);
  forward(handle);
  sleep(3);
  usleep(100000);
  left(handle);
  usleep(350000);
  forward(handle);
  usleep(700000);
  stop(handle);
}

void closePort(GpioHandle* handle) {
  Retcode rc = rcFail;
  /* Close GPIO port. */
  if (GPIO_INVALID_HANDLE != handle) {
    Retcode tempRc = GpioClosePort(handle);
    if (rcOk != tempRc) {
      fprintf(stderr, "GpioClosePort for %s port failed, error code: %d.\n",
              HW_MODULE_NAME, RC_GET_CODE(tempRc));
      rc = tempRc;
    }
  }
}

GpioHandle handle = GPIO_INVALID_HANDLE;

void trajectoryCmd() { trajectory(&handle); }

int gpioInitAll(void) {
  int returnValue = EXIT_FAILURE;
  Retcode rc = rcFail;
  //    GpioHandle handle = GPIO_INVALID_HANDLE;

  print("Start GPIO_output test.\n");

  rc = initGpioModule();

  /* Initialize and setup HW_MODULE_NAME port. */
  if (rcOk == rc) {
    rc = GpioOpenPort(HW_MODULE_NAME, &handle);
    if (rcOk != rc) {
      fprintf(stderr, "GpioOpenPort port %s failed, error code: %d.\n",
              HW_MODULE_NAME, RC_GET_CODE(rc));
    } else if (GPIO_INVALID_HANDLE == handle) {
      fprintf(stderr, "GPIO module %s handle is invalid.\n", HW_MODULE_NAME);
      rc = rcFail;
    }
  }

  gpioSetPortMode(&handle);
  //    trajectory(&handle);
  //    closePort(&handle);   // Fix Me !!!!!!!!!!!!
  return rcOk;
}
