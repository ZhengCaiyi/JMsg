/**
	@file		Des.h
	@date		2012年8月29日 9:27
	@brief		DES加密/解密
	@by 		来自网络,修改by首席村支书
	@Email 		wangbing323@163.com
	@QQ			32996758
*********************************************************************/
#pragma once
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#include <string>
namespace _ServerEngine
{
	/**
	* @brief Des加密/解密 
	*/
	class CDes
	{
		bool m_bSubKey[16][48];
		char m_cDESKey[16];

		char m_cOpBuff[512];//操作缓冲

		size_t m_nOpBuffSize;//缓冲使用大小

	public:
		CDes(){}
		/**
		* @brief 加密数据
		* @param [in] pData 需要加密的数据内容
		* @param [in] nLen 加密的数据长度(pData的长度），单位：字节
		* @param [in] pKey 加密密钥
		* @param [in] nKeyLen 加密密钥长度（pKey的长度)，单位：字节
		* @return 如果加密成功，则返回加密后的内容，否则返回NULL
		* @note 加密长度不能超过512个字节
		*/
		void* Encrypt(const void* pData,size_t nLen, const void* pKey,size_t nKeyLen);
		
		std::string EncryptString(const std::string& input, const std::string& key);
		//解密后长度不能超过512个字符
		/**
		* @brief 解密数据
		* @param [in] pData 待解密的数据
		* @param [in] nLen 待解密的数据长度（pData的长度)，必须是8的整数倍，单位：字节 
		* @param [in] pKey 解密密钥
		* @param [in] nKeyLen 解密密钥长度(pKey的长度),单位：字节
		* @return 如果解密成功，则返回解密后的内容，否则返回NULL
		* @note 解密长度不能超过512字节
		*/
		void* Decrypt(const void* pData,size_t nLen,const void* pKey,size_t nKeyLen);
		

		std::string DecryptString( const std::string& input, const std::string& key );

		/**
		* @brief 加密或者解密后数据的长度
		*/
		size_t GetDataBuffSize(void)const
		{
			return m_nOpBuffSize;
		}
	private:

		void BuildKey(IN const char* pKey,IN size_t nKeyLen);

		static void CharsToBits(IN const char* pSrc,OUT bool* pDest,size_t nSize);

		static void BitsToChars(IN bool* pSrc,OUT char* pDest,size_t nSize);

		//矩阵映射
		static void TransMatrix(bool* pSrc,bool* pDest,const char* pTable,size_t nSize);

		static void RotateL(IN OUT bool *pData,size_t nSize,int nRotateChars);

		static void Des(OUT char* pDest,IN const char* pSrc,const bool* pKey,bool bEncoder =false);

		static void F_Func(IN OUT bool* pData,IN const bool* pKey);

		static void F_Xor(IN OUT bool* pDest,const bool* pSrc,size_t nLen);

		static void S_Func(bool* pDest,const bool* pSrc);
		
	};
}
