# Team MEMORY LEAK - Kaspersky OS hackathon

Monolithic application to controle AlphaBot robot.

[kaspersky os documentation](https://docs.kasperskyos.com/)

```
.
├── CMakeLists.txt
├── cross-build.sh
├── einit
│   ├── CMakeLists.txt
│   └── src
│       ├── init.yaml.in
│       ├── security.psl
│       └── trusted.psl
├── README.md
├── resources
│   ├── edl
│   │   └── Subscriber.edl
│   ├── ramfs
│   │   └── etc
│   │       ├── dhcpcd.conf
│   │       ├── hosts
│   │       └── ntp.conf
│   └── sdcard
│       └── etc
│           ├── dhcpcd.conf
│           ├── hosts
│           └── ntp.conf
├── SETUP.md
└── subscriber
    ├── CMakeLists.txt
    └── src
        ├── general.h
        ├── gpio.cpp
        ├── gpiomodule.cpp
        ├── gpiomodule.h
        ├── main.cpp
        ├── subscriber.cpp
        └── subscriber.h
```    

All source code is located in `subscriber` directory.

## main.cpp
`main.cpp` - entry point of the application.

## subscriber.cpp
`subscriber.cpp` - MQTT subscriber implementation. It subscribes to `test` topic to reveive messages from the broker. It then sends messages to the `gpio` module to control the robot.

## gpio.cpp
`gpio.cpp` - GPIO implementation. It is used to control the robot. This module parses messages in format `direction:duration' and controls the robot accordingly.

## gpiomodule.cpp
`gpiomodule.cpp` - GPIO module. It is used to initialize the GPIO pins and to control the robot.


# Build instructions

First you need to install the Kaspersky OS SDK. You can find the instructions [here](https://docs.kasperskyos.com/).

Then you need to build the application. You can do it by running the following command in the root directory of the project:

If you are building the application for embedded device, you need to change `sim` to `kos-image` in the `cross-build.sh` script.

```
./cross-build.sh
```
It will create `build` directory and put the application there.

Developers: [GaiusJuliusCaesar38](https://github.com/GaiusJuliusCaesar38), [muratsat](https://github.com/muratsat)

If you have any questions, feel free to contact us.