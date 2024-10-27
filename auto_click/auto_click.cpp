#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>

#define HOTKEY_ID_1 1 // ID горячей клавиши 1
#define HOTKEY_ID_2 2 // ID горячей клавиши 2

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

void start_long_click()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Нажатие левой кнопки мыши
}

void stop_long_click()
{
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Отпускание левой кнопки мыши
}

bool check_hot_key(const MSG& msg, int hotkey_id)
{
    return (msg.message == WM_HOTKEY && msg.wParam == hotkey_id);
}

int main()
{
    setlocale(0, "");

    // Регистрация горячих клавиш
    if (RegisterHotKey(NULL, HOTKEY_ID_1, MOD_CONTROL | MOD_SHIFT, 0x41)) // Ctrl + Shift + A
    {
        std::cout << "Горячая клавиша зарегистрирована. Нажмите Ctrl + Shift + A для автоклика.\n";
    }
    else
    {
        std::cerr << "Не удалось зарегистрировать горячую клавишу Ctrl + Shift + A.\n";
    }

    if (RegisterHotKey(NULL, HOTKEY_ID_2, MOD_CONTROL | MOD_SHIFT, 0x42)) // Ctrl + Shift + B
    {
        std::cout << "Горячая клавиша зарегистрирована. Нажмите Ctrl + Shift + B для долгого клика.\n";
    }
    else
    {
        std::cerr << "Не удалось зарегистрировать горячую клавишу Ctrl + Shift + B.\n";
    }

    MSG msg = { 0 }; // Храним сообщения от Windows

    while (GetMessage(&msg, NULL, 0, 0)) // Получаем сообщения от Windows
    {
        if (check_hot_key(msg, HOTKEY_ID_1)) // Если нажата горячая клавиша 1
        {
            if (!flag) // Если автоклик не запущен
            {
                std::cout << "Запуск автоклика...\n";
                start_auto_click(); // Запускаем автоклик
            }
            else
            {
                std::cout << "Остановка автоклика...\n";
                stop_auto_click(); // Останавливаем автоклик
            }
        }
        else if (check_hot_key(msg, HOTKEY_ID_2)) // Если нажата горячая клавиша 2
        {

            // Проверка не работает, подумаю потом уже

            if (!flag) // Если долгий клик не запущен
            {
                std::cout << "Запуск долгого клика...\n";
                start_long_click(); // Запускаем долгий клик
            }
            else
            {
                std::cout << "Остановка долгого клика...\n";
                stop_long_click(); // Останавливаем долгий клик
            }
        }
    }

    // Освобождаем ресурсы после завершения программы
    UnregisterHotKey(NULL, HOTKEY_ID_1);
    UnregisterHotKey(NULL, HOTKEY_ID_2);

    return 0;
}
