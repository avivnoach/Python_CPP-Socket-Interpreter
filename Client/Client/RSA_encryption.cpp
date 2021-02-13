#include "cryptopp/rsa.h"
#include "cryptopp/base64.h"
#include "cryptopp/files.h"
#include "cryptopp/filters.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/pem.h"
#include <iostream>

void encrypt()
{
	CryptoPP::AutoSeededRandomPool prng{};

	std::string plain{ "Text123" };

	/**
	 * Read public key in PEM format.
	 */
	CryptoPP::FileSource fs{ "public.pem", /*pumpAll=*/true };
	CryptoPP::RSA::PublicKey publicKey{};
	PEM_Load(fs, publicKey);

	std::string encrypted{};

	/**
	 * Pump plain text through RSA encryption scheme with OAEP/SHA1 padding.
	 *
	 * In general, manual memory allocations should be avoided. However,
	 * the CryptoPP API expects us to allocate memory for each transformer and
	 * then pass the pointer to the next transformer, which then takes ownership
	 * of the pointer and will free it. This design obviously predates
	 * modern C++ smart pointers, which should be preferred when possible.
	 */
	CryptoPP::RSAES_OAEP_SHA_Encryptor cipher{ publicKey };
	auto* encoder{
		new CryptoPP::Base64Encoder{new CryptoPP::StringSink{encrypted}} };
	auto* encryptor{ new CryptoPP::PK_EncryptorFilter{prng, cipher, encoder} };
	CryptoPP::StringSource ss{ plain, /*pumpAll=*/true, encryptor };

	std::cout << encrypted;
}