#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <sstream> // Добавлено для использования stringstream

using namespace std;

// Загрузка матрицы смежности из файла
vector<vector<int>> loadMatrix(const string& filePath) {
    ifstream file(filePath);
    vector<vector<int>> matrix;
    string line;

    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл." << endl;
        exit(1);
    }

    while (getline(file, line)) {
        vector<int> row;
        int value;
        stringstream ss(line);
        while (ss >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    return matrix;
}

// BFS для нахождения связных компонент
void bfs(int start, const vector<vector<int>>& graph, vector<bool>& visited, set<int>& component) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        component.insert(node);

        for (int i = 0; i < graph[node].size(); ++i) {
            if (graph[node][i] == 1 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
}

// Нахождение всех связных компонент
vector<set<int>> findConnectedComponents(const vector<vector<int>>& graph) {
    vector<bool> visited(graph.size(), false);
    vector<set<int>> components;

    for (int i = 0; i < graph.size(); ++i) {
        if (!visited[i]) {
            set<int> component;
            bfs(i, graph, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

int main() {
    // Путь к файлу
    string filePath = "connections_matrix.txt";

    // Загрузка матрицы смежности
    vector<vector<int>> graph = loadMatrix(filePath);

    // Карта соответствия стран и континентов
    unordered_map<int, string> continentMap = {
            {0, "Europe"}, {1, "Asia"}, {2, "Africa"}, {3, "North America"},
            {4, "South America"}, {5, "Australia"}, {6, "Antarctica"}
    };

    // Ввод двух стран для удаления связи
    int country1, country2;
    cout << "Введите номера двух стран (0-индексированных), между которыми нужно оборвать связь: ";
    cin >> country1 >> country2;

    // Удаление связи
    graph[country1][country2] = 0;
    graph[country2][country1] = 0;

    // Нахождение связных компонент
    vector<set<int>> components = findConnectedComponents(graph);

    // Вывод результирующих континентов
    cout << "Связанные континенты после разрыва связи:" << endl;
    for (const auto& component : components) {
        set<string> connectedContinents;
        for (int country : component) {
            connectedContinents.insert(continentMap[country]);
        }
        cout << "{ ";
        for (const auto& continent : connectedContinents) {
            cout << continent << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}