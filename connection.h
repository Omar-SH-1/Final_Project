#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>

// Проверка, что строка содержит только цифры и пробелы
bool isValidMatrixLine(const std::string& line);

// Функция для загрузки списка стран
std::vector<std::string> loadCountries(const std::string& filename);

// Функция для загрузки матрицы из файла
std::vector<std::vector<int>> loadMatrix(const std::string& filename);

// Удаление связи между двумя странами
void removeConnection(std::vector<std::vector<int>>& matrix, int country1Idx, int country2Idx);

// Поиск кратчайшего маршрута с использованием BFS
std::vector<int> findShortestRoute(const std::vector<std::vector<int>>& matrix, int startIdx, const std::unordered_set<int>& targetIdxs);

// Проверка доступности до континентов и вывод маршрутов
void checkConnectionsToContinents(const std::vector<std::vector<int>>& matrix, int countryIdx,
                                  const std::unordered_map<std::string, std::vector<int>>& continents,
                                  const std::vector<std::string>& countries);

#endif // CONNECTION_H
