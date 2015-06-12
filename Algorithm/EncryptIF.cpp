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

const bool EncryptIF::STATIC_CONST_BOOL_ENCRYPT = false;
const bool EncryptIF::STATIC_CONST_BOOL_DECRYPT = true;
EncryptIF* EncryptIF::m_pInstance = NULL;

EncryptIF::EncryptIF():
m_eEncrypt(ENCRYPT_NON),
m_eEncryptSymmetry(ENCRYPTSYMMETRY_NON),
m_eEncryptSymmetryDES(ENCRYPTSYMMETRYDES_NON),
m_eEncryptSymmetryDESMode(ENCRYPTSYMMETRYDESMODE_NON),
m_pDES(NULL)
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
}

DES* 
EncryptIF::GetDES()
{
	if (NULL == m_pDES) {
		m_pDES = new DES;
	}
	return m_pDES;
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
	return EncryptDecrypt(STATIC_CONST_BOOL_ENCRYPT, dataIn, dataInlen, dataOut);
}

bool 
EncryptIF::Decrypt(char *dataIn, long dataInlen, char *dataOut)
{
	return EncryptDecrypt(STATIC_CONST_BOOL_DECRYPT, dataIn, dataInlen, dataOut);
}


//加解密  由于不同加密解决方式不一样, 为统一接口请在加解密前根据不同的加解密算法设置相应数据
bool 
EncryptIF::EncryptDecrypt(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	switch(m_eEncrypt) {
		case ENCRYPT_SYMMETRY:
			switch(m_eEncryptSymmetry) {
				case ENCRYPTSYMMETRY_DES:
					return EncryptDecryptSymmetryDES(bEncrypt, dataIn, dataInlen, dataOut);
				case ENCRYPTSYMMETRY_AES:
					return EncryptDecryptSymmetryAES(bEncrypt, dataIn, dataInlen, dataOut);
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
EncryptIF::EncryptDecryptSymmetryDES(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	//文件大小变化: 加密->增加0~8个字节 解密->减少加0~8个字节
	bool bEncryptDES = STATIC_CONST_BOOL_ENCRYPT==bEncrypt?DES::DES_ENCRYPT:DES::DES_DECRYPT;
	DES* pDES = GetDES();
	switch (m_eEncryptSymmetryDES) {
		case ENCRYPTSYMMETRYDES_DES:
			switch(m_eEncryptSymmetryDESMode) {
				case ENCRYPTSYMMETRYDESMODE_ECB:					
					return pDES->ECB(dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CBC:
					return pDES->CBC(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CFB:
					return pDES->CFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_OFB:
					return pDES->OFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CTR:
					return pDES->CTR(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 8, bEncryptDES);
				default:
					return false;
			}
			return false;
		case ENCRYPTSYMMETRYDES_3DES:
			switch(m_eEncryptSymmetryDESMode) {
				case ENCRYPTSYMMETRYDESMODE_ECB:
					return pDES->ECB(dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CBC:
					return pDES->CBC(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CFB:
					return pDES->CFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_OFB:
					return pDES->OFB(m_chSymmetryDesInitValue,dataOut, dataIn, dataInlen, m_chSymmetryKey, 16, bEncryptDES);
				case ENCRYPTSYMMETRYDESMODE_CTR:
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

bool 
EncryptIF::EncryptDecryptSymmetryAES(bool bEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
	return false;
}

void 
EncryptIF::SetSymmetryKey(const char* chSymmetryKey)
{
	memset(m_chSymmetryKey, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYKEYLENGTH);
	strncpy(m_chSymmetryKey, chSymmetryKey, STATIC_CONST_LONG_SYMMETRYKEYLENGTH-1);
}

void 
EncryptIF::SetSymmetryDesInitValue(const char* chSymmetryDesInitValue)
{
	memset(m_chSymmetryDesInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH);
	strncpy(m_chSymmetryDesInitValue, chSymmetryDesInitValue, STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH-1);
}
/* EOF */