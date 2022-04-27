#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "settings.h"

void wake_on_lan(char *mac_string, const char *port_string, const char *broadcast_string);

int main(int argc, char *argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);

    settings_template_handler();

    int i;

    const char *broadcast_in = strdup(settings_file_search("BROADCAST"));
    const char *port_in = strdup(settings_file_search("PORT"));
    char *saved_computer;

    if (argc < 2)
    {
        printf("No arguments passed. Use \".\\wol help\" for help.");
        exit(-1);
    }

    if (!(strcmp(argv[1], "HELP")) || !(strcmp(argv[1], "help")))
    {
        printf("Saving a computer: \".\\wol save [COMPUTER NAME] [MAC ADDRESS (format ab:cd:ef:gh:ij:kl)]\"\n");
        printf("Waking up a computer via MAC address: \".\\wol wake mac [MAC ADDRESS (format ab:cd:ef:gh:ij:kl)]\"\n");
        printf("Waking up a saved computer: \".\\wol wake [COMPUTER NAME]\"\n");
        exit(1);
    }

    else
    {
        if (argc >= 3)
        {
            if (!(strcmp(argv[1], "save")) || !(strcmp(argv[1], "SAVE")))
            {
                if ((argv[2]) && (argv[3]) && (strlen(argv[3]) >= 17))
                {
                    add_computer(argv[2], argv[3]);
                    printf("Saved \"%s\" as \"%s\" in settings.ini", argv[2], argv[3]);

                    exit(1);
                }

                printf("Mac address format may be incorrect. Remember to use \"ab:cd:ef:gh:ij:kl\". Use \".\\wol help\" for help.");
                exit(-1);
            }

            if (!(strcmp(argv[1], "wake")) || !(strcmp(argv[1], "WAKE")))
            {
                if ((!(strcmp(argv[2], "mac")) || !(strcmp(argv[2], "MAC"))) && (argv[3]))
                {
                    wake_on_lan(argv[3], port_in, broadcast_in);
                    printf("Waking up \"%s\"...", argv[3]);
                    exit(1);
                }

                if (settings_file_search(argv[2]))
                {
                    saved_computer = strdup(settings_file_search(argv[2]));
                    wake_on_lan(saved_computer, port_in, broadcast_in);
                    printf("Waking up \"%s\" (%s)...", argv[2], saved_computer);
                    exit(1);
                }

                else
                {
                    printf("Computer not found. Make sure the target machine is saved before waking it. Use \".\\wol help\" for help.");
                    exit(-1);
                }
            }

            printf("Incorrect arguments. Use \".\\wol help\" for help.");
            exit(-1);
        }

        else
        {
            printf("Not enough arguments passed. Use \".\\wol help\" for help.");
            printf("\narg1: %s ", argv[1]);
            printf("strcmp help: %d", strcmp(argv[1], "help"));
            printf("strcmp HELP: %d", strcmp(argv[1], "HELP"));
            exit(-1);
        }
    }

    return 0;
}

void wake_on_lan(char *mac_string, const char *port_string, const char *broadcast_string)
{
    int i, j;
    int port = atoi(port_string);
    char *mac_string_temp = malloc(sizeof(char) * 17);
    unsigned char mac[6], packet_buffer[102];
    char *token;

    // converting the mac string into an array of hexadecimal values

    strncpy(mac_string_temp, mac_string, sizeof(char) * 17);
    mac_string_temp[17] = '\0';

    token = strtok(mac_string_temp, ":");

    for (i = 0; i < 6; i++)
    {
        mac[i] = (unsigned char)strtol(token, NULL, 16);
        token = strtok(NULL, ":");
    }

    // packet building

    for (i = 0; i < 6; i++)
        packet_buffer[i] = 0xff;

    for (i = 1; i <= 16; i++)
    {
        for (j = 0; j < 6; j++)
        {
            packet_buffer[(i * 6) + j] = mac[j];
        }
    }

    // actual socket shenanigans

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in socket_in =
        {
            .sin_family = AF_INET,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(0)};

    int broadcast_bool = 666;

    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_bool, sizeof(broadcast_bool));

    bind(sock, (struct sockaddr *)&socket_in, sizeof(socket_in));

    struct sockaddr_in destination_in =
        {
            .sin_family = AF_INET,
            .sin_addr.s_addr = inet_addr(broadcast_string),
            .sin_port = htons(port)};

    sendto(sock, &packet_buffer, sizeof(packet_buffer), 0,
           (struct sockaddr *)&destination_in, sizeof(destination_in));
}