//////////////////////////////////////////////////////////////////////////
//
//// file_server.c -- socketæä¶ä èæå¡å¨ç¯çºää£ç
//
//// /////////////////////////////////////////////////////////////////////
//
//
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>

#include<stdio.h>

#include<stdlib.h>

#include<string.h>



#define HELLO_WORLD_SERVER_PORT    6666

#define LENGTH_OF_LISTEN_QUEUE     20

#define BUFFER_SIZE                1024

#define FILE_NAME_MAX_SIZE         512



int main(int argc, char **argv)
{

    // set socket's address information

    // è¾ç®ääªsocketå°åçæserver_addr,ä£è¨æå¡å¨internetçå°ååç¯å£

    struct sockaddr_in   server_addr;

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = htons(INADDR_ANY);

    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

    // create a stream socket

    // ååºç¨äinternetçæåè®(TCP)socketïç¨server_socketä£è¨æå¡å¨åå¢æ·ç¯æäæå¡çæ¥å£

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if(server_socket < 0)
    {
        printf("Create Socket Failed!");

        exit(1);
    }

    // æsocketåsocketå°åçæçå

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        printf("Server Bind Port: %d Failed!"
               , HELLO_WORLD_SERVER_PORT);
        exit(1);
    }
    // server_socketç¨äçå¬
    if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed!");
        exit(1);
    }

    // æå¡å¨ç¯äç´èèç¨ä¥æç­äºå¢æ·ç¯æäæå¡

    while(1)
    {
        // åäå¢æ·ç¯çsocketå°åçæclient_addrïåæ¶å°æ¥èªå¢æ·ç¯çè·æåïèç¨accept
        // æ¥åæ¤è·æïåæ¶åclientç¯çå°ååç¯å£çä¡æ¯åå¥client_addrä­
        struct sockaddr_in client_addr;

        socklen_t          length = sizeof(client_addr);

        // æ¥åääªäclientç¯å°è¾serverç¯çèæ¥è·æ,åå¢æ·ç¯çä¡æ¯äåå¨client_addrä­

        // åææ¡æèæ¥è·æïåäç´çåç´å°æèæ¥è·æäºæ¢ïèæ¯acceptå½æ°çç¹æ§ïå¯ä¥

        // ç¨select()æ¥åç°èæ¶ææ

        // accpetèåääªæ°çsocket,èäªsocketç¨æ¥äæ¤æ¡èæ¥å°serverçclientèèéä¡

        // èéçnew_server_socketä£è¨äèäªéä¡éé

        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);

        if(new_server_socket < 0)
        {
            printf("Server Accept Failed!");
            break;
        }
        else
        {
            const char* clientip =  inet_ntoa(client_addr.sin_addr);
            printf("client ip is %s\n",clientip);
        }
        char buffer[BUFFER_SIZE];

        bzero(buffer, sizeof(buffer));
        
        send(new_server_socket, argv[1], strlen(argv[1]) + 1, 0);

        FILE *fp = fopen(argv[1], "r");

        if(fp == NULL)
        {
            printf("File:    %s Not Found!", argv[1]);
        }

        else
        {
            bzero(buffer, BUFFER_SIZE);

            int file_block_length = 0;

            while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
            {
                //printf("file_block_length = %d", file_block_length);

                // åébufferä­çåç¦ä²å°new_server_socket,åéäå±æ¯åéçå¢æ·ç¯

                if(send(new_server_socket, buffer, file_block_length, 0) < 0)
                {
                    printf("Send File:   %s Failed!"
                           , argv[1]);
                    break;
                }
                bzero(buffer, sizeof(buffer));
            }

            fclose(fp);

            printf("File:    %s Transfer Finished!", argv[1]);
        }
        close(new_server_socket);
    }

    close(server_socket);

    return 0;
}

//////////////////////////////////////////////////////

// file_client.c  socketä èæä¶çclientç¯çºäçå

// ///////////////////////////////////////////////////



