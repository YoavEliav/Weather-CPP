
# Weather App

## Overview
The Weather App is a simple desktop application built using C++ and Dear ImGui for the user interface. It allows users to fetch real-time weather data for any city or country using the OpenWeatherMap API. Users can also add cities to a list of favorite locations, view weather details for their favorite cities, and export the weather data to a text file.

## Features
- **Fetch Weather:** Retrieve real-time weather data, including temperature, humidity, and wind speed, for any city or country.
- **Add to Favorites:** Save cities to a list of favorite locations for quick access.
- **View Favorite Locations:** View detailed weather information for your favorite cities.
- **Export Data:** Export weather data for selected favorite cities to a text file with a timestamp.

## Installation

### Prerequisites
- **C++ Compiler:** A C++17 compatible compiler.
- **CMake:** Version 3.10 or higher.
- **OpenGL:** Ensure your system has OpenGL installed.
- **GLFW:** The app uses GLFW for window management.
- **Dear ImGui:** ImGui for the graphical user interface.
- **nlohmann/json:** A JSON parsing library for handling the weather data.
- **cpp-httplib:** A simple and powerful HTTP/HTTPS library in C++.

### Setup
1. **Clone the Repository:**
   ```sh
   git clone https://github.com/yourusername/weather-app.git
   cd weather-app
   ```

2. **Build the Project:**
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the Application:**
   ```sh
   ./WeatherApp
   ```

## Usage
1. **Start the Application:**
   - Upon launching, you will see a window where you can input a city or country name.

2. **Fetching Weather:**
   - Type the name of a city or country and click "Get Weather" to fetch the current weather data.

3. **Add to Favorites:**
   - After fetching the weather data, you can add the city to your favorites by clicking "Add to Favorites."

4. **View Favorite Locations:**
   - The "Favorite Locations" panel allows you to view and select your saved cities. Clicking on a city name will display the weather details.

5. **Export Data:**
   - Select one or more favorite cities using the checkboxes and click "Export" to save the weather data to a text file.

6. **Remove from Favorites:**
   - Use the "Remove" button to delete selected cities from your favorites.

## API Key
- The app requires an API key from [OpenWeatherMap](https://openweathermap.org/api).
- The key should be passed when initializing the `WeatherManager` class in `main.cpp`.

## Dependencies
- **GLFW:** [GLFW](https://www.glfw.org/download.html)
- **Dear ImGui:** [Dear ImGui](https://github.com/ocornut/imgui)
- **nlohmann/json:** [nlohmann/json](https://github.com/nlohmann/json)
- **cpp-httplib:** [cpp-httplib](https://github.com/yhirose/cpp-httplib)

## Folder Structure
```
.
├── src/
│   ├── main.cpp
│   ├── WeatherManager.h
│   ├── WeatherManager.cpp
│   ├── UI.h
│   ├── UI.cpp
├── README.md
└── ... (other necessary files)
```

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any features, fixes, or improvements.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgments
- [OpenWeatherMap](https://openweathermap.org/) for providing the API used to fetch weather data.
- [Dear ImGui](https://github.com/ocornut/imgui) for the graphical user interface framework.
