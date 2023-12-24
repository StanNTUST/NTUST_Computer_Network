#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_SIZE 1024

using namespace std;

int main() {

	string response;
	string internetAddress;
	string sender;
	string recipient;
	string emailSubject;
	string title;
	string emailBody = "";
	string text;
	string emailEnd = "\r\n.\r\n";
	string emailData;

	cout << "please input sender's internet address: ";
	cin >> internetAddress;

	cout << "please input sender's email address: ";
	cin >> sender;

	cout << "please input recipient's email address: ";
	cin >> recipient;

	cout << "please input email title: ";
	cin >> title;
	emailSubject = "Subject: " + title + "\r\n";

	cout << "please input message, end with <Ctrl+D>" << endl;

	while (!cin.eof()) {
		getline(cin, text);
		emailBody = emailBody + text + "\r\n";

	}
	emailBody += "\r\n";

	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		perror("Socket creation failed");
		return 1;
	}

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(25); // SMTP port
	serverAddr.sin_addr.s_addr = inet_addr(internetAddress.c_str()); // Replace with your SMTP server address

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("Connection failed");
		return 1;
	}
	else {
		cout << "Connected to the SMTP server" << endl;
	}

	response.resize(MAX_SIZE);

	recv(clientSocket, &response[0], response.capacity(), 0);
	cout << "Received: " << response << endl;


	emailData = "MAIL FROM: <" + sender + ">\r\n";
	cout << "Sending: " << emailData;
	send(clientSocket, emailData.c_str(), emailData.size(), 0);
	recv(clientSocket, &response[0], response.capacity(), 0);
	cout << "Received: " << response << endl;

	emailData = "RCPT TO: <" + recipient + ">\r\n";
	cout << "Sending: " << emailData;
	send(clientSocket, emailData.c_str(), emailData.size(), 0);
	recv(clientSocket, &response[0], response.capacity(), 0);
	cout << "Received: " << response << endl;

	emailData = "DATA\r\n";
	cout << "Sending: " << emailData;
	send(clientSocket, emailData.c_str(), emailData.size(), 0);
	recv(clientSocket, &response[0], response.capacity(), 0);

	send(clientSocket, emailSubject.c_str(), emailSubject.size(), 0);
	send(clientSocket, emailBody.c_str(), emailBody.size(), 0);
	send(clientSocket, emailEnd.c_str(), emailEnd.size(), 0);
	cout << "Received: " << response << endl;

	emailData = "QUIT\r\n";
	cout << "Sending: " << emailData;
	send(clientSocket, emailData.c_str(), emailData.size(), 0);
	recv(clientSocket, &response[0], response.capacity(), 0);
	cout << "Received: " << response << endl;

	close(clientSocket);
	cout << "connection is closed" << endl;
}
