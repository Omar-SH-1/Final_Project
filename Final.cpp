#include "Final.h"

bool isValidMatrixLine(const std::string& line) {
    for (char c : line) {
        if (!std::isdigit(c) && c != ' ' && c != '\t') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> loadCountries(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        exit(1);
    }

    std::vector<std::string> countries;
    std::string country;
    while (std::getline(file, country)) {
        countries.push_back(country);
    }

    file.close();
    return countries;
}

std::vector<std::vector<int>> loadMatrix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        exit(1);
    }

    std::vector<std::vector<int>> matrix;
    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        if (!isValidMatrixLine(line)) {
            std::cerr << "Ошибка: Некорректные данные в строке: \"" << line << "\"" << std::endl;
            exit(1);
        }

        std::vector<int> row;
        std::istringstream stream(line);
        int value;
        while (stream >> value) {
            row.push_back(value);
        }

        matrix.push_back(row);
    }

    file.close();
    return matrix;
}

void removeConnection(std::vector<std::vector<int>>& matrix, int country1Idx, int country2Idx) {
    matrix[country1Idx][country2Idx] = 0;
    matrix[country2Idx][country1Idx] = 0;
}

std::vector<int> findShortestRoute(const std::vector<std::vector<int>>& matrix, int startIdx, const std::unordered_set<int>& targetIdxs) {
    std::queue<std::vector<int>> q;
    std::vector<bool> visited(matrix.size(), false);

    q.push({startIdx});
    visited[startIdx] = true;

    while (!q.empty()) {
        std::vector<int> path = q.front();
        q.pop();
        int current = path.back();

        if (targetIdxs.count(current)) {
            return path;
        }

        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                std::vector<int> newPath = path;
                newPath.push_back(i);
                q.push(newPath);
            }
        }
    }

    return {};
}

void checkConnectionsToContinents(const std::vector<std::vector<int>>& matrix, int countryIdx,
                                  const std::unordered_map<std::string, std::vector<int>>& continents,
                                  const std::vector<std::string>& countries) {
    for (const auto& [continent, countryIdxs] : continents) {
        std::unordered_set<int> targetIdxs(countryIdxs.begin(), countryIdxs.end());

        std::vector<int> route = findShortestRoute(matrix, countryIdx, targetIdxs);

        if (!route.empty()) {
            std::cout << "Страна " << countries[countryIdx] << " имеет доступ к континенту " << continent << " по следующему маршруту:\n";
            for (size_t i = 0; i < route.size(); ++i) {
                std::cout << countries[route[i]];
                if (i < route.size() - 1) std::cout << " -> ";
            }
            std::cout << "\n";
        } else {
            std::cout << "Страна " << countries[countryIdx] << " НЕ имеет доступа к континенту " << continent << std::endl;
        }
    }
}
