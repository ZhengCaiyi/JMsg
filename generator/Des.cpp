#include <windows.h>
#include "Des.h"
#include <string>

namespace _ServerEngine
{
	static const char PC1_Table[56] = {
		13, 2,10,50, 1,28,59,32,	30,46,19,63,33,41,24,52,
		27,42,49,18, 9,48,23,58,	31, 8, 7,12, 6, 5, 3,53,
		55,36,57,40,26, 4,44,17,	62,38,15,14,25,56,64,20,
		21,60,11,34,22,29,43,16
	};

	static const char PC2_Table[48] =
	{
		43, 1,19,20, 3,56,46, 7,	29,30, 5,38,31,16,14,17,
		53, 6,48, 2,18, 8, 9,49,	11,21,37,45,24,12,44,25,
		27,28,26,40,39,23,33, 4,	52,41,47,36,32,50,22,55,
	};

	static const char Rotate_Table[16]=
	{
		1,1,2,2, 2,2,2,2,
		2,2,2,2, 2,2,2,1
	};

	static const char IP_Table[64] =
	{
		13,  2, 10, 50,  1, 28, 59, 32,	 30, 46, 19, 63, 33, 41, 24, 52,
		27, 42, 49, 18,  9, 48, 23, 58,	 31,  8,  7, 12,  6,  5,  3, 53,
		55, 36, 57, 40, 26,  4, 44, 17,	 62, 38, 15, 14, 25, 56, 64, 20,
		21, 60, 11, 34, 22, 29, 43, 16,	 51, 35, 37, 45, 54, 39, 47, 61,

	};

	static const char IPR_Table[64]=
	{
		5,  2, 31, 38, 30, 29, 27, 26,	 21,  3, 51, 28,  1, 44, 43, 56,
		40, 20, 11, 48, 49, 53, 23, 15,	 45, 37, 17,  6, 54,  9, 25,  8,
		13, 52, 58, 34, 59, 42, 62, 36,	 14, 18, 55, 39, 60, 10, 63, 22,
		19,  4, 57, 16, 32, 61, 33, 46,	 35, 24,  7, 50, 64, 41, 12, 47,
	};

	static const char E_Table[48] = 
	{
		1,27, 4, 8, 2, 9,13,17,	21,22,26,30,24,32,10,23,
		3,15,28,18, 5,31,19,12,	 6,16,14,25, 7,11,29,20,
		28,16,18,14,29,15,27, 5,	17,21,30,12,19,11, 8,26
	};

	static const char P_Table[32]=
	{
		13,24,25,31,17,28, 2,29,	14, 3, 6, 9,21,12, 4,22,
		8, 5, 7,26,20,18,15,23,	 1,32,11,10,16,27,30,19

	};

	static const char S_Box[8][4][16]=
	{
		//Part1
		5,13, 7, 8, 3,10, 4, 1,	14,11,15,12, 2,16, 6, 9,
		11, 8, 5,12,15,14, 1,10,	13, 3, 2, 7, 4, 6,16, 9,
		10, 5, 6, 7,13, 8,12, 4,	16,11, 3, 1, 2, 9,15,14,
		5, 2,11,15,12, 4,10,13,	 7, 8, 3,14, 1, 9, 6,16,
		//Part2
		10, 4, 9, 3,14, 6, 5,12,	16, 2, 7, 1,13,15,11, 8,
		7, 4, 8,12,16, 6,10, 3,	13, 5, 1, 2, 9,14,11,15,
		6, 1,16,14,13,10, 5, 4,	 8,12, 7, 9,15,11, 3, 2,
		7,10,12, 5, 8, 4,16,13,	 3,11, 2,14,15, 9, 1, 6,
		//Part3
		16, 3, 4, 1,14,10,13, 5,	11,15, 9, 6, 8,12, 7, 2,
		12, 8,15, 7, 4, 5, 1,11,	14, 6, 9,16,10, 2,13, 3,
		10, 7,11,15, 6, 4, 5, 2,	 3,16, 9,14, 8,13,12, 1,
		3, 6, 5, 4,13, 8,14, 9,	 2,12,16, 1, 7,11,15,10,
		//Part4
		15,12, 3,14,10,11,16, 9,	 2,13, 5, 8, 6, 1, 4, 7,
		16,12, 3, 4, 9, 5, 7,10,	 6, 2, 1,14,15,13, 8,11,
		4,16,12, 2,13, 6,10, 7,	 3, 5,15,11, 1, 9, 8,14,
		14, 7, 5, 3, 8, 6,15,11,	10, 1,12, 9, 4,13,16, 2,



		//Part5
		5, 7,14, 4,13,15,11, 1,	 3,16, 6, 2, 8,10, 9,12,
		1, 4,15, 3, 9,10,11, 6,	 5, 7,13,12, 2,14,16, 8,
		13, 8, 4,16, 5,15,12,14,	 9, 1, 3, 7, 6,10,11, 2,
		2, 4,11, 8,16, 9,14, 1,	 5,13, 7,10,12,15, 6, 3,
		//Part6
		3, 2,14,16, 1, 8, 7, 4,	13, 5,15,10, 9,12,11, 6,
		10, 9, 4, 2, 7,15,14, 1,	 8,11, 5,12, 3, 6,16,13,
		5,10, 8, 4,11, 1, 2, 6,	 3,14,16, 9,13, 7,15,12,
		5,14, 3, 6, 4, 2,16,10,	13, 1, 8,12, 7,15, 9,11,
		//Part7
		13,12, 9, 6, 1,15,16, 7,	11, 2, 4, 8, 3,14,10, 5,
		10,15, 5,16,12, 8, 3, 1,	 2,11, 4, 9,14,13, 6, 7,
		10, 9,14, 2, 8, 5,11, 1,	 4, 7,13, 3, 6,15,16,12,
		10, 3, 5,13, 7, 2, 1,16,	15, 6, 9,11,14, 4,12, 8,

		//Part8
		7,10, 3, 6, 1,11, 9,16,	 4, 2,15, 5,13, 8,14,12,
		16,10,14, 6, 2, 8, 9,15,	 4,12,11, 5, 7, 1, 3,13,
		16, 5, 9, 6, 4, 8, 3,13,	 1,10, 7,11,15,14, 2,12,
		14, 1, 5,10, 2,13, 9,12,	 7,11,16, 3, 6, 4, 8,15

	};
	

