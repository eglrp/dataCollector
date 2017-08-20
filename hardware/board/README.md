GPIO for RK chip
======

Definitions
------
        
        #define GPIO_BANK0              (0 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK0              (0 << RK_GPIO_BANK_OFFSET)                                                                                                             
        #define GPIO_BANK1              (1 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK2              (2 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK3              (3 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK4              (4 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK5              (5 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK6              (6 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK7              (7 << RK_GPIO_BANK_OFFSET)
        #define GPIO_BANK8              (8 << RK_GPIO_BANK_OFFSET)
        
        #define RK_GPIO_BANK_OFFSET   8
        
        #define GPIO_A0                 0
        #define GPIO_A1                 1
        #define GPIO_A2                 2
        #define GPIO_A3                 3
        #define GPIO_A4                 4
        #define GPIO_A5                 5
        #define GPIO_A6                 6
        #define GPIO_A7                 7
        #define GPIO_B0                 8
        #define GPIO_B1                 9
        #define GPIO_B2                 10
        #define GPIO_B3                 11
        #define GPIO_B4                 12
        #define GPIO_B5                 13
        #define GPIO_B6                 14
        #define GPIO_B7                 15
        #define GPIO_C0                 16
        #define GPIO_C1                 17
        #define GPIO_C2                 18
        #define GPIO_C3                 19
        #define GPIO_C4                 20
        #define GPIO_C5                 21
        #define GPIO_C6                 22
        #define GPIO_C7                 23
        #define GPIO_D0                 24
        #define GPIO_D1                 25
        #define GPIO_D2                 26
        #define GPIO_D3                 27
        #define GPIO_D4                 28
        #define GPIO_D5                 29
        #define GPIO_D6                 30
        #define GPIO_D7                 31

Example
------
如gpio7 GPIO_B4就是(7 << RK_GPIO_BANK_OFFSET) | GPIO_D2
简单点的写法就是7x32+12=236

GPIO7_B4=236   //M180_V3 << SPICLLK

GPIO7_B5=237   //M180_V3 << SPICSN

shell  测试GPIO输出驱动

        echo 236 > /sys/class/gpio/export             //打开指定GPIO
        echo out > /sys/class/gpio/gpio236/direction  //修改为输出
        echo 1 >/sys/class/gpio/gpio236/value         //写入输出值  0 或者1

Warning!
------
RK3288  GPIO 输出驱动电流能力有限，请知悉！！！max_current  <=100MA
