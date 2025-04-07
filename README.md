# Smart-Home-RTOS-Simulator

The **Smart-Home-RTOS-Simulator** is a simulation project designed to emulate a real-time operating system (RTOS) for managing smart home devices. It includes tasks for controlling lights, window blinds, and temperature sensors, providing a modular and extensible framework for smart home automation.

## Features

- **Task Management**: Centralized task scheduling and execution using the `TaskManager`.
- **Light Control**: Manage room lights with time-based and motion-based rules.
- **Window Blind Control**: Automate window blinds based on time and light intensity.
- **Temperature Monitoring**: Simulate and monitor temperature readings.
- **Logging**: Centralized logging system for debugging and monitoring.

## Getting Started

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

The application provides a command-line interface for interacting with the smart home system. Use the following commands:

- `help`: Display available commands.
- `stats`: Show system statistics.
- `temp`: Display the current temperature.
- `blinds`: Control window blinds.
- `lights`: Control room lights.
- `status`: Show the complete system status.
- `quit`: Exit the application.

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
