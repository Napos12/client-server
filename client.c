#include "functionclientserver.c"



cl_req cl;
void split_p(char* path_input, int type_path)
{
    char* f_n = &strrchr(path_input, '/')[1];
    printf("%s", f_n);
    size_t path_l = f_n - path_input;
    printf("%ld\n",path_l);
    char path_out[path_l];
    if((f_n != NULL) && strcmp(f_n, "") != 0){
        printf("Dentro if\n");
        strncpy(path_out,path_input,path_l);        // Copia il percorso del file in results
        printf("Copia riuscita\n");
        path_out[path_l] = '\0';                         // Aggiunge il terminatore, per indicare la fine della stringa
    } else {perror("Error, missing slash in the path or file name not specified (try to run again the command and after -o or -f specify a correct path)");}
    printf("Fuori if\n");
    if (type_path == 0) 
    {
        cl.remote_path = strdup(path_out);
        cl.remote_file_n = strdup(f_n);
    }
    else 
    {
        cl.local_path = strdup(path_out);
        cl.local_file_n = strdup(f_n);
    }
    printf("Fine funzione\n");
} 

void check_arg_c(int argc,char* argv[])
{
    if(argc<8)
    {
        printf("few arguments, retry\n");
        exit(-1 );
    }
    else
    {
        if(strcmp(argv[1],"-w")==0)
        {
           cl.write=1;
           cl.read=0;
           cl.list=0;
        }
        else if(strcmp(argv[1],"-r")==0)
        {
            cl.write=0;
            cl.read=1;
            cl.list=0;
        }
        else if(strcmp(argv[1],"-l")==0)
        {
            cl.write=0;
            cl.read=0;
            cl.list=1;
        }
        else
        {
            printf("format not supported\n");
            exit(-1);
        }

        if(strcmp(argv[2],"-a")!=0)
        {
            printf("daje roma\n");
            exit(-1);
        }
        else
        {
            cl.server_addr=argv[3];
        }

        if(strcmp(argv[4],"-p")!=0)
        {
            printf("daje roma 2\n");
            exit(-1);
        }
        else
        {
            
            cl.server_po=atoi(argv[5]);
        }
        //PROBLEMI
        if(strcmp(argv[6],"-f")!=0)
        {
            printf("daje roma 3\n");
            exit(-1);
        }
        else
        {
            printf("%s\n",argv[7]);
            if(cl.list!=1 && cl.read==1)
            {
                split_p(argv[7],0);
            }
            else if(cl.list!=1 && cl.write==1)
            {
                split_p(argv[7],1);
            }
            else
            {
                cl.remote_path=strdup(argv[7]);
            }         
        }

        if(argc>8)
        {
            if(strcmp(argv[8],"-o")!=0)
            {
                printf("formato error\n");
                exit(-1);
            }
            else
            {
                if(cl.read==1)
                {
                    split_p(argv[7],1);
                }
                else
                {
                    split_p(argv[7],0);
                }
            }
        }

    }
}


int main(int argc, char* argv[])
{
    int client_sock;

    struct sockaddr_in server_addr;

    msg messaggio;

    check_arg_c(argc, argv);
    printf("Controllo finito\n");

    client_sock=socket(AF_INET,SOCK_STREAM,0);
    if(client_sock<0)
    {
        perror("Socket error\n");
        exit(-1);
    }

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(cl.server_po);

    if(inet_pton(AF_INET, cl.server_addr, &server_addr.sin_addr) <= 0)
    {
        perror("Error converting IP address");
        exit(-1);
    }


    connect(client_sock,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(connect<0)
    {
        perror("connection error");
        exit(-1);
    }
    printf("Connessione riuscita\n");



    if(cl.read==1)
    {
        messaggio.mode=0;
    }
    else if(cl.write==1)
    {
        messaggio.mode=1;
    }
    else
    {
        messaggio.mode=2;
    }
        
    strcpy(messaggio.path,"dale");
    strcpy(messaggio.file_name,"gg");
        

    printf("%d\n",messaggio.mode);




    
    if(send(client_sock,&messaggio,sizeof(messaggio),0)<0)
    {
        perror("send error\n");
        exit(-1);
    }
    printf("messaggio inviato\n");
    close(client_sock);
    
    return 0;


}