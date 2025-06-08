#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include<conio.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using json = nlohmann::json;

struct CityStruct {
    std::string name;
    double longitude;
    double latitude;

    CityStruct(const std::string& name_) {
        name = name_;
        longitude = 0;
        latitude = 0;
    }
};


struct WeatherStruct {
    int16_t temperature = 0;
    uint16_t wind_speed = 0;
    uint8_t cloud_coverage = 0;
};


void SetWeatherParameters(WeatherStruct& weather_struct, uint8_t hour, size_t i_day, const json& meteo_js, uint8_t day_period) { /// day_period:   0 - 5 night    6 - 11 morning     12 - 17 day     18 - 23 evening
    int16_t temperature_ = meteo_js["hourly"]["temperature_2m"][i_day * 24 + day_period * 6 + hour];
    weather_struct.temperature += temperature_;
    int16_t wind_speed_ = meteo_js["hourly"]["windspeed_10m"][i_day * 24 + day_period * 6 + hour];
    weather_struct.wind_speed += wind_speed_;
    int16_t cloud_coverage_ = meteo_js["hourly"]["cloudcover_mid"][i_day * 24 + day_period * 6 + hour];
    weather_struct.cloud_coverage += cloud_coverage_;
}


void MakeAverage(WeatherStruct& weather_struct) {
    uint8_t DAY_PERIOD_DURATION = 6;

    weather_struct.temperature /= DAY_PERIOD_DURATION;
    weather_struct.wind_speed /= DAY_PERIOD_DURATION;
    weather_struct.cloud_coverage /= DAY_PERIOD_DURATION;

}


void PrintMeteo(uint16_t days, double latitude, double longitude) {

    cpr::Response meteo_api = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                       cpr::Parameters{
                                               {"latitude",        std::to_string(latitude)},
                                               {"longitude",       std::to_string(longitude)},
                                               {"forecast_days",   std::to_string(days)},
                                               {"hourly",          "temperature_2m,windspeed_10m,cloudcover_mid"},
                                               {"current_weather", "true"}
                                       });

    json meteo_js = json::parse(meteo_api.text);

    std::cout << "CURRENT WEATHER " << std::string(meteo_js["current_weather"]["time"]).substr(0, 10) << "\n";
    std::cout << "Temperature = " << meteo_js["current_weather"]["temperature"] << " Â°C" << "\n";
    std::cout << "Windspeed = " << meteo_js["current_weather"]["windspeed"] << " km/h" << "\n\n";


    for (size_t i_day = 0; i_day < days; ++i_day) {
        WeatherStruct night;
        WeatherStruct morning;
        WeatherStruct day;
        WeatherStruct evening;

        for (uint8_t hour = 0; hour < 6; ++hour) {
            SetWeatherParameters(night, hour, i_day, meteo_js, 0);
            SetWeatherParameters(morning, hour, i_day, meteo_js, 1);
            SetWeatherParameters(day, hour, i_day, meteo_js, 2);
            SetWeatherParameters(evening, hour, i_day, meteo_js, 3);

        }

        MakeAverage(night);
        MakeAverage(morning);
        MakeAverage(day);
        MakeAverage(evening);


        std::cout << std::string(meteo_js["hourly"]["time"][i_day * 24]).substr(0, 10) << "\n";
        std::cout << ' ';
        for (size_t i = 0; i < 99; ++i) {
            std::cout << "â”€";
        }
        std::cout << " \n";
        std::cout << "â”‚" << "         Morning        " << "â”‚" << "           Day          " << "â”‚"
                  << "         Evening        " << "â”‚" << "          Night         " << "â”‚" << std::endl;
        std::cout << ' ';
        for (size_t i = 0; i < 99; ++i) {
            std::cout << "â”€";
        }
        std::cout << " \n";

        std::cout << "â”‚" << std::left << std::setw(25)
                  << " Temperature = " + std::to_string(morning.temperature) + " Â°C";
        std::cout << "|" << std::left << std::setw(25) << " Temperature = " + std::to_string(day.temperature) + " Â°C";
        std::cout << "|" << std::left << std::setw(25)
                  << " Temperature = " + std::to_string(evening.temperature) + " Â°C";
        std::cout << "|" << std::left << std::setw(25) << " Temperature = " + std::to_string(night.temperature) + " Â°C";
        std::cout << "â”‚\n";

        std::cout << "â”‚" << std::left << std::setw(24)
                  << " Windspeed = " + std::to_string(morning.wind_speed) + " km/h";
        std::cout << "|" << std::left << std::setw(24) << " Windspeed = " + std::to_string(day.wind_speed) + " km/h";
        std::cout << "|" << std::left << std::setw(24)
                  << " Windspeed = " + std::to_string(evening.wind_speed) + " km/h";
        std::cout << "|" << std::left << std::setw(24) << " Windspeed = " + std::to_string(night.wind_speed) + " km/h";
        std::cout << "â”‚\n";

        std::cout << "â”‚" << std::left << std::setw(24)
                  << " Cloudcover = " + std::to_string(morning.cloud_coverage) + " %";
        std::cout << "|" << std::left << std::setw(24) << " Cloudcover = " + std::to_string(day.cloud_coverage) + " %";
        std::cout << "|" << std::left << std::setw(24)
                  << " Cloudcover = " + std::to_string(evening.cloud_coverage) + " %";
        std::cout << "|" << std::left << std::setw(24)
                  << " Cloudcover = " + std::to_string(night.cloud_coverage) + " %";
        std::cout << "â”‚\n";

        std::cout << ' ';
        for (size_t i = 0; i < 99; ++i) {
            std::cout << "â”€";
        }
        std::cout << " \n\n\n";

    }
}