	void* CDes::Encrypt( const void* pData,size_t nLen, const void* pKey,size_t nKeyLen )
	{
		size_t lenOld = nLen;
		m_nOpBuffSize = 0;
		if(pData == NULL || pKey == NULL || nLen==0 ||nKeyLen == 0||nLen>512)
			return NULL;

		CHAR cData[8];
		CHAR cKey[16] = {0};

		memcpy(cKey,pKey,nKeyLen>16?16:nKeyLen);

		nKeyLen = 16;
		BuildKey(cKey,nKeyLen);

		const char* pSrc = reinterpret_cast<const char*>(pData);

		for(size_t i=0;static_cast<INT>(nLen)>0;++i,nLen-=8)
		{
			if(nLen>=8)
				memcpy(cData,pSrc+i*8,8);
			else
			{
				memcpy(cData,pSrc+i*8,nLen);
				memset(cData+nLen,0,8-nLen);
			}

			Des(m_cOpBuff+i*8,cData,m_bSubKey[0],true);
		}
		m_nOpBuffSize = (lenOld+7)/8*8;
		return m_cOpBuff;
	}

	void* CDes::Decrypt(const void* pData,size_t nLen,const void* pKey,size_t nKeyLen)
	{
		m_nOpBuffSize = 0;
		if(pData == NULL || pKey == NULL || nLen==0 ||nLen>512||nLen%8||nKeyLen == 0)
			return NULL;

		CHAR cKey[16] = {0};

		memcpy(cKey,pKey,nKeyLen>16?16:nKeyLen);

		nKeyLen = 16;
		BuildKey(cKey,nKeyLen);

		const char* pSrc = reinterpret_cast<const char*>(pData);

		for(size_t i=0;i<nLen/8;++i)
		{
			Des(m_cOpBuff+i*8,pSrc+8*i,m_bSubKey[0],false);
		}
		return m_cOpBuff;
	}

	void CDes::BuildKey(const char* pKey,size_t nKeyLen)
	{
		memset(m_cDESKey,0,sizeof(m_cDESKey));
		memcpy(m_cDESKey,pKey,nKeyLen>16?16:nKeyLen);

		bool K[64];

		CharsToBits(pKey,K,sizeof(K));
		TransMatrix(K,K,PC1_Table,56);

		for(size_t i=0;i<16;++i)
		{
			RotateL(K,28,Rotate_Table[i]);
			RotateL(K+28,28,Rotate_Table[i]);

			TransMatrix(K,m_bSubKey[i],PC2_Table,48);
		}
	}

	__forceinline void CDes::CharsToBits(IN const char* pSrc,bool* pDest,size_t nSize)
	{
		for(size_t i=0;i<nSize;++i)
			pDest[i] = (pSrc[i/8]>>(i&7))&1;
	}

