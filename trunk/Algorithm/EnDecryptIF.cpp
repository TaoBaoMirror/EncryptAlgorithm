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

#ifndef ENDECRYPTIF_H
#include "EnDecryptIF.h"
#endif

#include <string.h>
//#include "openssl\\evp.h"
//#pragma comment( lib, "..\\Algorithm\\ssl\\lib\\libeay32.lib")

EnDecryptIF* EnDecryptIF::m_pInstance = NULL;

EnDecryptIF::EnDecryptIF():
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

EnDecryptIF::~EnDecryptIF()
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
EnDecryptIF::GetDES()
{
	if (NULL == m_pDES) {
		m_pDES = new DES;
	}
	return m_pDES;
}

AES* 
EnDecryptIF::GetAES()
{
	if (NULL == m_pAES) {
		m_pAES = new AES;
	}
	return m_pAES;
}

EnDecryptIF* 
EnDecryptIF::Instance(void)
{
	if (NULL == m_pInstance) {
		m_pInstance = new EnDecryptIF;
	}
	return m_pInstance;
}

void 
EnDecryptIF::Destroy()
{
	if (NULL != m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool 
EnDecryptIF::Encrypt(char *dataIn, long dataInlen, char *dataOut)
{
	return EncryptDecrypt(ENCRYPTDECRYPT_ENCRYPT, dataIn, dataInlen, dataOut);
}

bool 
EnDecryptIF::Decrypt(char *dataIn, long dataInlen, char *dataOut)
{
	return EncryptDecrypt(ENCRYPTDECRYPT_DECRYPT, dataIn, dataInlen, dataOut);
}


//加解密  由于不同加密解决方式不一样, 为统一接口请在加解密前根据不同的加解密算法设置相应数据
bool 
EnDecryptIF::EncryptDecrypt(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
{
//	OpenSslSymmetryExample();
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
EnDecryptIF::EncryptDecryptSymmetryDES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
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
EnDecryptIF::EncryptDecryptSymmetryAES(ENCRYPTDECRYPT_TYPE eEncrypt, char *dataIn, long dataInlen, char *dataOut)
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
EnDecryptIF::SetSymmetryKey(const char* chSymmetryKey, long lKeyLen)
{
	if (lKeyLen > STATIC_CONST_LONG_SYMMETRYKEYLENGTH) {
		return;
	}
	memset(m_chSymmetryKey, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYKEYLENGTH);
	memcpy(m_chSymmetryKey, chSymmetryKey, lKeyLen);
}

void 
EnDecryptIF::SetSymmetryDesInitValue(const char* chSymmetryDesInitValue, long lInitValueLen)
{
	if (lInitValueLen > STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH) {
		return;
	}
	memset(m_chSymmetryDesInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH);
	memcpy(m_chSymmetryDesInitValue, chSymmetryDesInitValue, lInitValueLen);
}


bool 
EnDecryptIF::OpenSslSymmetryExample()
{
	return false;
// 	int                                ret,which=1;
// 	EVP_CIPHER_CTX             ctx;
// 	const EVP_CIPHER      *cipher;
// 	unsigned char        key[24],iv[8],in[100],out[108],de[100];
// 	int                                i,len,inl,outl,total=0;
// 	for(i=0;i<24;i++) {
// 		memset(&key[i],i,1);
// 	}
// 	for(i=0;i<8;i++) {
// 		memset(&iv[i],i,1);
// 	}
// 	for(i=0;i<100;i++) {
// 		memset(&in[i],i,1);
// 	}
// 
// 	EVP_CIPHER_CTX_init(&ctx);
// 	printf("please select :\n");
// 	printf("1: EVP_des_ede3_ofb\n");
// 	printf("2: EVP_des_ede3_cbc\n");
// 
// 	scanf("%d",&which);
// 	if(which==1)
// 		cipher=EVP_des_ede3_ofb();
// 	else
// 		cipher=EVP_des_ede3_cbc(); 
// 
// 	ret=EVP_EncryptInit_ex(&ctx,cipher,NULL,key,iv);
// 	if(ret!=1) {
// 		printf("EVP_EncryptInit_ex err1!\n");
// 		return false;
// 	}
// 
// 	inl=50;
// 	len=0;
// 	EVP_EncryptUpdate(&ctx,out+len,&outl,in,inl);
// 
// 	len+=outl;
// 	EVP_EncryptUpdate(&ctx,out+len,&outl,in+50,inl);
// 
// 	len+=outl;
// 	EVP_EncryptFinal_ex(&ctx,out+len,&outl);
// 
// 	len+=outl;
// 	printf("加密结果长度：%d\n",len);
// 
// 	/* 解密 */
// 	EVP_CIPHER_CTX_cleanup(&ctx);
// 	EVP_CIPHER_CTX_init(&ctx);
// 	ret=EVP_DecryptInit_ex(&ctx,cipher,NULL,key,iv);
// 
// 	if(ret!=1) {
// 		printf("EVP_DecryptInit_ex err1!\n");
// 		return false;
// 	}
// 
// 	total=0;
// 	EVP_DecryptUpdate(&ctx,de+total,&outl,out,44);
// 	total+=outl;
// 	EVP_DecryptUpdate(&ctx,de+total,&outl,out+44,len-44);
// 	total+=outl;
// 	ret=EVP_DecryptFinal_ex(&ctx,de+total,&outl);
// 	total+=outl;
// 	if(ret!=1) {
// 		EVP_CIPHER_CTX_cleanup(&ctx);
// 		printf("EVP_DecryptFinal_ex err\n");
// 		return false;
// 	}
// 
// 	if((total!=100) || (memcmp(de,in,100))) {
// 		printf("err!\n");
// 		return false;
// 	}
// 
// 	EVP_CIPHER_CTX_cleanup(&ctx);
// 	printf("test ok!\n");
// 	return true;
}

bool 
EnDecryptIF::OpenSslUnsymmetryExample()
{
	return false;
}

bool 
EnDecryptIF::OpenSslHashExample()
{
	return false;
}
/* EOF */