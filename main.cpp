#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Полный список стран мира
    std::vector<std::string> countries = {
            "Австралия", "Австрия", "Азербайджан", "Албания", "Алжир", "Ангола",
            "Андорра", "Антигуа и Барбуда", "Аргентина", "Армения", "Афганистан",
            "Багамы", "Бангладеш", "Барбадос", "Бахрейн", "Беларусь", "Белиз",
            "Бельгия", "Бенин", "Болгария", "Боливия", "Босния и Герцеговина",
            "Ботсвана", "Бразилия", "Бруней", "Буркина-Фасо", "Бурунди", "Бутан",
            "Вануату", "Ватикан", "Великобритания", "Венгрия", "Венесуэла",
            "Восточный Тимор", "Вьетнам", "Габон", "Гаити", "Гайана", "Гамбия",
            "Гана", "Гватемала", "Гвинея", "Гвинея-Бисау", "Германия", "Гондурас",
            "Гренада", "Греция", "Грузия", "Дания", "Джибути", "Доминика",
            "Доминиканская Республика", "Египет", "Замбия", "Зимбабве",
            "Израиль", "Индия", "Индонезия", "Иордания", "Ирак", "Иран", "Ирландия",
            "Исландия", "Испания", "Италия", "Йемен", "Кабо-Верде", "Казахстан",
            "Камбоджа", "Камерун", "Канада", "Катар", "Кения", "Кипр", "Киргизия",
            "Кирибати", "Китай", "Колумбия", "Коморы", "Конго", "Коста-Рика",
            "Кот-д’Ивуар", "Куба", "Кувейт", "Лаос", "Латвия", "Лесото", "Либерия",
            "Ливан", "Ливия", "Литва", "Лихтенштейн", "Люксембург", "Маврикий",
            "Мавритания", "Мадагаскар", "Малави", "Малайзия", "Мали", "Мальдивы",
            "Мальта", "Марокко", "Маршалловы Острова", "Мексика", "Мозамбик",
            "Молдова", "Монако", "Монголия", "Мьянма", "Намибия", "Науру",
            "Непал", "Нигер", "Нигерия", "Нидерланды", "Никарагуа", "Новая Зеландия",
            "Норвегия", "Объединенные Арабские Эмираты", "Оман", "Пакистан",
            "Палау", "Панама", "Папуа-Новая Гвинея", "Парагвай", "Перу", "Польша",
            "Португалия", "Республика Конго", "Республика Корея", "Россия",
            "Руанда", "Румыния", "Сальвадор", "Самоа", "Сан-Марино",
            "Сан-Томе и Принсипи", "Саудовская Аравия", "Северная Македония",
            "Сейшельские Острова", "Сенегал", "Сент-Винсент и Гренадины",
            "Сент-Китс и Невис", "Сент-Люсия", "Сербия", "Сингапур", "Сирия",
            "Словакия", "Словения", "Соломоновы Острова", "Сомали", "Судан",
            "Суринам", "США", "Сьерра-Леоне", "Таджикистан", "Таиланд", "Танзания",
            "Того", "Тонга", "Тринидад и Тобаго", "Тувалу", "Тунис", "Туркмения",
            "Турция", "Уганда", "Узбекистан", "Украина", "Уругвай", "Фиджи",
            "Филиппины", "Финляндия", "Франция", "Хорватия", "Центральноафриканская Республика",
            "Чад", "Черногория", "Чехия", "Чили", "Швейцария", "Швеция", "Шри-Ланка",
            "Эквадор", "Экваториальная Гвинея", "Эритрея", "Эсватини", "Эстония",
            "Эфиопия", "Южная Африка", "Южный Судан", "Ямайка", "Япония"
    };

    // Размер матрицы (количество стран)
    int n = countries.size();

    // Матрица смежности (изначально все элементы равны 0)
    std::vector<std::vector<int>> adjacencyMatrix(n, std::vector<int>(n, 0));

    // Заполнение матрицы диагональных элементов
    for (int i = 0; i < n; ++i) {
        adjacencyMatrix[i][i] = 1; // Диагональ всегда 1
    }

    // Ввод данных от пользователя
    std::string country1, country2;
    std::cout << "pervaya strana: ";
    std::getline(std::cin, country1);
    std::cout << "vtoraya strana: ";
    std::getline(std::cin, country2);

    // Поиск индексов стран
    auto it1 = std::find(countries.begin(), countries.end(), country1);
    auto it2 = std::find(countries.begin(), countries.end(), country2);

    if (it1 != countries.end() && it2 != countries.end()) {
        int index1 = it1 - countries.begin();
        int index2 = it2 - countries.begin();

        // Устанавливаем соседство
        adjacencyMatrix[index1][index2] = 1;
        adjacencyMatrix[index2][index1] = 1;

        std::cout << "Страны \"" << country1 << "\" и \"" << country2 << "\" теперь являются соседями.\n";
    } else {
        std::cout << "odna iz stran ne naydena v spiske\n";
    }

    // Вывод матрицы
    std::cout << "\nmatrica smejnosti:\n";
    for (const auto& row : adjacencyMatrix) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
