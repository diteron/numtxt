#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#include <Ws2tcpip.h>

#define BUFF_SIZE 1024

int main()
{
    setlocale(LC_ALL, "Rus");
    // Инициализация использования системы сокетов
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 2);
    if (WSAStartup(dllVersion, &wsaData) != 0) {
        std::cout << "Ошибка инициализации Winsock!\n";
        return 1;
    }

    // Структура адреса сокета
    SOCKADDR_IN addr;
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &(addr.sin_addr.s_addr));
    int addrLen = sizeof(addr);

    // Создание сокета для соединения с сервером
    SOCKET sockConnect = socket(AF_INET, SOCK_STREAM, NULL);
    if (sockConnect == INVALID_SOCKET) {
        std::cout << "Ошибка создания сокета\n";
        return 1;
    }
    if (connect(sockConnect, (SOCKADDR*) &addr, addrLen) != 0) {
        std::cout << "Ошибка соединения с сервером!\n";
        return 1;
    }
    std::cout << "Соединение с сервером успешно установлено\n";

    // Отправка серверу строки с числом и получение числа прописью
    char buffer[BUFF_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        std::cin.clear();
        fflush(stdin);
        std::cin.getline(buffer, sizeof(buffer));
        if (strcmp(buffer, "exit") == 0) { break; }

        if (send(sockConnect, buffer, sizeof(buffer), NULL) == SOCKET_ERROR) {
            std::cout << "Ошибка отправки данных в сокет\n";
            continue;
        }
        if (recv(sockConnect, buffer, sizeof(buffer), NULL) == SOCKET_ERROR) {
            std::cout << "Ошибка получения данных из сокета\n";
            continue;
        }

        std::cout << buffer << "\n";
        std::cout << "====================================================================" << "\n";
        Sleep(10);
    }

    shutdown(sockConnect, SD_BOTH);
    closesocket(sockConnect);
    WSACleanup();
    return 0;
}
