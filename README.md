# Smart-Home-RTOS-Simulator

The **Smart-Home-RTOS-Simulator** is a C++ project designed to simulate a real-time operating system (RTOS) for managing smart home devices. The project demonstrates the use of modern C++ features, multithreading, and object-oriented programming to create a modular and extensible framework for smart home automation.

## Overview

This simulator provides a centralized task management system (`TaskManager`) to schedule and execute tasks for controlling various smart home devices, such as lights, window blinds, and temperature sensors. The project is designed to emulate the behavior of an RTOS, where tasks are prioritized and executed based on their importance and readiness.

### Key Components

1. **Task Management**:
   - The `TaskManager` class is the core of the system, responsible for scheduling and executing tasks.
   - Tasks are represented as objects derived from the `Task` base class, allowing for extensibility and modularity.
   - Tasks are prioritized and executed in a thread-safe manner using `std::mutex` and `std::condition_variable`.

2. **Multithreading**:
   - The project uses C++ standard threading libraries (`std::thread`, `std::mutex`, `std::condition_variable`) to simulate concurrent task execution.
   - The scheduler runs in a separate thread, ensuring that tasks are executed without blocking the main control panel.

3. **Device Control**:
   - **Window Blinds**: The `WindowBlindTask` class manages the state of window blinds, allowing users to open, close, or set specific positions for each blind.
   - **Lights**: The `LightControlTask` class provides functionality to control room lights, including turning them on/off and adjusting brightness levels.
   - **Temperature Monitoring**: The `TemperatureSensorTask` class simulates a temperature sensor, providing periodic readings and warnings for abnormal temperatures.

4. **Logging**:
   - The `Logger` class provides a centralized logging system to record events and debug information.
   - Logs can be output to the console or stored in a file for later analysis.

5. **Command-Line Interface**:
   - The `ControlPanel` class provides an interactive command-line interface for users to interact with the system.
   - Users can issue commands to control devices, view system statistics, and monitor the overall status of the smart home.
   
### Technologies and Design Choices

- **C++17**: The project leverages modern C++ features such as smart pointers (`std::unique_ptr`), `std::chrono` for time management, and `std::atomic` for thread-safe flags.
- **Object-Oriented Design**: The use of classes and inheritance ensures a clean and modular design, making it easy to extend the system with new devices or features.
- **Thread Safety**: The project uses synchronization primitives like `std::mutex` and `std::condition_variable` to ensure safe access to shared resources in a multithreaded environment.
- **Extensibility**: The `Task` base class and the modular design of device controllers make it easy to add new tasks or devices to the system.

### Prerequisites

- **C++17 or later**: Ensure your compiler supports C++17.
- **CMake**: Version 3.10 or later.
- **Git**: For version control.

### Build Instructions

1. Clone the repository:
   ```sh
   git clone https://github.com/your-username/Smart-Home-RTOS-Simulator.git
   cd Smart-Home-RTOS-Simulator
   ```
2. Create a build directory and configure the project:
   ```sh
   mkdir build
   cd build
   cmake ..
   ```
3. Build the project:
   ```sh
   cmake --build .
   ```
4. Run the application:
   ```sh
   ./bin/smart_home_rtos
   ```
## Usage

- `help`: Display a list of available commands and their descriptions.
- `quit`: Exit the application and stop the scheduler.
- `stats`: Show system statistics, including the total number of tasks, active tasks, system uptime, and task priorities.
- `temp`: Display the latest temperature reading from the temperature sensor. Includes warnings for high or low temperatures.
- `blinds`: Control window blinds with the following options:
  - View the current status of all window blinds.
  - Set the position of a specific window blind (e.g., Closed, Quarter Open, Half Open, etc.).
  - Open all blinds.
  - Close all blinds.
- `lights`: Control room lights with the following options:
  - View the current status of all room lights.
  - Turn on or off a specific room light.
  - Set the brightness level of a specific room light (e.g., Off, Low, Medium, High, Full).
  - Turn on all lights.
  - Turn off all lights.
- `status`: Display the complete system status, including:
  - Current temperature.
  - System uptime.
  - Status of all room lights.
  - Status of all window blinds.

Simply type the command in the terminal and follow the on-screen instructions for interactive options.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes and push them to your fork.
4. Submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by real-time operating systems and smart home automation concepts.
- Developed as a learning project for C++ and RTOS simulation.
- Oskar Kierys
