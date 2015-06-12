/**
@(#)$Id$
* @(#)
*
* (c)  XIAOTING PRIVATE PERSON  2014
* All Rights Reserved. QQ:451292510 Email:xiaoting_chen2010@163.com
*/

/**
@brief  des算法
    DES算法为密码体制中的对称密码体制，又被称为美国数据加密标准，是1972年美国IBM公司研制的对称密码体制加密算法。
	明文按64位进行分组，密钥长64位，密钥事实上是56位参与DES运算（第8、16、24、32、40、48、56、64位是校验位， 
	使得每个密钥都有奇数个1）分组后的明文组和56位的密钥按位替代或交换的方法形成密文组的加密方法。

@Author $Author$
@Date $Date$
@Revision $Revision$
@URL $URL$
@Header $Header$
*/

#ifndef DES_H
#define DES_H

#pragma warning (disable:4996)	//取消警告

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
	void SDES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);//标准DES加/解密
	void SetKey(const char* Key, int len);// 设置密钥 当密钥长度超过8时 3DES加密
	void SetSubKey(PSubKey pSubKey, const char Key[8]);// 设置子密钥
	void F_func(bool In[32], const bool Ki[48]);// f 函数
	void S_func(bool Out[32], const bool In[48]);// S 盒代替
	void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
	void Xor(bool *InA, const bool *InB, int len);// 异或
	void RotateL(bool *In, int len, int loop);// 循环左移
	void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
	void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组

	void Xor(char *InA, const char *InB, int len);
	void RotateL(char *In, int len, int loop);

public:
protected:
private:
	bool SubKey[2][16][48];// 16圈子密钥
	bool Is3DES;// 3次DES标志
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