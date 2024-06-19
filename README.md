# 📊 Welcome Aboard the Elevator Simulator! 🛗

This Arduino code brings a multi-floor elevator to life (virtually, of course)! It simulates button presses, detects floor sensors, and manages the elevator's movement and those delightful **ding**ing doors. 

## 🛠️ Gear Up!

To build your elevator simulator, you'll need:

| Item | Quantity |
| --- | --- |
| Arduino Uno (or compatible board) | 1 |
| Servo motor (for that smooth door action) | 1 |
| Liquid Crystal I2C display (optional, but adds a visual touch) | 1 |
| Analog sensors (for call buttons, cabin buttons, and floor sensors) | 4 |
| LEDs (one for each floor to show where you are) | 4 |
| Jumper wires to connect everything | ⚡ |
| Breadboard (optional, for easy prototyping) | 1 |
| Dc motor with controller | 1 |

## 🔌 Wiring It Up!

The connections will vary slightly depending on your hardware, but here's a roadmap:

- Connect the call buttons, cabin buttons, and floor sensors to analog input pins (A0, A1, A2 in this code).
- Each floor should have its own LED, connected to digital output pins (Pins 3, 4, 5, 6 in this code).
- Hook up the servo motor to pin 11.
- If you're using the LCD display, refer to the LiquidCrystal_I2C library documentation for specific connection instructions.
- Connect the power and ground pins according to your motor and display specifications (Pins 13 and 12 are used here as examples).

  ![Arduino Simulation](https://github.com/doffn/ELEVATOR/blob/main/Elevator.png)

## 🔍 Code Walkthrough: A Peek Under the Hood

This code is like a well-oiled machine, with different functions working together:

| Function | Description |
| --- | --- |
| `setup()` | Gets everything ready for takeoff! It initializes pins, the LCD display (if used), the servo motor, and even calibrates the elevator. |
| `loop()` | This is the main control center, constantly checking for button presses, sensor readings, and making sure the elevator moves and the doors open and close at the right times. |
| `printButtonEvents()` | This function (mostly for debugging) shows the current state of the button presses. |
| `floorReg()` | Reads the call and cabin buttons, keeping track of who wants to go up or down in the `buttonEvent` array. |
| `registerFloor()` | Uses the floor sensors to figure out which floor the elevator is currently on. |
| `calibration()` | This important function helps the elevator learn where each floor is by moving it down to the ground floor and identifying each floor sensor reading. |
| `movingUp()` | As the name suggests, this function moves the elevator up one floor, opening the door if someone requested a stop there. |
| `movingDown()` | Similar to `movingUp()`, but takes the elevator down a floor and opens the door for waiting passengers. |
| `door()` | Controls the servo motor, making the elevator door open with a flourish and then close securely. |
| `ledOn()` | Lights up the LED for the current floor, so everyone knows where they are. |
| `printFloor()` | If you're using the LCD display, this function shows the current floor number. |
| `mainLift()` | The brain of the operation, this function decides where the elevator goes next based on button requests and the current floor. |

## 🎨 Make it Yours!

This code is a great starting point, and you can customize it to create your dream elevator:

- Change the number of floors (`N`) in the code to match your simulation needs.
- Adjust the pin assignments in the `setup()` function to work with your specific hardware setup.
- Play around with the movement timing (`movementTime`) and door duration (`doorDuration`) in milliseconds to get that perfect elevator experience.

## 🚀 Bonus Level Up!

Want to take your elevator simulator to the next level? Here are some ideas:

- Add emergency stop buttons for those "uh oh" moments.
- Implement overload detection to prevent the elevator from getting too crowded.
- Use the LCD display to show additional information like waiting times or a fun animation.

The sky's the limit! 🌌 With this code and a little creativity, you can build a fantastic elevator simulator that brings your imagination to life. We hope this README makes building and understanding the code a breeze. Happy tinkering! 🛠️

## Contact Me 📧

Feel free to reach out to me via email at dawitneri888@gmail.com or connect with me on [LinkedIn](https://www.linkedin.com/in/dawit-neri-16275a208/) and [Twitter](https://twitter.com/doffneri). I'm open to collaboration opportunities, so let's connect and discuss exciting projects!

## Acknowledgements 🙏

A special thanks to [Elevator](https://github.com/AlieksieievYurii/Elevator) for his work on the system.

---

Thank you for visiting. The sky's the limit! 🌌 With this code and a little creativity, you can build a fantastic elevator simulator that brings your imagination to life. We hope this README makes building and understanding the code a breeze. Happy tinkering! 🛠️
😊
