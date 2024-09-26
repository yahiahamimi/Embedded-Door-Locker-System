# Final Project Control ECU
This project is a part of the Embedded Systems course at Birkbeck University. It is a control unit for an electric vehicle, which is connected to an LCD display, a keypad, a buzzer, and a DC motor. The project is implemented in C and uses the AVR microcontroller.

## Features
- The user can create a password and store it in the EEPROM memory.
- The user can input the password and control the DC motor, LCD display, keypad, buzzer, and UART communication based on the correctness of the password.
- The user can also control the DC motor and LCD display using the keypad.
- The project uses the TWI (I2C) protocol for communication between the control unit and external EEPROM memory.
- The project uses the Timer1 and Timer0 PWM for controlling the speed of the DC motor.
- The project uses the UART protocol for communication between the control unit and a PC.
- The project uses the LCD display for displaying information to the user.
- The project uses the keypad for inputting the password and controlling the DC motor and LCD display.
- The project uses the buzzer for indicating the correctness of the password.
- The project uses the DC motor for controlling the speed of the vehicle.

## Requirements
- AVR Studio 5.0 or later
- AVR Development Board
- LCD Display
- Keypad
- Buzzer
- DC Motor
- External EEPROM Memory
- USB-to-Serial Cable

## Installation
1. Open the Final_project_Control_ECU.ASM file in AVR Studio.
2. Click on the "Build" button to compile and flash the code to the AVR Development Board.
3. Connect the LCD display, keypad, buzzer, DC motor, and external EEPROM memory to the AVR Development Board.
4. Connect the USB-to-Serial Cable to the AVR Development Board.
5. Open the UART terminal in the AVR Studio.
6. Press the "Start" button to run the program.

## Usage
1. Create a password by selecting the "Create Password" option.
2. Input the password by selecting the "Input Password" option.
3. Control the DC motor, LCD display, keypad, buzzer, and UART communication based on the correctness of the password.
4. Control the DC motor and LCD display using the keypad.
5. The project uses the TWI protocol for communication between the control unit and external EEPROM memory.
6. The project uses the Timer1 and Timer0 PWM for controlling the speed of the DC motor.
7. The project uses the UART protocol for communication between the control unit and a PC.
8. The project uses the LCD display for displaying information to the user.
9. The project uses the keypad for inputting the password and controlling the DC motor and LCD display.
10. The project uses the buzzer for indicating the correctness of the password.
11. The project uses the DC motor for controlling the speed of the vehicle.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Dr. M.A. Karim
- Dr. M.A. Ashraf
- Dr. M.A. Muneeb
- Dr. M.A. Waqas
