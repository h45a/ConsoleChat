/*
    UDP Client routine
*/
#include "UDPSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h> // Для inet_pton

int main(int argc, char* argv[]) {
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    unsigned short srv_port = 0;
    char srv_ip_addr[40] = { 0 };

    // Создаём UDP сокет
    UDPSocket client_sock;

    // Настраиваем адрес сервера
    memset((char*)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;

    if (argc == 1) {
        si_other.sin_port = htons(PORT);
        inet_pton(AF_INET, SERVER, &si_other.sin_addr); // Используем inet_pton вместо inet_addr
        printf("1: Server - addr=%s , port=%d\n", SERVER, PORT);
    }
    else if (argc == 2) {
        si_other.sin_port = htons(atoi(argv[1]));
        inet_pton(AF_INET, SERVER, &si_other.sin_addr);
        printf("2: Server - addr=%s , port=%d\n", SERVER, atoi(argv[1]));
    }
    else {
        si_other.sin_port = htons(atoi(argv[2]));
        inet_pton(AF_INET, argv[1], &si_other.sin_addr);
        printf("3: Server - addr=%s , port=%d\n", argv[1], atoi(argv[2]));
    }

    // Основной цикл отправки сообщений
    while (1) {
        printf("\nEnter message: ");
        gets_s(message, BUFLEN);

        // Отправляем сообщение серверу
        client_sock.SendDatagram(message, (int)strlen(message), (struct sockaddr*)&si_other, slen);

        // Получаем ответ от сервера
        memset(buf, '\0', BUFLEN);
        client_sock.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);

        printf("Answer: %s\n", buf); // Добавлено "Answer:" перед ответом
    }

    return 0;
}
