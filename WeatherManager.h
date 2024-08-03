#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include "json.hpp"

struct WeatherData {
    float temperature;
    int humidity;
    float windSpeed;
    bool validCity;
};

class WeatherManager {
public:
    WeatherManager(const std::string& apiKey);
    void fetchWeather(const std::string& city);
    std::string getCity() const;
    float getTemperature() const;
    int getHumidity() const;
    float getWindSpeed() const;
    bool isValidCity() const;
    void addFavoriteCity(std::string city);
    bool favoriteCityExists(const std::string& city) const;
    const std::set<std::string>& getFavoriteCities() const;
    const WeatherData& getWeatherData(const std::string& city) const;
    void removeFavoriteCity(const std::string& city);
    
private:
    void parseWeatherData(const std::string& jsonData);
    void capitalizeCityName(std::string& city);

    std::string apiKey;
    std::string currentCity;
    WeatherData currentWeatherData;
    mutable std::mutex dataMutex;
    std::set<std::string> favoriteCities;
    std::map<std::string, WeatherData> favoriteWeatherData;
    std::vector<std::string> countries;
    std::vector<std::string> cities;
};

#endif // WEATHER_MANAGER_H