int main(int argc, char** argv) {

    std::string path_to_config = argv[1];
    std::string api_key = argv[2];


    std::fstream f(path_to_config);
    json data = json::parse(f);

    uint16_t days = data["days"];
    uint16_t update = data["update"];

    std::vector<CityStruct> city_list;
    for (std::string a_city: data["cities"]) {
        CityStruct cityStruct(a_city);
        city_list.push_back(cityStruct);
    }


    size_t i = 0;
    while (i < city_list.size()) {
        std::string url = "https://api.api-ninjas.com/v1/city";
        cpr::Response city_api = cpr::Get(cpr::Url{url}, cpr::Header{
                {"X-Api-Key", api_key}}, cpr::Parameters{{"name", city_list[i].name}});
        if (city_api.status_code != 200) {
            std::cout << "error when getting info from Api-Ninja. Status_code = " << city_api.status_code << "\n";
            system("clear");
            continue;
        }

        json city_js = json::parse(city_api.text);
        city_list[i].longitude = city_js[0]["longitude"];
        city_list[i].latitude = city_js[0]["latitude"];
        ++i;
    }

    auto start = std::chrono::steady_clock::now();

    char key;
    int city_pointer = 0;
    while (!_kbhit()) {
        auto end = std::chrono::steady_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

        if (duration >= update) {
            std::cout << "Please wait\n";
            start = std::chrono::steady_clock::now();

            system("clear");
            std::cout << "\nForecast for " << city_list[city_pointer].name << "\n\n";
            PrintMeteo(days, city_list[city_pointer].latitude, city_list[city_pointer].longitude);

        }
    }

    while (true) {
        int key = _getch();

        if (key == '\033') { ///Esc
            std::cout << "ESC pressed\n";
            break;
        }

        if (key == '\053') { ///+
            days++;
        } else if (key == '\055') {///-

            if (days == 1) {
                continue;
            }
            days--;

        } else if (key == '\156') { ///n
            city_pointer = (city_pointer + 1) % city_list.size();
        } else if (key == '\160') { ///p
            if (city_pointer == 0) city_pointer = city_list.size() - 1;
            else city_pointer--;
        }

        while (!_kbhit()) {
            auto end = std::chrono::steady_clock::now();
            long long duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

            if (duration >= update) {
                std::cout << "Please wait\n";
                start = std::chrono::steady_clock::now();

                system("clear");
                std::cout << "\nForecast for " << city_list[city_pointer].name << "\n\n";
                PrintMeteo(days, city_list[city_pointer].latitude, city_list[city_pointer].longitude);
            }
        }
    }

    return 0;
}
