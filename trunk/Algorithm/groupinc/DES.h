/**
@(#)$Id$
* @(#)
*
* (c)  XIAOTING PRIVATE PERSON  2014
* All Rights Reserved. QQ:451292510 Email:xiaoting_chen2010@163.com
*/

/**
@brief  des�㷨
    DES�㷨Ϊ���������еĶԳ��������ƣ��ֱ���Ϊ�������ݼ��ܱ�׼����1972������IBM��˾���ƵĶԳ��������Ƽ����㷨��
	���İ�64λ���з��飬��Կ��64λ����Կ��ʵ����56λ����DES���㣨��8��16��24��32��40��48��56��64λ��У��λ�� 
	ʹ��ÿ����Կ����������1���������������56λ����Կ��λ����򽻻��ķ����γ�������ļ��ܷ�����

@Author $Author$
@Date $Date$
@Revision $Revision$
@URL $URL$
@Header $Header$
*/

#ifndef DES_H
#define DES_H

#pragma warning (disable:4996)	//ȡ������

class DES  
{
friend class EncryptIF;
public:
	typedef bool    (*PSubKey)[16][48];

	virtual ~DES();
	bool ECB(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
	bool CBC(char *IV, char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
	bool CFB(char *IV, char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
	bool OFB(char *IV, char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
	bool CTR(char *CNT, char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);

protected:
	DES();

private:
	void SDES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);//��׼DES��/����
	void SetKey(const char* Key, int len);// ������Կ ����Կ���ȳ���8ʱ 3DES����
	void SetSubKey(PSubKey pSubKey, const char Key[8]);// ��������Կ
	void F_func(bool In[32], const bool Ki[48]);// f ����
	void S_func(bool Out[32], const bool In[48]);// S �д���
	void Transform(bool *Out, bool *In, const char *Table, int len);// �任
	void Xor(bool *InA, const bool *InB, int len);// ���
	void RotateL(bool *In, int len, int loop);// ѭ������
	void ByteToBit(bool *Out, const char *In, int bits);// �ֽ���ת����λ��
	void BitToByte(char *Out, const bool *In, int bits);// λ��ת�����ֽ���

	void Xor(char *InA, const char *InB, int len);
	void RotateL(char *In, int len, int loop);

public:
protected:
private:
	bool SubKey[2][16][48];// 16Ȧ����Կ
	bool Is3DES;// 3��DES��־
	char Tmp[256], deskey[16];
	bool tmpA[64], tmpB[64];

	static const char IP_Table[64];
	// final permutation IP^-1 
	static const char IPR_Table[64];
	// expansion operation matrix
	static const char E_Table[48];
	// 32-bit permutation function P used on the output of the S-boxes 
	static const char P_Table[32];
	// permuted choice table (key) 
	static const char PC1_Table[56];
	// permuted choice key (table) 
	static const char PC2_Table[48];
	// number left rotations of pc1 
	static const char LOOP_Table[16];
	// The (in)famous S-boxes 
	static const char S_Box[8][4][16];

	//encrypt
	static const bool DES_ENCRYPT;
	//decrypt
	static const bool DES_DECRYPT;
};

#endif // DES_H