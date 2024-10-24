#include <windows.h>
#include <iostream>

// Константы для сочетания клавиш Ctrl + Shift + A
#define HOTKEY_ID 1 // ID горячей клавиши

void auto_click()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Нажатие левой кнопки мыши
    Sleep(1000); // Пауза в секунду
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Отпустили левую кнопку мыши
}

bool check_hot_key(const MSG &msg, bool &flag)
{
    // msg.message == WM_HOTKEY. Здесь мы проверяем, относится ли событие к горячим клавишам
    if (msg.message == WM_HOTKEY && msg.wParam == HOTKEY_ID) // Проверяем, соответствует ли нажатая кнопка HOTKEY_ID
        return !flag;

    return flag;
}

int main()
{
    bool flag = false;
    setlocale(0, "");

    if (RegisterHotKey(NULL, HOTKEY_ID, MOD_CONTROL | MOD_SHIFT, 0x41))
    {
        std::cout << "Горячая клавиша зарегистрирована. Нажмите Ctrl + Shift + A для автонажатия мыши." << std::endl;
    }
    else
    {
        std::cerr << "Не удалось зарегистрировать горячую клавишу." << std::endl;
        return 1;
    }

    MSG msg = { 0 }; // Храним сообщения от Windows
    while (GetMessage(&msg, NULL, 0, 0)) // Пока мы получаем какое-либо сообщение
    {
        while (check_hot_key(msg, flag))
        {
            auto_click(); 
        }
    }

    UnregisterHotKey(NULL, HOTKEY_ID);

    return 0;
}
