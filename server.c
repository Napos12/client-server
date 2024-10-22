#include "functionclientserver.c"



char server_port[100];
char server_address[100];
char root_directory[100];
msg messaggio;





void check_arg_s(int argc, char* argv[])
{
    if(argc<7)
    {
        printf("argomenti mancanti\n");
        exit(-1);    
    }
    else
    {
        if(strcmp(argv[1],"-a")!=0 || strcmp(argv[3],"-p")!=0 || strcmp(argv[5],"-d")!=0 )
        {
            printf("Utilizzare formato: MyFTserver -a server_address -p server_port -d ft_root_directory\n");
            exit(-1);
        }
        else
        {
            
            strcpy(server_address,argv[2]);
            strcpy(server_port,argv[4]);
            strcpy(root_directory,argv[6]);
            
        }
    }
}


int main(int argc, char* argv[])
{
    int sock,portnu;
    struct sockaddr_in server,client;
    int len;
    int current_sock;
    int opt=1;


    check_arg_s(argc,argv);


    portnu=atoi(server_port);

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket error");
        exit(-1); 
    }

    server.sin_family= AF_INET;
    server.sin_port=htons(portnu);


    if(inet_pton(AF_INET, server_address, &server.sin_addr) <= 0)
    {
     perror("Errore nella conversione dell'indirizzo IP");
    } 
    // Imposta l'opzione SO_REUSEADDR per il socket in modo che e' possibile riutilizzare l'indirzzo che e' stato impostato
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))<0) 
    {
        perror("setsockopt");
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEPORT");
    close(sock);
    exit(-1);
    }

    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("Binding error");
        close(sock);
        exit(-1);
    }

    if (listen(sock, 4) == -1) {
        perror("Listen error");
        close(sock);
        exit(-1);
    }

    printf("Server attivo alla porta %d, attendo connessioni...\n", portnu);

    len = sizeof(client);
    while (1) {
        
        int client_sock = accept(sock, (struct sockaddr*)&client, (socklen_t *)&len);
        
        if (client_sock < 0) {
            perror("Accept error");
            exit(-1);
        }

        printf("Connessione ricevuta...\n");


        if(recv(client_sock,&messaggio,sizeof(messaggio),0)<0)
        {
            perror("read error");
            exit(-1);
        }
        printf("messaggio: %d,%s,%s",messaggio.mode,messaggio.path,messaggio.file_name);
        

    }






}