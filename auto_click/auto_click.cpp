#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>

#define HOTKEY_ID 1 // ID горячей клавиши

std::atomic<bool> flag{ false }; // Переменная для управления состоянием потока автоклика

void auto_click()
{
    while (flag) // Пока флаг включен
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Нажатие левой кнопки мыши
        Sleep(1000); // Пауза в 1 секунду
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Отпускание левой кнопки мыши
        Sleep(1000); // Пауза в 1 секунду
    }
}

void start_auto_click()
{
    flag = true; // Включаем флаг
    std::thread t(auto_click); // Запускаем поток для автоклика
    t.detach(); // Отсоединяем поток, чтобы он работал асинхронно
}

void stop_auto_click()
{
    flag = false; // Останавливаем выполнение автоклика
}

bool check_hot_key(const MSG& msg)
{
    // Проверяем, является ли сообщение горячей клавишей
    return (msg.message == WM_HOTKEY && msg.wParam == HOTKEY_ID);
}

int main()
{
    setlocale(0, "");

    if (RegisterHotKey(NULL, HOTKEY_ID, MOD_CONTROL | MOD_SHIFT, 0x41)) // Ctrl + Shift + A
    {
        std::cout << "Горячая клавиша зарегистрирована. Нажмите Ctrl + Shift + A для автонажатия мыши.\n";
    }
    else
    {
        std::cerr << "Не удалось зарегистрировать горячую клавишу.\n";
        return 1;
    }

    MSG msg = { 0 }; // Храним сообщения от Windows

    while (GetMessage(&msg, NULL, 0, 0)) // Получаем сообщения от Windows
    {
        if (check_hot_key(msg)) // Если горячая клавиша нажата
        {
            if (!flag) // Если автоклик не запущен
            {
                std::cout << "Запуск автоклика...\n";
                start_auto_click(); // Запускаем автоклик
            }
            else // Если автоклик уже запущен
            {
                std::cout << "Остановка автоклика...\n";
                stop_auto_click(); // Останавливаем автоклик
            }
        }
    }

    // Освобождаем ресурсы после завершения программы
    UnregisterHotKey(NULL, HOTKEY_ID);

    return 0;
}
