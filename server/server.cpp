#include <iostream>
#include <cstring>
#include "numtxtconverter.h"

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
        std::cout << "Ошибка инициализации Winsock\n";
        return 1;
    }

    // Структура адреса сокета
    SOCKADDR_IN addr;
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &(addr.sin_addr.s_addr));
    int addrLen = sizeof(addr);

    // Создание сокета и привязка адреса
    SOCKET sockListen = socket(AF_INET, SOCK_STREAM, NULL);
    if (sockListen == INVALID_SOCKET) {
        std::cout << "Ошибка создания сокета\n";
        return 1;
    }
    if (bind(sockListen, (SOCKADDR*) &addr, addrLen) != 0) {
        closesocket(sockListen);
        std::cout << "Ошибка при привязке сокета\n";
        return 1;
    }
    if (listen(sockListen, SOMAXCONN) != 0) {
        std::cout << "Ошибка при включении режима прослушивания\n";
        closesocket(sockListen);
        return 1;
    }

    // Основной цикл сервера
    SOCKET sockClient = INVALID_SOCKET;
    while (true) {
        // Сокет для соединения с клиентом
        sockClient = accept(sockListen, (SOCKADDR*) &addr, &addrLen);
        if (sockClient == INVALID_SOCKET) {
            std::cout << "Ошибка соединения с клиентом!\n";
            continue;
        }
        std::cout << "Соединение с новым клиентом успешно установлено\n";

        // Обслуживание соединения (получение строки с числом и вывод строки прописью)
        NumTxtConverter converter;
        char buffer[BUFF_SIZE];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            if (recv(sockClient, buffer, sizeof(buffer), NULL) == SOCKET_ERROR) {
                std::cout << "Соединение с клиентом разорвано\n";
                break;
            }
            std::string numNxt = converter.decimalToText(buffer);
            strcpy_s(buffer, sizeof(buffer), numNxt.c_str()); // Сохранение результата конвертации в буфер
            
            std::cout << buffer << "\n";
            std::cout << "====================================================================" << "\n";
            if (send(sockClient, buffer, sizeof(buffer), NULL) == SOCKET_ERROR) {
                std::cout << "Ошибка отправки данных в сокет\n";
                break;
            }
        }
        shutdown(sockClient, SD_BOTH);
        closesocket(sockClient);
        sockClient = INVALID_SOCKET;
    }

    shutdown(sockListen, SD_BOTH);
    Sleep(100);
    closesocket(sockListen);
    WSACleanup();
    return 0;
}
