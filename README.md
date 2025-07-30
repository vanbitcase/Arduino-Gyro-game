# MPU6050 OLED Gravity Game

A simple and fun avoidance game for Arduino, using an MPU6050 accelerometer to control the player on a 128x32 OLED display.

---

## How to Play

* **Objective:** Dodge the incoming rectangular obstacles for as long as you can.
* **Controls:** Tilt the MPU6050 sensor to move your player (the small solid square) around the screen.
* **Losing:** The game ends if your player collides with any of the obstacles. To play again, you'll need to reset your Arduino board.

---

## Hardware Required

1.  An Arduino-compatible board (e.g., Arduino Nano, Uno).
2.  MPU6050 Accelerometer & Gyroscope Module.
3.  SSD1306 128x32 I2C OLED Display.
4.  Jumper wires and a breadboard.

---

## Customization

You can easily modify the game's difficulty by changing a few variables at the top of the code.

### 1. Change Player & Obstacle Size

You can adjust the size of the player and the obstacles to make the game easier or harder.

* **Player Size:** Find the `playerSize` variable and change its value. A larger number makes your player bigger.
    ```cpp
    // Player properties
    int playerSize = 5; // Change this value
    ```
* **Obstacle Size:** In the `Obstacle` structure, you can change the `w` (width) and `h` (height) values.
    ```cpp
    struct Obstacle {
      int x, y;
      int w = 8, h = 8; // Change these values
      // ...
    };
    ```

### 2. Change the Number of Obstacles

You can control how many obstacles appear on the screen at the same time.

* **Max Obstacles:** Find the `MAX_OBSTACLES` constant and change its value. A higher number will make the game more challenging by cluttering the screen.
    ```cpp
    const int MAX_OBSTACLES = 3; // Change this value
    
