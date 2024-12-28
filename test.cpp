#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "connection.h"

TEST_SUITE_BEGIN("GeographicalConnections");

TEST_CASE("testing loadCountries") {
    std::ofstream outFile("test_countries.txt");
    outFile << "Россия\nСША\nКитай\nГермания\n";
    outFile.close();

    auto countries = loadCountries("test_countries.txt");
    CHECK(countries.size() == 4);
    CHECK(countries[0] == "Россия");
    CHECK(countries[1] == "США");
    CHECK(countries[2] == "Китай");
    CHECK(countries[3] == "Германия");

    std::remove("test_countries.txt");
}

TEST_CASE("testing loadMatrix") {
    std::ofstream outFile("test_matrix.txt");
    outFile << "0 1 0 1\n"
            << "1 0 1 0\n"
            << "0 1 0 1\n"
            << "1 0 1 0\n";
    outFile.close();

    auto matrix = loadMatrix("test_matrix.txt");
    CHECK(matrix.size() == 4);
    CHECK(matrix[0] == std::vector<int>{0, 1, 0, 1});
    CHECK(matrix[1] == std::vector<int>{1, 0, 1, 0});
    CHECK(matrix[2] == std::vector<int>{0, 1, 0, 1});
    CHECK(matrix[3] == std::vector<int>{1, 0, 1, 0});

    std::remove("test_matrix.txt");
}

TEST_CASE("testing removeConnection") {
    std::vector<std::vector<int>> matrix = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}};

    removeConnection(matrix, 0, 3);
    CHECK(matrix[0][3] == 0);
    CHECK(matrix[3][0] == 0);
}

TEST_CASE("testing findShortestRoute") {
    std::vector<std::vector<int>> matrix = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0}};

    std::unordered_set<int> targets = {3};
    auto route = findShortestRoute(matrix, 0, targets);

    CHECK(route == std::vector<int>{0, 4, 3});
}

TEST_CASE("testing checkConnectionsToContinents") {
    std::vector<std::string> countries = {"Россия", "США", "Китай", "Германия", "Япония"};
    std::unordered_map<std::string, std::vector<int>> continents = {
        {"Asia", {0, 2, 4}},
        {"Europe", {3}},
        {"North America", {1}}};

    std::vector<std::vector<int>> matrix = {
        {0, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}};

    std::ostringstream oss;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(oss.rdbuf());

    checkConnectionsToContinents(matrix, 0, continents, countries);

    std::cout.rdbuf(oldCoutBuffer);

    std::string output = oss.str();
    CHECK(output.find("Страна Россия имеет доступ к континенту Asia") != std::string::npos);
}

TEST_SUITE_END();
