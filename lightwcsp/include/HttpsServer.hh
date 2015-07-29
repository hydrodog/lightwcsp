#ifndef HTTPS_SERVER_HH
#define HTTPS_SERVER_HH
#include <iostream>
#include <HttpRequest.hh>
#include "openssl/ssl.h"
#include "openssl/err.h"
class HttpsServer{
	SSL_CTX *ctx;
	int serverSock;
	int OpenListener();
	void InitServerCTX();
	void LoadCertificates(char* CertFile, char* KeyFile);
public:
	void ShowCerts(SSL* ssl);
	HttpsServer();
	~HttpsServer();
	void run();
};
#endif