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
#include "stdafx.h"

#ifndef ENCRYPTIF_H
#include "EncryptIF.h"
#endif

EncryptIF* EncryptIF::m_pInstance = NULL;

EncryptIF::EncryptIF():
m_eEncrypt(ENCRYPT_NON),
m_eEncryptSymmetryMode(ENCRYPTSYMMETRYMODE_NON),
m_eEncryptSymmetry(ENCRYPTSYMMETRY_NON),
m_eEncryptSymmetryDES(ENCRYPTSYMMETRYDES_NON),
m_eEncryptSymmetryAESKeySize(ENCRYPTSYMMETRYAESKEYSIZE_NON),
m_pDES(NULL),
m_pAES(NULL)
{
	memset(m_chSymmetryKey, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYKEYLENGTH);
	memset(m_chSymmetryDesInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH);
}

EncryptIF::~EncryptIF()
{
	if (NULL != m_pDES) {
		delete m_pDES;
		m_pDES = NULL;
	}
	if (NULL != m_pAES) {
		delete m_pAES;
		m_pAES = NULL;
	}
}

DES* 
EncryptIF::GetDES()
{
	if (NULL == m_pDES) {
		m_pDES = new DES;
	}
	return m_pDES;
}

AES* 
EncryptIF::GetAES()
{
	if (NULL == m_pAES) {
		m_pAES = new AES;
	}
	return m_pAES;
}

EncryptIF* 
EncryptIF::Instance(void)
{
	if (NULL == m_pInstance) {
		m_pInstance = new EncryptIF;
	}
	return m_pInstance;
}

void 
EncryptIF::Destroy()
{
	if (NULL != m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool 
EncryptIF::Encrypt(char *dataIn, long dataInlen, char *dataOut)
{
	return EncryptDecrypt(ENCRYPTDECRYPT_ENCRYPT, dataIn, dataInlen, dataOut);
}

bool 
EncryptIF::Decrypt(char *dataIn, long dataInlen, char *dataOut)
{
	return EncryptDecrypt(ENCRYPTDECRYPT_DECRYPT, dataIn, dataInlen, dataOut);
}


//加解密  由于不同加密解决方式不一样, 为统一接口请在加解密前根据不同的加解密算法设置相应数据
bool 
EncryptIF::EncryptDecrypt(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	switch(m_eEncrypt) {
		case ENCRYPT_SYMMETRY:
			switch(m_eEncryptSymmetry) {
				case ENCRYPTSYMMETRY_DES:
					return EncryptDecryptSymmetryDES(eEncrypt, dataIn, dataInlen, dataOut);
				case ENCRYPTSYMMETRY_AES:
					return EncryptDecryptSymmetryAES(eEncrypt, dataIn, dataInlen, dataOut);
					return false;
				default:
					return false;
			}
			return false;
		case ENCRYPT_UNSYMMETRY:
			return false;
		case ENCRYPT_HASH:
			return false;
		default:
			return false;
	}
	return false;
}

bool 
EncryptIF::EncryptDecryptSymmetryDES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	//文件大小变化: 加密->增加0~8个字节 解密->减少加0~8个字节
	bool bEncryptDES = ENCRYPTDECRYPT_ENCRYPT==eEncrypt?DES::DES_ENCRYPT:DES::DES_DECRYPT;
	DES* pDES = GetDES();
	switch (m_eEncryptSymmetryDES) {
		case ENCRYPTSYMMETRYDES_DES:
			switch(m_eEncryptSymmetryMode) {
				case ENCRYPTSYMMETRYMODE_ECB:					
					return pDES->ECB(dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CBC:
					return pDES->CBC(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CFB:
					return pDES->CFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_OFB:
					return pDES->OFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CTR:
					return pDES->CTR(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				default:
					return false;
			}
			return false;
		case ENCRYPTSYMMETRYDES_3DES:
			switch(m_eEncryptSymmetryMode) {
				case ENCRYPTSYMMETRYMODE_ECB:
					return pDES->ECB(dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CBC:
					return pDES->CBC(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CFB:
					return pDES->CFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_OFB:
					return pDES->OFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYMODE_CTR:
					return pDES->CTR(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				default:
					return false;
			}
			return false;
		default:
			return false;
	}
	return false;
}
AES* pAEStest = NULL; 

bool 
EncryptIF::EncryptDecryptSymmetryAES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	AES::AES_ENCRYPTDECRYPT_TYPE eAESEncrypt = ENCRYPTDECRYPT_ENCRYPT==eEncrypt?AES::AES_ENCRYPTDECRYPT_ENCRYPT:AES::AES_ENCRYPTDECRYPT_DECRYPT;
	AES::AES_KEYSIZE eAESKeySize = AES::AES_KEYSIZE_NON;
	switch (m_eEncryptSymmetryAESKeySize) {
		case ENCRYPTSYMMETRYAESKEYSIZE_BIT128:
			eAESKeySize = AES::AES_KEYSIZE_BIT128;
			break;
		case ENCRYPTSYMMETRYAESKEYSIZE_BIT192:
			eAESKeySize = AES::AES_KEYSIZE_BIT192;
			break;
		case ENCRYPTSYMMETRYAESKEYSIZE_BIT256:
			eAESKeySize = AES::AES_KEYSIZE_BIT256;
			break;
		default:
			eAESKeySize = AES::AES_KEYSIZE_INVALID;
			break;
	}
	AES* const pAES = GetAES();
	switch(m_eEncryptSymmetryMode) {
		case ENCRYPTSYMMETRYMODE_ECB:
			return pAES->ECB(dataOut, dataIn, dataInlen, m_chSymmetryKey, eAESKeySize, eAESEncrypt);
		case ENCRYPTSYMMETRYMODE_CBC:
			return pAES->CBC(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, eAESKeySize, eAESEncrypt);
		case ENCRYPTSYMMETRYMODE_CFB:
			return pAES->CFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, eAESKeySize, eAESEncrypt);
		case ENCRYPTSYMMETRYMODE_OFB:
			return pAES->OFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, eAESKeySize, eAESEncrypt);
		case ENCRYPTSYMMETRYMODE_CTR:
			return pAES->CTR(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, eAESKeySize, eAESEncrypt);
		default:
			return false;
	}
	return false;
}

void 
EncryptIF::SetSymmetryKey(const char* chSymmetryKey, long lKeyLen)
{
	if (lKeyLen > STATIC_CONST_LONG_SYMMETRYKEYLENGTH) {
		return;
	}
	memset(m_chSymmetryKey, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYKEYLENGTH);
	memcpy(m_chSymmetryKey, chSymmetryKey, lKeyLen);
}

void 
EncryptIF::SetSymmetryDesInitValue(const char* chSymmetryDesInitValue, long lInitValueLen)
{
	if (lInitValueLen > STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH) {
		return;
	}
	memset(m_chSymmetryDesInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH);
	memcpy(m_chSymmetryDesInitValue, chSymmetryDesInitValue, lInitValueLen);
}
/* EOF */