	__forceinline void CDes::BitsToChars(IN bool* pSrc,OUT char* pDest, size_t nSize)
	{
		memset(pDest,0,nSize/8);
		for(size_t i=0;i<nSize;++i)
			pDest[i/8]|=pSrc[i]<<(i&7);
	}

	void CDes::TransMatrix(bool* pSrc,bool* pDest,const char* pTable,size_t nSize)
	{

		char cTemp[256];
		for(size_t i=0;i<nSize;++i)
			cTemp[i] = pSrc[pTable[i]-1];

		CopyMemory(pDest,cTemp,nSize);
	}

	__forceinline void CDes::RotateL(IN OUT bool *pData,size_t nSize,int nRotateChars)
	{
#pragma warning(disable:6204)
		char cTemp[256];
		memcpy(cTemp,pData,nRotateChars);
		memcpy(pData,pData+nRotateChars,nSize-nRotateChars);
		memcpy(pData+nSize-nRotateChars,cTemp,nRotateChars);
#pragma warning(default:6204)
	}

	void CDes::Des( OUT char* pDest,IN const char* pSrc,const bool* pKey,bool bEncoder /*=false*/ )
	{
		bool M[64],bTemp[32];
		CharsToBits(pSrc,M,64);
		TransMatrix(M,M,IP_Table,64);
		if(bEncoder) //¼ÓÃÜ
		{
			for(int i=0;i<16;++i)
			{
				memcpy(bTemp,M+32,32);
				F_Func(M+32,pKey+i*48);
				F_Xor(M+32,M,32);
				memcpy(M,bTemp,32);
			}
		}
		else
		{
			for(int i=15;i>=0;--i)
			{
				memcpy(bTemp,M,32);
				F_Func(M,pKey+i*48);
				F_Xor(M,M+32,32);
				memcpy(M+32,bTemp,32);
			}
		}

		TransMatrix(M,M,IPR_Table,64);
		BitsToChars(M,pDest,64);
	}

	inline void CDes::F_Func( IN OUT bool* pData,IN const bool* pKey )
	{
		bool MR[48];
		TransMatrix(pData,MR,E_Table,48);
		F_Xor(MR,pKey,48);
		S_Func(pData,MR);
		TransMatrix(pData,pData,P_Table,32);
	}

	__forceinline void CDes::F_Xor( IN OUT bool* pDest,const bool* pSrc,size_t nLen )
	{
		for(size_t i=0;i<nLen;++i)
			pDest[i]^=pSrc[i];
	}

	void CDes::S_Func( bool* pDest,const bool* pSrc )
	{
		for(char i=0,j,k;i<8;++i,pDest+=4,pSrc+=6)
		{
			j=(pSrc[0]*2)+pSrc[5];
			k=(pSrc[1]*8)+pSrc[2]*4+pSrc[3]*2+pSrc[4];
			CharsToBits(&S_Box[i][j][k],pDest,4);
		}
	}

	std::string CDes::EncryptString( const std::string& input, const std::string& key )
	{
		std::string idlNew= input;
		int leftLen = (8 - input.size() % 8) % 8;
		for(int i = 0; i - leftLen; i++) {
			idlNew.append("\0",1);
		}

		std::string encoded;
		int lenEncoded = 0;
		for(;;) {
			size_t lenToEncode = idlNew.size() - lenEncoded;

			if(lenToEncode == 0) {
				break;
			}
			if(lenToEncode > 512) {
				lenToEncode = 512;
			}
			void* pData = Encrypt(idlNew.c_str() + lenEncoded,lenToEncode,key.c_str(),key.size());
			int bufferSize = GetDataBuffSize();
			encoded.append((char*)pData, bufferSize);
			lenEncoded += lenToEncode;
			if(lenToEncode < 512) {
				break;
			}
		}
		return encoded;
	}
	
	std::string CDes::DecryptString( const std::string& input, const std::string& key )
	{
		std::string idlNew = input;

		const int kLenBlock = 512;
		std::string decoded;
		int lenDecoded = 0;
		for(;;) {
			size_t lenToDecode = idlNew.size() - lenDecoded;

			if(lenToDecode == 0) {
				break;
			}
			if(lenToDecode > kLenBlock) {
				lenToDecode = kLenBlock;
			}
			void* pData = Decrypt(idlNew.c_str() + lenDecoded,lenToDecode,key.c_str(),key.size());
			decoded.append((char*)pData, lenToDecode);
			lenDecoded += lenToDecode;
			if(lenToDecode < 512) {
				break;
			}
		}
		return decoded;
	}

}
