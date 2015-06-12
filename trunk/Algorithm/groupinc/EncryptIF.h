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

	enum ENCRYPTSYMMETRYDESMODE_TYPE 
	{
		ENCRYPTSYMMETRYDESMODE_NON = 0x00,
		ENCRYPTSYMMETRYDESMODE_ECB,
		ENCRYPTSYMMETRYDESMODE_CBC,
		ENCRYPTSYMMETRYDESMODE_CFB,
		ENCRYPTSYMMETRYDESMODE_OFB,
		ENCRYPTSYMMETRYDESMODE_CTR,

		ENCRYPTSYMMETRYDESMODE_ALL,
		ENCRYPTSYMMETRYDESMODE_INVALID = -1,
	};
	void SetEncryptSymmetryDESMode(ENCRYPTSYMMETRYDESMODE_TYPE eEncryptSymmetryDESMode) {m_eEncryptSymmetryDESMode = eEncryptSymmetryDESMode;}
	ENCRYPTSYMMETRYDESMODE_TYPE GetEncryptSymmetryDESMode() {return m_eEncryptSymmetryDESMode;}

	void SetSymmetryKey(const char* chSymmetryKey);
	void SetSymmetryDesInitValue(const char* chSymmetryDesInitValue);
protected:
	EncryptIF();

private:
	DES* GetDES();
	bool EncryptDecrypt(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut);
	bool EncryptDecryptSymmetryDES(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut);
	bool EncryptDecryptSymmetryAES(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut);
public:
protected:
private:
	

	static EncryptIF* m_pInstance;
	ENCRYPT_TYPE m_eEncrypt;
	ENCRYPTSYMMETRY_TYPE m_eEncryptSymmetry;
	ENCRYPTSYMMETRYDES_TYPE m_eEncryptSymmetryDES;
	ENCRYPTSYMMETRYDESMODE_TYPE m_eEncryptSymmetryDESMode;
	
	static const long STATIC_CONST_LONG_SYMMETRYKEYLENGTH = 128;
	char m_chSymmetryKey[STATIC_CONST_LONG_SYMMETRYKEYLENGTH];
	static const long STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH = 9;	//DES加密 8位初始向量/计数值
	char m_chSymmetryDesInitValue[STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH]; //DES加密 8位初始向量/计数值
	DES* m_pDES;
	
	static const bool STATIC_CONST_BOOL_ENCRYPT;	//encrypt
	static const bool STATIC_CONST_BOOL_DECRYPT;	//decrypt
};

#endif // ENCRYPTIF_H