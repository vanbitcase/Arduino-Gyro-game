#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Create instances for the sensor and the display
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

// --- Game Variables ---

// Player properties
int playerX, playerY;
int playerSize = 5;

// Game state
bool gameOver = false;

// Obstacle structure
struct Obstacle {
  int x, y;
  int w = 3, h = 3; // Obstacle size
  int speedX, speedY;
  bool active = false;
};

const int MAX_OBSTACLES = 2;
Obstacle obstacles[MAX_OBSTACLES];

// --- Game Functions ---

void spawnNewObstacle() {
  // Find an inactive obstacle slot to reuse
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {
      obstacles[i].active = true;
      
      // Randomly decide if the obstacle moves on the X or Y axis
      if (random(0, 2) == 0) { // X-axis movement
        obstacles[i].y = random(0, display.height() - obstacles[i].h);
        obstacles[i].speedY = 0;
        // Start from left or right edge
        if (random(0, 2) == 0) {
          obstacles[i].x = 0;
          obstacles[i].speedX = 1; // Move right
        } else {
          obstacles[i].x = display.width() - obstacles[i].w;
          obstacles[i].speedX = -1; // Move left
        }
      } else { // Y-axis movement
        obstacles[i].x = random(0, display.width() - obstacles[i].w);
        obstacles[i].speedX = 0;
        // Start from top or bottom edge
        if (random(0, 2) == 0) {
          obstacles[i].y = 0;
          obstacles[i].speedY = 1; // Move down
        } else {
          obstacles[i].y = display.height() - obstacles[i].h;
          obstacles[i].speedY = -1; // Move up
        }
      }
      return; // Exit after spawning one obstacle
    }
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Seed for better random numbers

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) yield();
  }
  Serial.println("MPU6050 Found! Game Starting...");

  // Initialize SSD1306 display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(1000);
}

void loop() {
  if (gameOver) {
    // If the game is over, display the message and stop
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 8);
    display.println("GAME OVER");
    display.display();
    while(1); // Freeze here until reset
  }

  // --- Game Logic ---

  // 1. Update Player Position
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  playerX = map(a.acceleration.x, -11, 11, 0, display.width() - playerSize);
  playerY = map(a.acceleration.y, -11, 11, 0, display.height() - playerSize);
  playerX = constrain(playerX, 0, display.width() - playerSize);
  playerY = constrain(playerY, 0, display.height() - playerSize);

  // 2. Update Obstacles
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      obstacles[i].x += obstacles[i].speedX;
      obstacles[i].y += obstacles[i].speedY;

      // Deactivate obstacle if it goes off-screen
      if (obstacles[i].x < 0 || obstacles[i].x > display.width() ||
          obstacles[i].y < 0 || obstacles[i].y > display.height()) {
        obstacles[i].active = false;
      }
    }
  }

  // 3. Spawn a new obstacle if needed
  spawnNewObstacle();

  // 4. Check for Collisions
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      // Simple AABB (Axis-Aligned Bounding Box) collision check
      if (playerX < obstacles[i].x + obstacles[i].w &&
          playerX + playerSize > obstacles[i].x &&
          playerY < obstacles[i].y + obstacles[i].h &&
          playerY + playerSize > obstacles[i].y) {
        gameOver = true; // Collision detected!
      }
    }
  }

  // --- Drawing ---
  display.clearDisplay();
  
  // Draw player
  display.fillRect(playerX, playerY, playerSize, playerSize, SSD1306_WHITE);
  
  // Draw all active obstacles
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      display.drawRect(obstacles[i].x, obstacles[i].y, obstacles[i].w, obstacles[i].h, SSD1306_WHITE);
    }
  }
  
  display.display();
  
  delay(10);
}
