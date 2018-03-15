/**
	@file		Des.h
	@date		2012��8��29�� 9:27
	@brief		DES����/����
	@by 		��������,�޸�by��ϯ��֧��
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
	* @brief Des����/���� 
	*/
	class CDes
	{
		bool m_bSubKey[16][48];
		char m_cDESKey[16];

		char m_cOpBuff[512];//��������

		size_t m_nOpBuffSize;//����ʹ�ô�С

	public:
		CDes(){}
		/**
		* @brief ��������
		* @param [in] pData ��Ҫ���ܵ���������
		* @param [in] nLen ���ܵ����ݳ���(pData�ĳ��ȣ�����λ���ֽ�
		* @param [in] pKey ������Կ
		* @param [in] nKeyLen ������Կ���ȣ�pKey�ĳ���)����λ���ֽ�
		* @return ������ܳɹ����򷵻ؼ��ܺ�����ݣ����򷵻�NULL
		* @note ���ܳ��Ȳ��ܳ���512���ֽ�
		*/
		void* Encrypt(const void* pData,size_t nLen, const void* pKey,size_t nKeyLen);
		
		std::string EncryptString(const std::string& input, const std::string& key);
		//���ܺ󳤶Ȳ��ܳ���512���ַ�
		/**
		* @brief ��������
		* @param [in] pData �����ܵ�����
		* @param [in] nLen �����ܵ����ݳ��ȣ�pData�ĳ���)��������8������������λ���ֽ� 
		* @param [in] pKey ������Կ
		* @param [in] nKeyLen ������Կ����(pKey�ĳ���),��λ���ֽ�
		* @return ������ܳɹ����򷵻ؽ��ܺ�����ݣ����򷵻�NULL
		* @note ���ܳ��Ȳ��ܳ���512�ֽ�
		*/
		void* Decrypt(const void* pData,size_t nLen,const void* pKey,size_t nKeyLen);
		

		std::string DecryptString( const std::string& input, const std::string& key );

		/**
		* @brief ���ܻ��߽��ܺ����ݵĳ���
		*/
		size_t GetDataBuffSize(void)const
		{
			return m_nOpBuffSize;
		}
	private:

		void BuildKey(IN const char* pKey,IN size_t nKeyLen);

		static void CharsToBits(IN const char* pSrc,OUT bool* pDest,size_t nSize);

		static void BitsToChars(IN bool* pSrc,OUT char* pDest,size_t nSize);

		//����ӳ��
		static void TransMatrix(bool* pSrc,bool* pDest,const char* pTable,size_t nSize);

		static void RotateL(IN OUT bool *pData,size_t nSize,int nRotateChars);

		static void Des(OUT char* pDest,IN const char* pSrc,const bool* pKey,bool bEncoder =false);

		static void F_Func(IN OUT bool* pData,IN const bool* pKey);

		static void F_Xor(IN OUT bool* pDest,const bool* pSrc,size_t nLen);

		static void S_Func(bool* pDest,const bool* pSrc);
		
	};
}
