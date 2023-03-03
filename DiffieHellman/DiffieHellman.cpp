#include <openssl/bn.h>

#include <cinttypes>
#include <cmath>
#include <iostream>
#include <memory>

#include "DesAlgo.h"

class Client {
public:
  // Constructor
  Client(BIGNUM *prime, BIGNUM *primitive_root, BN_CTX *ctx)
      : m_ctx(ctx), m_p(BN_new()), m_g(BN_new()), m_secretKey(BN_new()) {
    BN_copy(m_p, prime);
    BN_copy(m_g, primitive_root);

    BN_rand_range(m_secretKey, m_p);
  }

  ~Client() {
    BN_free(m_p);
    BN_free(m_g);
    BN_free(m_secretKey);
  }

  void getPublicKey(BIGNUM *pc) {
    BN_mod_exp(pc, m_g, m_secretKey, m_p, m_ctx);
  }

  void setOtherPublicKey(BIGNUM *otherPublicKey) {
    auto sharedKey = BN_new();
    BN_mod_exp(sharedKey, otherPublicKey, m_secretKey, m_p, m_ctx);
    auto secretKeySize = BN_num_bytes(sharedKey);
    auto secretKeyData = std::vector<char>(secretKeySize, char(0));
    BN_bn2bin(sharedKey,
              reinterpret_cast<unsigned char *>(secretKeyData.data()));
    secretKeyData.resize(8);
    memcpy(m_sharedKey, secretKeyData.data(), 8);
    BN_free(sharedKey);
  }

  std::vector<char> encryptMessage(const std::string& message)
  {
      auto data = std::vector<char>{ message.begin(), message.end() };
      return DES::encrypt(data, m_sharedKey);
  }

  std::string decryptMessage(const std::vector<char>& message)
  {
      auto data = DES::decrypt(message, m_sharedKey);
      return std::string{ data.begin(), data.end() };
  }

private:
  BN_CTX *m_ctx;
  BIGNUM *m_p;
  BIGNUM *m_g;
  BIGNUM *m_secretKey;
  char m_sharedKey[8];
};

int main() {
  BN_CTX *ctx = BN_CTX_new();

  BIGNUM *bn_p = BN_new();
  BIGNUM *bn_g = BN_new();
  BN_dec2bn(&bn_p, "1066340417491710595814572169");
  BN_dec2bn(&bn_g, "99194853094755497");

  auto A = Client{bn_p, bn_g, ctx};
  auto B = Client{bn_p, bn_g, ctx};
  BN_free(bn_p);
  BN_free(bn_g);

  BIGNUM* bn_tmp = BN_new();                         
  // обмен ключами
  B.getPublicKey(bn_tmp);
  A.setOtherPublicKey(bn_tmp);
  A.getPublicKey(bn_tmp);
  B.setOtherPublicKey(bn_tmp);
  BN_free(bn_tmp);

  auto messageAB = "Hello, B!";
  std::cout << "Message A -> B: " << messageAB << std::endl; 
  auto encryptedMessageAB = A.encryptMessage(messageAB);
  std::cout << "Encrypted message A -> B: " << std::string{ encryptedMessageAB.data(), encryptedMessageAB.size() } << std::endl;
  std::cout << "Decrypted message A -> B: " << B.decryptMessage(encryptedMessageAB) << std::endl;
  std::cout << std::endl;
  auto messageBA = "Hello, A!";
  std::cout << "Message B -> A: " << messageBA << std::endl;
  auto encryptedMessageBA = B.encryptMessage(messageBA);
  std::cout << "Encrypted message B -> A: " << std::string{ encryptedMessageBA.data(), encryptedMessageBA.size() } << std::endl;
  std::cout << "Decrypted message B -> A: " << B.decryptMessage(encryptedMessageBA) << std::endl;
}