/*
    UDP Server
*/
#include "UDPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h> // Для inet_ntop

int main(int argc, char* argv[]) {
    struct sockaddr_in si_other;
    unsigned short srvport;
    int slen;
    char buf[BUFLEN];
    char msg[BUFLEN];
    char client_ip[INET_ADDRSTRLEN]; // Для хранения IP-адреса клиента

    srvport = (argc == 1) ? PORT : atoi(argv[1]);

    // Создаём UDP сервер
    UDPServer server(srvport);
    slen = sizeof(si_other);

    // Основной цикл обработки запросов
    while (1) {
        printf("Waiting for data... ");
        fflush(stdout);

        // Очищаем буфер
        memset(buf, '\0', BUFLEN);

        // Получаем сообщение от клиента
        server.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);

        // Преобразуем IP-адрес клиента в строку
        inet_ntop(AF_INET, &si_other.sin_addr, client_ip, INET_ADDRSTRLEN);

        // Выводим данные клиента
        printf("\nReceived packet from %s:%d\n", client_ip, ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        // Отвечаем клиенту
        printf("Answer: ");
        gets_s(msg, BUFLEN);
        server.SendDatagram(msg, (int)strlen(msg), (struct sockaddr*)&si_other, slen);
    }

    return 0;
}
