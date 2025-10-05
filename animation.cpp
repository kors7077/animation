#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>
#include <algorithm>

/// <summary>
/// Получить список файлов анимации в директории
/// </summary>
/// <returns>Вектор с именами файлов анимации</returns>
std::vector<std::string> getAnimationFiles() {
    std::vector<std::string> animationFiles;

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA("animation*.txt", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                animationFiles.push_back(findFileData.cFileName);
            }
        } while (FindNextFileA(hFind, &findFileData));
        FindClose(hFind);
    }

    // Сортируем файлы для удобства
    std::sort(animationFiles.begin(), animationFiles.end());
    return animationFiles;
}

/// <summary>
/// Отобразить меню выбора анимации
/// </summary>
/// <param name="animationFiles">Список файлов анимации</param>
/// <returns>Выбранный пункт меню</returns>
int showMenu(const std::vector<std::string>& animationFiles) {
    std::cout << "=== ANIMATION SELECTION ===" << std::endl;
    std::cout << "Available animations:" << std::endl;

    // Выводим нумерованный список файлов анимации
    for (size_t i = 0; i < animationFiles.size(); ++i) {
        std::cout << i + 1 << ". " << animationFiles[i] << std::endl;
    }

    // Добавляем пункт для выхода
    std::cout << animationFiles.size() + 1 << ". Exit" << std::endl;
    std::cout << "===========================" << std::endl;

    int choice;
    while (true) {
        std::cout << "Select animation (1-" << animationFiles.size() + 1 << "): ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Error: please enter a number!" << std::endl;
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(animationFiles.size() + 1)) {
            break;
        }
        else {
            std::cout << "Error: please select a number from 1 to " << animationFiles.size() + 1 << "!" << std::endl;
        }
    }

    return choice;
}

/// <summary>
/// Получить содержимое файла
/// </summary>
/// <param name="filename">Имя считываемого файла</param>
/// <returns>Содержимое файла</returns>
std::string getFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    return content;
}

/// <summary>
/// Функция разделения строки по разделителю
/// </summary>
/// <param name="content">Исходная строка</param>
/// <param name="delimeter">Разделитель</param>
/// <returns>Список подстрок</returns>
std::vector<std::string> split(const std::string& content, const std::string& delimeter) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = content.find(delimeter);

    while (end != std::string::npos) {
        parts.push_back(content.substr(start, end - start));
        start = end + delimeter.length();
        end = content.find(delimeter, start);
    }
    parts.push_back(content.substr(start));
    return parts;
}

/// <summary>
/// Воспроизвести анимацию из файла
/// </summary>
/// <param name="filename">Имя файла с анимацией</param>
void playAnimation(const std::string& filename) {
    std::string content = getFileContent(filename);
    if (content.empty()) {
        std::cout << "Animation file is empty or not found!" << std::endl;
        return;
    }

    std::vector<std::string> frames = split(content, "FRAME");

    std::cout << "Starting animation: " << filename << std::endl;
    std::cout << "Number of frames: " << frames.size() << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {
        for (const std::string& frame : frames) {
            system("cls");
            std::cout << frame << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }
    }
}

int main() {
    // Получаем список файлов анимации
    std::vector<std::string> animationFiles = getAnimationFiles();

    if (animationFiles.empty()) {
        std::cout << "Animation files not found!" << std::endl;
        std::cout << "Place files with names 'animation*.txt' in current directory." << std::endl;
        system("pause");
        return 1;
    }

    while (true) {
        // Показываем меню и получаем выбор пользователя
        int choice = showMenu(animationFiles);

        // Проверяем, не выбран ли пункт "Выход"
        if (choice == static_cast<int>(animationFiles.size() + 1)) {
            std::cout << "Exiting program..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return 0; // Завершаем программу сразу
        }

        // Воспроизводим выбранную анимацию
        std::string selectedFile = animationFiles[choice - 1];
        playAnimation(selectedFile);

        // После завершения анимации (по Ctrl+C) возвращаемся в меню
        std::cout << std::endl << "Animation finished. Returning to menu..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}