#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __SETTINGSPATH__ "settings/settings.ini"
#define __MAXCHAR__ 1000

int settings_template_handler();
char *settings_file_search(char *setting);
void add_computer(char *computer, char *mac_address);

int settings_template_handler()
{
    FILE *file_ptr;
    file_ptr = fopen(__SETTINGSPATH__, "r");
    int i;

    if (file_ptr == NULL)
    {
        mkdir("settings");
        // printf("\"settings.ini\" file not detected or could not be oppened. Creating a new file...\n");
        if ((file_ptr = fopen(__SETTINGSPATH__, "w")))
        {
            /* it is written like this in case I need to expand the template in the future.*/
            const char settings_template[3][__MAXCHAR__] = {"BROADCAST = 255.255.255.255\nPORT = 9\n[SAVED COMPUTERS]"};

            for (i = 0; i < 1; i++)
                fputs(settings_template[i], file_ptr);

            fclose(file_ptr);

            return 1;
        }
        else
        {
            fclose(file_ptr);
            return 0;
        }
    }
    else
    {
        if (settings_file_search("BROADCAST") != NULL && settings_file_search("PORT") != NULL)
            return 1;

        else
        {
            printf("Settings file may be incorrectly formatted. Fix or delete the settings file in \"settings\\settings.ini\". Exiting the program...");

            exit(-1);

            return 0;
        }
    }
}

char *settings_file_search(char *setting)
{
    FILE *file_ptr;
    file_ptr = fopen(__SETTINGSPATH__, "r");

    char setting_buffer[__MAXCHAR__];
    char *token;
    unsigned char found_flag = 0;

    while (fgets(setting_buffer, __MAXCHAR__, file_ptr))
    {
        setting_buffer[strcspn(setting_buffer, "\n")] = 0;
        token = strtok(setting_buffer, " = ");
        if ((strcmp(setting, token)) == 0)
        {
            token = strtok(NULL, " = ");
            found_flag = 1;
            break;
        }
    }

    fclose(file_ptr);

    if (found_flag)
    {
        return token;
    }
    else
        return NULL;
}

void add_computer(char *computer, char *mac_address)
{
    int i;
    FILE *file_ptr;
    file_ptr = fopen(__SETTINGSPATH__, "a");
    char computer_str[4][__MAXCHAR__] = {"\n", "", " = ", "",};

    strcpy(computer_str[1], computer);
    strcpy(computer_str[3], mac_address);

    for (i = 0; i < 4; i++)
        fputs(computer_str[i], file_ptr);

    fclose(file_ptr);
}