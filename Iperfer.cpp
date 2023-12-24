#include <iostream>            /*  standard I/O  */
#include <stdlib.h>           /*  standard library */
#include <string.h>           /*  string library */
#include <sys/socket.h>       /*  socket definitions */
#include <sys/types.h>        /*  socket types       */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>
#include <time.h>
#include <iomanip>
#include <chrono>

#define MAXDATASIZE 1000 // max number of bytes we can get at once
#define  BACKLOG   10

using namespace std;

int main(int argc, char* argv[])
{
	int port, bytesReceived;
	char buffer[MAXDATASIZE];
	long long totalByte;
	double rate;
	try {
		if (strcmp(argv[1], "-s") == 0) {
			if (strcmp(argv[2], "-p") != 0) throw(string("Error: missing or additional arguments"));

			port = stoi(argv[3]);

			if (port < 1024 || port>65535)  throw(string("Error: port number must be in the range 1024 to 65535"));

			int  sockfd, clientSocket; // listen on sock_fd, new connection on new_fd
			struct sockaddr_in   my_addr; // my address information
			struct sockaddr_in   their_addr; // connector¡¦s address information
			socklen_t sin_size;
			if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				perror("socket");
				exit(1);
			}
			my_addr.sin_family = AF_INET; // host byte order
			my_addr.sin_port = htons(port); // short, network byte order
			my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
			memset(&(my_addr.sin_zero), 0, 8); // zero the rest of the struct

			if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1) {
				perror("bind");
				exit(1);
			}
			if (listen(sockfd, BACKLOG) == -1) {
				perror("listen");
				exit(1);
			}
			while (1) {
				sin_size = sizeof(struct sockaddr_in);
				if ((clientSocket = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size)) == -1) {
					perror("accept");
					continue;
				}

				cout << "Connected to the TCP " << endl;
				cout << endl;
				cout << "Start to test bandwidth..." << endl;

				totalByte = 0;
				
				auto startTime = chrono::steady_clock::now();
				while (1) {
					bytesReceived = recv(clientSocket, buffer, MAXDATASIZE, 0);

					if (bytesReceived <= 0) break;

					totalByte += bytesReceived;
					buffer[bytesReceived] = 0;
				}
				auto endTime = chrono::steady_clock::now();

				totalByte = totalByte / 1000;
				rate = (((double)totalByte / 1000) * 8) / (double(chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count())/1000);

				cout << "send: " << totalByte << " KB" << endl;
				cout << "rate: " << fixed << setprecision(3) << rate << " Mbps" << endl;

				close(clientSocket);
			}

		}
		else if (strcmp(argv[1], "-c") == 0) {
			if (strcmp(argv[2], "-h") != 0 || strcmp(argv[4], "-p") != 0 || strcmp(argv[6], "-t") != 0) throw(string("Error: missing or additional arguments"));

			string hostName = argv[3];
			port = stoi(argv[5]);
			double time = stod(argv[7]);
			int sockfd, numbytes;

			cout << port << endl;
			if (port < 1024 || port>65535)  throw(string("Error: port number must be in the range 1024 to 65535"));

			struct sockaddr_in server_addr; // connector¡¦s address information

			if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				perror("socket");
				exit(1);
			}

			server_addr.sin_family = AF_INET; // host byte order
			server_addr.sin_port = htons(port); // short, network byte order
			server_addr.sin_addr.s_addr = inet_addr(hostName.c_str());
			memset(&(server_addr.sin_zero), 0, 8); // zero the rest of the struct
			if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
				perror("connect");
				exit(1);
			}
			else {
				cout << "Connected to the server" << endl;
			}
			cout << endl;
			cout << "Start to test" << endl;
			
			totalByte = 0;
			
			auto startTime = chrono::steady_clock::now();
			while (1) {
				bytesReceived = send(sockfd, buffer, MAXDATASIZE - 1, 0);

				totalByte += bytesReceived;
				buffer[bytesReceived] = 0;
				auto endTime = chrono::steady_clock::now();
				if ((double(chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()) / 1000)  >= time) break;
				
			}
			auto endTime = chrono::steady_clock::now();

			totalByte = totalByte / 1000;
			rate = (((double)totalByte / 1000) * 8) / (double(chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()) / 1000);

			cout << "send: " << totalByte << " KB" << endl;
			cout << "rate: " << fixed << setprecision(3) << rate << " Mbps" << endl;
			close(sockfd);
			return 0;
		}
		else throw(string("Error: missing or additional arguments"));

	}
	catch (string error) {
		cout << error << endl;
	}
	catch (...) {
		cout << "Error: missing or additional arguments" << endl;
	}
	return 0;
}
