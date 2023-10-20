#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ping() {
    char ip[20];
    printf("Entrez l'adresse IP de la machine distante : ");
    scanf("%s", ip);

    char command[100];
    sprintf(command, "ping -c 22 %s", ip);

    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'exécution de la commande ping.\n");
        exit(1);
    }

    char buffer[1024];
    int received_packets = 0;
    float average_time = 0.0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "time=") != NULL) {
            char* start = strstr(buffer, "time=") + 5;
            char* end = strchr(start, ' ');
            *end = '\0';
            printf("Temps de réponse : %s ms\n", start);
        } else if (strstr(buffer, "packets transmitted") != NULL) {
            char* start = strstr(buffer, "received,") - 2;  // Backtrack 2 characters to capture the number
            received_packets = atoi(start);
        } else if (strstr(buffer, "rtt min/avg/max/mdev") != NULL) {
            char* start = strstr(buffer, "/") - 5;  // Backtrack 5 characters to capture the number before the slash
            char* second_slash = strchr(start + 6, '/');  // Find the second slash for average time
            average_time = atof(start + 6);  // Convert average time to float
            *second_slash = '\0';
        }
    }

    printf("Paquets reçus : %d%% (sur 4 paquets envoyés)\n", received_packets * 25);  // As we are sending 4 packets
    printf("Délais aller-retour moyen : %.2f ms\n", average_time);

    pclose(fp);
}


int main() {
    ping();
    return 0;
}


