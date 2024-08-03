#include "UI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <ctime>
#include <algorithm>

void renderUI(WeatherManager& weatherManager) {
    ImGui::Begin("Weather App");

    static bool showWeather = false;
    static bool showInvalidCityMessage = false;
    static bool showCityAlreadyExistsMessage = false;
    static bool showExportSuccessMessage = false;
    const char* exportFilePath = "Weather in the favorite cities.txt";

    static char city[128] = "";
    ImGui::Text("Enter a city or country:");
    ImGui::SetNextItemWidth(100);
    ImGui::InputText(" ", city, sizeof(city));

    bool isCityEntered = city[0] != '\0';

    if (!isCityEntered) {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Get Weather")) {
        weatherManager.fetchWeather(city);
        showWeather = true;
        showInvalidCityMessage = false;
        showCityAlreadyExistsMessage = false;
        city[0] = '\0'; // Clear input text
    }

    if (ImGui::Button("Add to Favorites")) {
        std::string cityName(city);
        std::transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower); // Convert to lowercase

        if (weatherManager.favoriteCityExists(cityName)) {
            showCityAlreadyExistsMessage = true;
            showInvalidCityMessage = false;
        }
        else {
            weatherManager.fetchWeather(city);
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait for the request to complete
            if (weatherManager.isValidCity()) {
                weatherManager.addFavoriteCity(city);
                showWeather = false; // Clear weather display
                showCityAlreadyExistsMessage = false;
                showInvalidCityMessage = false;
            }
            else {
                showInvalidCityMessage = true;
                showCityAlreadyExistsMessage = false;
            }
        }
        city[0] = '\0'; // Clear input text
    }
    ImGui::Separator(); // print the line

    if (!isCityEntered) {
        ImGui::EndDisabled();
    }

    if (showWeather) {
        if (!weatherManager.isValidCity()) {
            ImGui::Text("INVALID CITY! TRY AGAIN :)");
        }
        else if (!weatherManager.getCity().empty()) {
            ImGui::Text("City: %s", weatherManager.getCity().c_str());
            ImGui::Text("Temperature: %.2f C", weatherManager.getTemperature());
            ImGui::Text("Humidity: %d%%", weatherManager.getHumidity());
            ImGui::Text("Wind Speed: %.2f m/s", weatherManager.getWindSpeed());
        }
    }

    if (showInvalidCityMessage) {
        ImGui::Text("INVALID CITY! TRY AGAIN :)");
    }

    if (showCityAlreadyExistsMessage) {
        ImGui::Text("CITY ALREADY EXISTS IN FAVORITES!");
    }

    ImGui::End();

    // Favorites panel
    ImGui::Begin("Favorite Locations");

    static std::string selectedFavoriteCity;
    static std::unordered_map<std::string, bool> checkedCities;
    static bool selectAll = true; // Set selectAll to true by default
    bool anyChecked = true; // Set anyChecked to true by default to avoid disabling Remove and Export buttons initially

    // Ensure new favorite cities are added to the checkedCities map
    for (const auto& favoriteCity : weatherManager.getFavoriteCities()) {
        if (checkedCities.find(favoriteCity) == checkedCities.end()) {
            checkedCities[favoriteCity] = true; // Set all new checkboxes to true by default
        }
    }

    if (ImGui::Checkbox("Select All", &selectAll)) {
        for (auto& city : checkedCities) {
            city.second = selectAll;
        }
    }

    anyChecked = false; // Reset anyChecked before the loop

    for (const auto& favoriteCity : weatherManager.getFavoriteCities()) {
        bool isSelected = checkedCities[favoriteCity];
        if (ImGui::Checkbox(("##check_" + favoriteCity).c_str(), &isSelected)) {
            checkedCities[favoriteCity] = isSelected;
            if (!isSelected) {
                selectAll = false;
            }
        }

        if (isSelected) {
            anyChecked = true;
        }

        ImGui::SameLine();
        bool isSelectedCity = (selectedFavoriteCity == favoriteCity);
        if (ImGui::Selectable(favoriteCity.c_str(), isSelectedCity)) {
            if (isSelectedCity) {
                selectedFavoriteCity.clear(); // Clear the selection if already selected
            }
            else {
                selectedFavoriteCity = favoriteCity; // Set the selected city
            }
        }

        if (selectedFavoriteCity == favoriteCity) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); // Yellow text
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 128, 255)); // Dark blue background
            ImGui::BeginChild("WeatherData", ImVec2(0, 100), true, ImGuiWindowFlags_NoScrollbar);

            const WeatherData& weatherData = weatherManager.getWeatherData(favoriteCity);
            if (weatherData.validCity) {
                ImGui::Text("City: %s", favoriteCity.c_str());
                ImGui::Text("Temperature: %.2f C", weatherData.temperature);
                ImGui::Text("Humidity: %d%%", weatherData.humidity);
                ImGui::Text("Wind Speed: %.2f m/s", weatherData.windSpeed);
            }
            else {
                ImGui::Text("INVALID CITY! TRY AGAIN :)");
            }

            ImGui::EndChild();
            ImGui::PopStyleColor(2); // Restore the colors
        }
    }

    if (!anyChecked) {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Remove")) {
        for (const auto& city : checkedCities) {
            if (city.second) {
                weatherManager.removeFavoriteCity(city.first);
            }
        }
        checkedCities.clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("Export")) {
        std::ofstream outFile("Weather in the favorite cities.txt");

        // Get current time
        std::time_t now = std::time(nullptr);
        struct tm timeInfo;
        localtime_s(&timeInfo, &now); // Using localtime_s for secure function
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeInfo);

        // Write time to the file
        outFile << "Export Date and Time: " << timeStr << std::endl << std::endl;
        for (const auto& city : checkedCities) {
            if (city.second) {
                const WeatherData& weatherData = weatherManager.getWeatherData(city.first);
                if (weatherData.validCity) {
                    outFile << "City: " << city.first << std::endl;
                    outFile << "Temperature: " << weatherData.temperature << "°C" << std::endl;
                    outFile << "Humidity: " << weatherData.humidity << "%" << std::endl;
                    outFile << "Wind Speed: " << weatherData.windSpeed << " m/s" << std::endl;
                    outFile << "-----------------------------" << std::endl;
                }
                else {
                    outFile << "City: " << city.first << std::endl;
                    outFile << "INVALID CITY! TRY AGAIN :)" << std::endl;
                    outFile << "-----------------------------" << std::endl;
                }
            }
        }

        outFile.close();
        showExportSuccessMessage = true;
    }

    if (!anyChecked) {
        ImGui::EndDisabled();
    }

    if (showExportSuccessMessage) {
        ImGui::OpenPopup("Export Success");
    }

    if (ImGui::BeginPopupModal("Export Success", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("The file was created successfully!");
        if (ImGui::Button("Go to File")) {
            std::string command = std::string("explorer.exe /select,") + exportFilePath;
            system(command.c_str());
            showExportSuccessMessage = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
            showExportSuccessMessage = false;
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}
