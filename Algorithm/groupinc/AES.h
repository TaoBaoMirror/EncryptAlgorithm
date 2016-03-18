/**
@(#)$Id$
* @(#)
*
* (c)  XIAOTING PRIVATE PERSON  2014
* All Rights Reserved. QQ:451292510 Email:xiaoting_chen2010@163.com
*/

/**
@brief aes���߼����ܱ�׼��
    �߼����ܱ�׼��Ӣ�Advanced Encryption Standard����д��AES����������ѧ���ֳ�Rijndael���ܷ�����������������
	���õ�һ��������ܱ�׼�������׼�������ԭ�ȵ�DES���Ѿ����෽�����ҹ�Ϊȫ������ʹ�á������������ѡ���̣�
	�߼����ܱ�׼���������ұ�׼�뼼���о�Ժ��NIST����2001��11��26�շ�����FIPS PUB 197������2002��5��26�ճ�Ϊ��Ч
	�ı�׼��2006�꣬�߼����ܱ�׼��Ȼ��Ϊ�Գ���Կ�����������е��㷨֮һ��
@Author $Author$
@Date $Date$
@Revision $Revision$
@URL $URL$
@Header $Header$
*/

#ifndef AES_H
#define AES_H

#pragma warning (disable:4996)	//ȡ������

#include <windows.h>
#pragma once
class AES
{
friend class EnDecryptIF;
public:
	enum AES_KEYSIZE
	{
		AES_KEYSIZE_NON = 0,
		AES_KEYSIZE_BIT128,
		AES_KEYSIZE_BIT192,
		AES_KEYSIZE_BIT256,

		AES_KEYSIZE_ALL,
		AES_KEYSIZE_INVALID
	};
	enum AES_ENCRYPTDECRYPT_TYPE
	{
		AES_ENCRYPTDECRYPT_ENCRYPT,
		AES_ENCRYPTDECRYPT_DECRYPT,
	};
public:
	bool ECB(char *Out, char *In, long datalen, const char *Key, AES_KEYSIZE keySize, AES_ENCRYPTDECRYPT_TYPE Type);
	bool CBC(char *IV, char *Out, char *In, long datalen, const char *Key, AES_KEYSIZE keySize, AES_ENCRYPTDECRYPT_TYPE Type);
	bool CFB(char *IV, char *Out, char *In, long datalen, const char *Key, AES_KEYSIZE keySize, AES_ENCRYPTDECRYPT_TYPE Type);
	bool OFB(char *IV, char *Out, char *In, long datalen, const char *Key, AES_KEYSIZE keySize, AES_ENCRYPTDECRYPT_TYPE Type);
	bool CTR(char *CNT, char *Out, char *In, long datalen, const char *Key, AES_KEYSIZE keySize, AES_ENCRYPTDECRYPT_TYPE Type);

	AES(AES_KEYSIZE keysize, BYTE *key);
// 	BOOL Decrypt(BYTE *lpInBuf,int nlen, BYTE *lpOutBuf);
// 	BOOL Encrypt(BYTE *lpInBuf,int nlen, BYTE *lpOutBuf);
	
	~AES(void);
	void Cipher( BYTE *input, BYTE *output );
	void InvCipher( BYTE *input, BYTE *output );
protected:
	AES();
	BYTE *RotWord( BYTE *word );
	BYTE *SubWord( BYTE *word );
	void AddRoundKey(int round);
	void SubBytes();
	void InvSubBytes();
	void ShiftRows();
	void InvShiftRows();
	void MixColumns();
	void InvMixColumns();
	 static BYTE gfmultby01(BYTE b)
    {
      return b;
    }

    static BYTE gfmultby02(BYTE b)
    {
      if (b < 0x80)
        return (BYTE)(int)(b <<1);
      else
        return (BYTE)( (int)(b << 1) ^ (int)(0x1b) );
    }

    static BYTE gfmultby03(BYTE b)
    {
      return (BYTE) ( (int)gfmultby02(b) ^ (int)b );
    }

    static BYTE gfmultby09(BYTE b)
    {
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)b );
    }

    static BYTE gfmultby0b(BYTE b)
    {
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(b) ^
                     (int)b );
    }

    static BYTE gfmultby0d(BYTE b)
    {
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(gfmultby02(b)) ^
                     (int)(b) );
    }

    static BYTE gfmultby0e(BYTE b)
    {
      return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(gfmultby02(b)) ^
                     (int)gfmultby02(b) );
    }
private:
	void SetKey(AES_KEYSIZE keysize, BYTE *key);

	void Xor(bool *InA, const bool *InB, int len);// ���
	void Xor(char *InA, const char *InB, int len);
	void RotateL(bool *In, int len, int loop);// ѭ������
	void RotateL(char *In, int len, int loop);
	void ByteToBit(bool *Out, const char *In, int bits);// �ֽ���ת����λ��
	void BitToByte(char *Out, const bool *In, int bits);// λ��ת�����ֽ���

public:
protected:
private:
	int Nb;
	int Nk;
	int Nr;
	BYTE *key;// the seed key. size will be 4 * keySize from ctor.
	typedef struct BYTE4_
	{
		BYTE w[4];
	}BYTE4;
	BYTE4 *w;
	LPBYTE State[4];
	/*
    private byte[,] iSbox;  // inverse Substitution box 
    private byte[,] w;      // key schedule array. 
    private byte[,] Rcon;   // Round constants.
    private byte[,] State;  // State matrix*/

	char Tmp[512];
	bool tmpA[128];
	bool tmpB[128];
};
#endif //AES_H