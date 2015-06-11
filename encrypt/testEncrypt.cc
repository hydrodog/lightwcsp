#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

void print(ostream& s, const char msg[], unsigned char text[], int n) {
  s << msg << "\tlength =" << n << '\n';
	for(int i = 0;i < n;i++)
	  cout << hex << setw(2) << setfill('0') << (unsigned int)text[i];
	cout << dec <<'\n';
}

int main(int argc, char *argv[])
{
 	if (argc < 2) {
		cerr << "usage: testEncrypt n [output]\n";
		exit(-1);
	}

	const int n = atoi(argv[1]);
	
	char* outputFile=nullptr;
	if (argc > 2) {
		outputFile  = argv[2];
	}
  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */
  /* A 256 bit key */
  unsigned char *key = (unsigned char*)"01234567890123456789012345678901";
  /* A 128 bit IV */
  unsigned char *iv = (unsigned char*)"01234567890123456";
  /* Message to be encrypted */
	unsigned int* data = new unsigned int[n+4];
  for (int i =0;i<n;i++) data[i]=i+1;
	const int n4 = sizeof(int)*n;
  unsigned char *plaintext = (unsigned char*)data;
  //   "The quick brown fox jumps over the lazy dog";
  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */

  unsigned char ciphertext[n4+16];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[n4];

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);


	print(cout, "plaintext", plaintext, n4);
  /* Encrypt the plaintext */
  int ciphertext_len = encrypt(plaintext, n4, key, iv, ciphertext);
	print(cout, "encrypted", ciphertext, n4);

  int decryptedtext_len =decrypt(ciphertext,ciphertext_len,key,iv,plaintext);
	print(cout, "decrypted", plaintext, n4);
  
	if(outputFile!=nullptr){
		ofstream f(outputFile); // write out encrypted data
		f<<"key: "<<key<<endl;
		f<<"iv: "<<iv<<endl;
	
		print(f, "ciphertext", ciphertext, n4);
		print(f, "plaintext", plaintext, n4);
	}
  /* Clean up */
	//  EVP_cleanup();
	// ERR_free_strings();

  return 0;
}	
