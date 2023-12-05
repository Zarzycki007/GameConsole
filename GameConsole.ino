/*
Hello! This is a game console! If you want to add a game, follow these 4 steps:
1. Add your game to the GAME class.
2. Add your game to the gameList array.
3. Change the size of the gameList array.
4. Call an object of your game class and invoke the main method.

The program already includes the NewGame class, which you can remove. It serves as an example of how to correctly add a game.
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define screen width and height
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Define pins
#define SIO 2
#define SJO 3
#define IO 7

// Initialize the OLED display object
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// Control class to handle all input devices
class Control {
  // Define joystick range
  #define JOSTICK_MIN 123
  #define JOSTICK_MAX 900

  // Define joystick directions
  #define UP 1
  #define DOWN 2
  #define RIGHT 1
  #define LEFT 2

  // Define pins for horizontal and vertical directions
  #define HOR A0
  #define VER A1

public:
  // Function to read joystick input and determine direction
  uint8_t Jostick(uint8_t pin) {
    if (analogRead(pin) >= JOSTICK_MAX)
      return 2;
    if (analogRead(pin) <= JOSTICK_MIN)
      return 1;
    else
      return 0;
  }

  // Function to check the state of a switch
  bool Switch(uint8_t pin) {
    if (digitalRead(pin) == HIGH)
      return false;
    if (digitalRead(pin) == LOW)
      return true;
  }

  // Function to check the state of a switch and handle debouncing
  bool SwitchIO(uint8_t pin, uint8_t time) {
    if (Switch(pin)) {
      delay(time);
      if (Switch(pin))
        return true;
      else
        return false;
    }
  }
};

// 1. GAME Class: Add your games here!
class Game {
public:
  class NewGame {
    Control contr;

  public:
    void main() {
      delay(200);

      bool exit = true;

      while (exit) {
        display.display();
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2, 2);
        display.setCursor(15, 15);
        display.print("NEW GAME");

        display.setTextSize(1, 2);
        display.setCursor(37, 45);
        display.print("Exit");

        display.drawRect(32, 42, 50, 20, WHITE);

        if (contr.Switch(SJO)) {
          exit = false;
        }
      }
    }
  };
};

// System class to manage the main menu and system operations
class System {
  Control &contr;

  #define CURSOR_HEIGHT 20
  #define CURSOR_MARGIN 2
  String gameList[1]; // 3. Don't forget to change the size of the array, and remember it is zero-based!
  uint8_t id;
  uint8_t gameListSize;
  uint8_t cursorLimit;
  int8_t cursorPos;
  int8_t bgPos;
  bool exitMenu;

public:
  System() : id(1), cursorPos(2),
  cursorLimit(gameListSize * CURSOR_HEIGHT + CURSOR_MARGIN - CURSOR_HEIGHT),
  gameListSize(sizeof(gameList) / 6), bgPos(5), exitMenu(true) {
    // 2. Add your game to the menu. Remember to change the size of the array!
    gameList[0] = "Create Your Game!";
  }

  // Main function for system operation
  void main() {
    while (exitMenu) {
      delay(100);
      pointer();
      displayDisplays(bgPos);
      turnOFF();
    }
  }

  // Function to turn on the system
  void turnON() {
    if (contr.SwitchIO(SIO, 20)) {
      digitalWrite(IO, HIGH);
      delay(200);

      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }
  }

  // Function to turn off the system
  void turnOFF() {
    if (contr.SwitchIO(SIO, 20)) {
      display.clearDisplay();
      display.display();

      digitalWrite(IO, LOW);

      delay(200);
      void (*resetFunc)(void) = 0;
      resetFunc();
      delay(200);
    }
  }

  // Function to display menu options on the OLED screen
  void displayDisplays(uint8_t setElements) {
    display.clearDisplay();
    for (String name : gameList) {
      display.setTextColor(WHITE);
      display.setTextSize(1, 2);
      display.setCursor(5, setElements);
      display.println(name);
      setElements += CURSOR_HEIGHT;
    }
    display.drawRect(2, cursorPos, 120, CURSOR_HEIGHT, WHITE);
    display.display();
  }

  // Function to handle joystick input for menu navigation
  void pointer() {
    if (contr.Switch(SJO)) {
      display.clearDisplay();
      menu();
    }

    if (contr.Jostick(VER) == DOWN) {
      cursorPos += CURSOR_HEIGHT;
      id++;

      if (cursorPos > cursorLimit)
        cursorPos = cursorLimit, id = gameListSize;
      if (cursorPos > 42)
        bgPos -= 60, cursorPos = CURSOR_MARGIN, cursorLimit = ((gameListSize + 1) * CURSOR_HEIGHT + CURSOR_MARGIN) - 100;
    }

    if (contr.Jostick(VER) == UP) {
      cursorPos -= CURSOR_HEIGHT;
      id--;
      if (cursorPos < CURSOR_MARGIN && bgPos == 5)
        cursorPos = CURSOR_MARGIN, id = 1;
      if (cursorPos < CURSOR_MARGIN)
        bgPos += 60, cursorPos = 42, cursorLimit = gameListSize * CURSOR_HEIGHT + CURSOR_MARGIN - CURSOR_HEIGHT;
    }
  }

  // 4. Function to handle menu selection and navigation. Call your game object here!
  void menu() {
    Game Game;

    switch (id) {
    case 1: {
      Game::NewGame newGame;
      newGame.main();
      break;
    }

    default:
      Serial.println("Error");
      break;
    }
  }
};

Control contr;

// Setup function to initialize pins and serial communication
void setup() {
  Serial.begin(9600);

  pinMode(SIO, INPUT_PULLUP);
  pinMode(SJO, INPUT_PULLUP);
  pinMode(IO, OUTPUT);
  pinMode(HOR, INPUT);
  pinMode(VER, INPUT);
}

// Main loop function
void loop() {
  if (contr.SwitchIO(SIO, 20)) {
    // Create a System object, turn on the system, and run the main loop
    System system;
    system.turnON();
    system.main();
  }
}
