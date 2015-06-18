/**
@(#)$Id$
* @(#)
*
* (c)  XIAOTING PRIVATE PERSON  2014
* All Rights Reserved. QQ:451292510 Email:xiaoting_chen2010@163.com
*/

/**
@brief  Log IF
@Author $Author$
@Date $Date$
@Revision $Revision$
@URL $URL$
@Header $Header$
*/

#ifndef ENCRYPTIF_H
#define ENCRYPTIF_H

#ifndef DES_H
#include "DES.h"
#endif

#ifndef AES_H
#include "AES.h"
#endif

class EncryptIF
{
public:
	static EncryptIF* Instance(void);
	static void Destroy();

	//加解密 由于不同加密解决方式不一样, 为统一接口请在加解密前根据不同的加解密算法设置相应数据
	bool Encrypt(char *dataIn, long dataInlen, char *dataOut);
	bool Decrypt(char *dataIn, long dataInlen, char *dataOut);

	~EncryptIF();

	enum ENCRYPT_TYPE 
	{
		ENCRYPT_NON = 0x00,
		ENCRYPT_SYMMETRY,
		ENCRYPT_UNSYMMETRY,
		ENCRYPT_HASH,

		ENCRYPT_ALL,
		ENCRYPT_INVALID = -1,
	};
	void SetEncrypt(ENCRYPT_TYPE eEncrypt) {m_eEncrypt = eEncrypt;}
	ENCRYPT_TYPE GetEncrypt() {return m_eEncrypt;}

	enum ENCRYPTSYMMETRYMODE_TYPE 
	{
		ENCRYPTSYMMETRYMODE_NON = 0x00,
		ENCRYPTSYMMETRYMODE_ECB,
		ENCRYPTSYMMETRYMODE_CBC,
		ENCRYPTSYMMETRYMODE_CFB,
		ENCRYPTSYMMETRYMODE_OFB,
		ENCRYPTSYMMETRYMODE_CTR,

		ENCRYPTSYMMETRYMODE_ALL,
		ENCRYPTSYMMETRYMODE_INVALID = -1,
	};
	void SetEncryptSymmetryMode(ENCRYPTSYMMETRYMODE_TYPE eEncryptSymmetryDESMode) {m_eEncryptSymmetryMode = eEncryptSymmetryDESMode;}
	ENCRYPTSYMMETRYMODE_TYPE GetEncryptSymmetryMode() {return m_eEncryptSymmetryMode;}

	enum ENCRYPTSYMMETRY_TYPE 
	{
		ENCRYPTSYMMETRY_NON = 0x00,
		ENCRYPTSYMMETRY_DES,
		ENCRYPTSYMMETRY_AES,

		ENCRYPTSYMMETRY_ALL,
		ENCRYPTSYMMETRY_INVALID = -1,
	};
	void SetEncryptSymmetry(ENCRYPTSYMMETRY_TYPE eEncryptSymmetry) {m_eEncryptSymmetry = eEncryptSymmetry;}
	ENCRYPTSYMMETRY_TYPE GetEncryptSymmetry() {return m_eEncryptSymmetry;}

	enum ENCRYPTSYMMETRYDES_TYPE 
	{
		ENCRYPTSYMMETRYDES_NON = 0x00,
		ENCRYPTSYMMETRYDES_DES,
		ENCRYPTSYMMETRYDES_3DES,

		ENCRYPTSYMMETRYDES_ALL,
		ENCRYPTSYMMETRYDES_INVALID = -1,
	};
	void SetEncryptSymmetryDES(ENCRYPTSYMMETRYDES_TYPE eEncryptSymmetryDES) {m_eEncryptSymmetryDES = eEncryptSymmetryDES;}
	ENCRYPTSYMMETRYDES_TYPE GetEncryptSymmetryDES() {return m_eEncryptSymmetryDES;}

	enum ENCRYPTSYMMETRYAESKEYSIZE_TYPE 
	{
		ENCRYPTSYMMETRYAESKEYSIZE_NON = 0x00,
		ENCRYPTSYMMETRYAESKEYSIZE_BIT128,
		ENCRYPTSYMMETRYAESKEYSIZE_BIT192,
		ENCRYPTSYMMETRYAESKEYSIZE_BIT256,

		ENCRYPTSYMMETRYAESKEYSIZE_ALL,
		ENCRYPTSYMMETRYAESKEYSIZE_INVALID = -1,
	};
	void SetEncryptSymmetryAESKeySize(ENCRYPTSYMMETRYAESKEYSIZE_TYPE eEncryptSymmetryAESKeySize) {m_eEncryptSymmetryAESKeySize = eEncryptSymmetryAESKeySize;}
	ENCRYPTSYMMETRYAESKEYSIZE_TYPE GetEncryptSymmetryAESKeySize() {return m_eEncryptSymmetryAESKeySize;}

	void SetSymmetryKey(const char* chSymmetryKey, long lKeyLen);
	void SetSymmetryDesInitValue(const char* chSymmetryDesInitValue, long lInitValueLen);
protected:
	EncryptIF();

private:
	DES* GetDES();
	AES* GetAES();
	enum ENCRYPTDECRYPT_TYPE
	{
		ENCRYPTDECRYPT_ENCRYPT,
		ENCRYPTDECRYPT_DECRYPT,
	};
	bool EncryptDecrypt(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut);
	bool EncryptDecryptSymmetryDES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut);
	bool EncryptDecryptSymmetryAES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut);
public:
	static const long STATIC_CONST_LONG_SYMMETRYKEYLENGTH = 32;				//DES加密 16位密码 AES加密 32位密码
	static const long STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH = 16;	//DES加密 8位初始向量/计数值 AES加密 16位初始向量/计数值
protected:
private:
	static EncryptIF* m_pInstance;
	ENCRYPT_TYPE m_eEncrypt;
	ENCRYPTSYMMETRYMODE_TYPE m_eEncryptSymmetryMode;
	ENCRYPTSYMMETRY_TYPE m_eEncryptSymmetry;
	ENCRYPTSYMMETRYDES_TYPE m_eEncryptSymmetryDES;
	ENCRYPTSYMMETRYAESKEYSIZE_TYPE m_eEncryptSymmetryAESKeySize;
	
	char m_chSymmetryKey[STATIC_CONST_LONG_SYMMETRYKEYLENGTH];
	char m_chSymmetryDesInitValue[STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH];
	DES* m_pDES;
	AES* m_pAES;
};

#endif // ENCRYPTIF_H