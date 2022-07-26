#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>

#define MAX_STR 4096
#define BYTES_PER_MSG 4096

using namespace std;

bool exit_flag=false;
thread t_send, t_recv;
int client_socket;

//enviar mensagem avisando que o ctrl+c esta desativado
void blockCrtl_C(int signal) {
	cout<<("Ctrl + c nao esta disponivel")<<endl;
	cout<<("Se deseja sair do chat utilize o comando /quit ou CTRL+D")<<endl;
}

//apagar o texto do terminal
void eraseText(int len){
	char delete_ = 8;
	for(int i = 0; i < len; i++)
		cout<<delete_;
}

//dividir as mensagens em blocos de ate 4096
char* create_block(char* msg, int current_block){
    //+1 pra incluir o '\0'
    char* msg_block = (char*)malloc(sizeof(char)*(BYTES_PER_MSG+1));
    int i = 1;
    int pos = current_block*BYTES_PER_MSG;
    msg_block[0] = msg[pos];
    
    while(msg[pos + i] != '\n' && msg[pos + i] != '\r' && msg[pos + i] != '\0' && i%BYTES_PER_MSG != 0){
        msg_block[i] = msg[pos + i];
        i++;
    }
    msg_block[i] = '\0';
    return msg_block;
}

//enviar mensagens
void send_message(int client_socket){
    char* server_message = NULL;
    char* block = NULL;
    while(1){
        cout << "Voce: ";
        server_message = NULL;
        block = NULL;
        int res = scanf(" %m[^\n\r]", &server_message);

        //para sair com ctrl+d
        if(res == -1){
            cout << "Saindo..." << endl;
            send(client_socket, "/quit",5,0);
            exit_flag=true;
            t_recv.detach();	
            close(client_socket);
            return;
        }else{
            int message_size = 0;
            int n_blocks = 0;

            while(server_message[message_size] != '\0') {
                if(message_size%BYTES_PER_MSG == 0){
                    n_blocks++;
                }
                message_size++;
            }

            //enviando os blocos
            int j = 0;
            for(int i = 0; i < n_blocks; i++){
                block = create_block(server_message, i);
                while(block[j] != '\0') j++;

                send(client_socket, block,j,0);
                if((strncmp(block,"/quit", 5) == 0) || (int)block[0] == 0){
                    cout << "Saindo..." << endl;
                    exit_flag=true;
                    t_recv.detach();	
                    close(client_socket);
                    return;
                }
                free(block);
            }
        }
        free(server_message);
    }
}

//recber as mensagens
void recv_message(int client_socket){
	while(1){
		if(exit_flag)
			return;
		
        char name[MAX_STR], str[BYTES_PER_MSG];
		int maisDeUmUsuario;
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		
        if(bytes_received<=0)
			continue;
		
        recv(client_socket,&maisDeUmUsuario,sizeof(maisDeUmUsuario),0);		
        recv(client_socket,str,sizeof(str),0);
        eraseText(6);

        if(strcmp(name,"#KICK") == 0){
            cout << "Saindo..." << endl;
            cout << "Saindo..." << endl;
            send(client_socket, "/quit",5,0);
            exit_flag=true;
            t_recv.detach();	
            close(client_socket);
            exit(0);
        }

        if(strcmp(name,"#NULL")!=0)
			cout<<name<<": "<<str<<endl;
		else
			cout<<str<<endl;
            
		cout<<"Voce: ";
		fflush(stdout);
	}	
}




int main(){
    char username[MAX_STR] = {'\0'};
    char channel_name[MAX_STR] = {'\0'}; //canal
    int op = 0;
	client_socket = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(9002); 
	client.sin_addr.s_addr = INADDR_ANY;

    //ignorar ctrl+c
	signal(SIGINT, blockCrtl_C);
    signal(SIGQUIT,blockCrtl_C);
    
    int conectando = connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in));
    if (conectando == -1) {
        cout << "Erro: connectando, reinicie o servidor" << endl;
        return EXIT_FAILURE;
    }

    while(op == 0 && !exit_flag){
        char conexao [7] = {'\0'};
        cout<<"\nDigite /connect se deseja se estabelecr uma conexao com o servidor"<<endl;
        cin >> conexao;
        

        if((strncmp(conexao,"/quit", 5) == 0) || (int)conexao[0] == 0){
            cout << "Saindo..." << endl;
            exit_flag = true;
			close(client_socket);
			exit(0);
  	    }

        if(strcmp(conexao,"/connect") == 0){
            cout<<"\nPara determianr o seu nome de usuario digite: /nickname seuNomeUsuario"<<endl;
            
            while(1){
                char nickname[9] = {'\0'};
                cin >> nickname;
                if((strncmp(nickname,"/quit", 5) == 0) || (int)nickname[0] == 0){
                    cout << "Saindo..." << endl;
			        close(client_socket);
			        exit(0);
  	            }
                
                if(strcmp(nickname,"/nickname") == 0){
                    cin >> username;
                    if(strlen(username) > 49 || strlen(username) < 2){
                        cout << "Nome de usuario tem que ser menor que 50 e maio que 2 caracteres" << endl;
                        cout << "Digite novamente /nickname seuNomeUsuario para determinar o seu nome de usuário" << endl;
                        memset(username, 0, strlen(username));
                    }else
                        break;
                }else
                    cout << "\nComando invalido, tente novamente" << endl;
            }

            if((strncmp(conexao,"/quit", 5) == 0) || (int)conexao[0] == 0){
                cout << "Saindo..." << endl;
                exit_flag = true;
                close(client_socket);
                exit(0);
  	        }

            while(1){
                cout << "Digite /join nomeDoCanal para entrar em um canal\n";
                char join[5] = {'\0'};
                cin >> join;
                if((strncmp(join,"/quit", 5) == 0) || (int)join[0] == 0){
                    cout << "Saindo..." << endl;
			        close(client_socket);
			        exit(0);
  	            }
                  
                if(strcmp(join,"/join") == 0){
                    cin >>channel_name;
                    break;
                }else
                    cout << "\nComando invalido, tente novamente" << endl;
            }
            
            
            //enviar o nome de usuario para o servidor
            send(client_socket,username,sizeof(username),0);
            send(client_socket,channel_name,sizeof(channel_name),0); //canal

            thread send_thread(send_message, client_socket);
            thread recv_thread(recv_message, client_socket);

            t_send = move(send_thread);
            t_recv = move(recv_thread);

            if(t_send.joinable())
                t_send.join();
            if(t_recv.joinable())
                t_recv.join();
        }else{
            cout << "Nao foi possivel estabelecer uma conexao\nSe deseja tentar novamente digite 0, se deseja sair digite 1" <<endl;
            cin >> op;
        }   
    }
    	
    return 0;
}









