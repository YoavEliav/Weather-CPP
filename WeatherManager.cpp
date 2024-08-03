#include "WeatherManager.h"
#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <sstream>
#include <cctype>
#include <algorithm> 

WeatherManager::WeatherManager(const std::string& apiKey) : apiKey(apiKey), currentWeatherData{ 0.0f, 0, 0.0f, true } {}

// a
void WeatherManager::fetchWeather(const std::string& city) {
    // create a thread
    std::thread([this, city]() {
        httplib::Client cli("http://api.openweathermap.org");
        std::string path = "/data/2.5/weather?q=" + city + "&units=metric&appid=" + this->apiKey;
        if (auto res = cli.Get(path.c_str())) {
            if (res->status == 200) { // OK
                {
                    std::lock_guard<std::mutex> lock(this->dataMutex);
                    this->currentCity = city;
                    this->currentWeatherData.validCity = true;
                    parseWeatherData(res->body); // parse
                    if (favoriteCities.count(city)) {
                        favoriteWeatherData[city] = currentWeatherData;
                    }
                }
            }
            else {
                std::lock_guard<std::mutex> lock(this->dataMutex);
                this->currentWeatherData.validCity = false;
                std::cerr << "Error: " << res->status << std::endl;
            }
        }
        else {
            std::lock_guard<std::mutex> lock(this->dataMutex);
            this->currentWeatherData.validCity = false;
            std::cerr << "Error: " << res.error() << std::endl;
        }
        }).detach();
}

void WeatherManager::parseWeatherData(const std::string& jsonData) {
    auto json = nlohmann::json::parse(jsonData);
    currentWeatherData.temperature = json["main"]["temp"].get<float>();
    currentWeatherData.humidity = json["main"]["humidity"].get<int>();
    currentWeatherData.windSpeed = json["wind"]["speed"].get<float>();
}

std::string WeatherManager::getCity() const {
    std::lock_guard<std::mutex> lock(this->dataMutex);
    return currentCity;
}

float WeatherManager::getTemperature() const {
    return currentWeatherData.temperature;
}

int WeatherManager::getHumidity() const {
    return currentWeatherData.humidity;
}

float WeatherManager::getWindSpeed() const {
    return currentWeatherData.windSpeed;
}

bool WeatherManager::isValidCity() const {
    return currentWeatherData.validCity;
}

void WeatherManager::addFavoriteCity(std::string city) {
    capitalizeCityName(city);
    std::lock_guard<std::mutex> lock(this->dataMutex);
    favoriteCities.insert(city);
    favoriteWeatherData[city] = currentWeatherData;
}

bool WeatherManager::favoriteCityExists(const std::string& city) const {
    std::string lowerCity = city;
    std::transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(), ::tolower);

    for (const auto& favoriteCity : favoriteCities) {
        std::string lowerFavoriteCity = favoriteCity;
        std::transform(lowerFavoriteCity.begin(), lowerFavoriteCity.end(), lowerFavoriteCity.begin(), ::tolower);
        if (lowerFavoriteCity == lowerCity) {
            return true;
        }
    }
    return false;
}

const std::set<std::string>& WeatherManager::getFavoriteCities() const {
    std::lock_guard<std::mutex> lock(this->dataMutex);
    return favoriteCities;
}

const WeatherData& WeatherManager::getWeatherData(const std::string& city) const {
    std::lock_guard<std::mutex> lock(this->dataMutex);
    return favoriteWeatherData.at(city);
}

void WeatherManager::removeFavoriteCity(const std::string& city) {
    std::lock_guard<std::mutex> lock(this->dataMutex);
    favoriteCities.erase(city);
    favoriteWeatherData.erase(city);
}

void WeatherManager::capitalizeCityName(std::string& city) {
    std::istringstream iss(city);
    std::string word;
    std::string capitalizedCity;
    while (iss >> word) {
        if (!word.empty() && word[0] >= 'a' && word[0] <= 'z') {
            word[0] = std::toupper(word[0]);
        }
        for (size_t i = 1; i < word.size(); ++i) {
            word[i] = std::tolower(word[i]);
        }
        if (!capitalizedCity.empty()) {
            capitalizedCity += " ";
        }
        capitalizedCity += word;
    }
    city = capitalizedCity;
}




