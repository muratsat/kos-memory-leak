#include <bits/stdc++.h>
#include <gpio/gpio.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#ifdef __arm__
#include <bsp/bsp.h>
#endif

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

/* void connect() {
    int sock;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
    }
} */

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
  GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}
void right(GpioHandle* handle) {
  fprintf(stderr, "right\n");
  GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
  GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
  GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

std::pair<std::string, std::string> Split(const std::string& s) {
  std::string s1;
  std::string s2;

  int delim = 0;

  for (int i = 0; i < s.size(); i++) {
    if (s[i] == ',') {
      delim = i;
      break;
    } else {
      s1 += s[i];
    }
  }

  for (int i = delim + 1; i < s.size(); i++) {
    s2 += s[i];
  }

  return {s1, s2};
}

int gpiomain(const std::string& command) {
  fprintf(stderr, "Start GPIO_output test\n");

#ifdef __arm__
  /**
   * InitializeGPIO_PIN_NUM board support package (BSP) driver and set
   * configuration for GPIO pins. It`s required for stdout by UART.
   */
  {
    BspError rc = BspInit(NULL);
    if (rc != BSP_EOK) {
      fprintf(stderr, "Failed to initialize BSP\n");
      return EXIT_FAILURE;
    }

    rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
    if (rc != BSP_EOK) {
      fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
      return EXIT_FAILURE;
    }
  }
#endif

  if (GpioInit()) {
    fprintf(stderr, "GpioInit failed\n");
    return EXIT_FAILURE;
  }

  /* initialize and setup gpio0 port */
  GpioHandle handle = NULL;
  if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE) {
    fprintf(stderr, "GpioOpenPort failed\n");
    return EXIT_FAILURE;
  }

  GpioSetMode(handle, GPIO_PIN_NUM_IN1, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN2, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN3, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_IN4, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_ENA, GPIO_DIR_OUT);
  GpioSetMode(handle, GPIO_PIN_NUM_ENB, GPIO_DIR_OUT);

  fprintf(stderr, "Starting move\n");
  GpioHandle* p_handle = &handle;

  auto p = Split(command);

  std::string cmd = p.first;
  int value = 1;
  if (!p.second.empty()) {
    value = std::stoi(p.second);
  }

  std::cout << cmd << ' ' << value << '\n';

  if (cmd == "forward") {
    forward(p_handle);
    sleep(value);
    stop(p_handle);
  }
  if (cmd == "left") {
    left(p_handle);
    sleep(value);
    stop(p_handle);
  }
  if (cmd == "right") {
    right(p_handle);
    sleep(value);
    stop(p_handle);
  }
  if (cmd == "stop") {
    stop(p_handle);
  }

  if (cmd == "back") {
    backward(p_handle);
    sleep(value);
    stop(p_handle);
  }

  if (GpioClosePort(handle)) {
    fprintf(stderr, "GpioClosePort failed\n");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "Test finished.\n");

  return EXIT_SUCCESS;
}
