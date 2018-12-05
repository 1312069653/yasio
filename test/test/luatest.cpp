#include "lmasio.h"
#include "crypto-support/crypto_wrapper.h"
#include <thread>
#pragma comment(lib, "lua51.lib")

#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#if 0
RSA* getPrivateKey(char* in_szKeyPath)
{
    FILE    *fp = NULL;
    char    szKeyPath[1024];
    RSA     *priRsa = NULL, *pubRsa = NULL, *pOut = NULL;

    memset(szKeyPath, 0, sizeof(szKeyPath));

    if (256 < strlen(in_szKeyPath))
        strncpy(szKeyPath, in_szKeyPath, 256);
    else
        strncpy(szKeyPath, in_szKeyPath, strlen(in_szKeyPath));

    printf("��Կ�ļ�·��[%s]", szKeyPath);

    /*  ����Կ�ļ� */
    if (NULL == (fp = fopen(szKeyPath, "rb")))
    {
        printf("����Կ�ļ�[%s]����", szKeyPath);
        return NULL;
    }
    /*  ��ȡ˽��Կ */
    if (NULL == (priRsa = PEM_read_RSAPrivateKey(fp, &priRsa, NULL, NULL)))
    {
        printf("����˽Կ���ݳ���\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    printf("��ȡ˽Կ\n");
    pOut = priRsa;
    return pOut;
}

RSA* getPublicKey(char* in_szKeyPath)
{
    FILE    *fp = NULL;
    char    szKeyPath[1024];
    RSA     *priRsa = NULL, *pubRsa = NULL, *pOut = NULL;

    memset(szKeyPath, 0, sizeof(szKeyPath));

    if (256 < strlen(in_szKeyPath))
        strncpy(szKeyPath, in_szKeyPath, 256);
    else
        strncpy(szKeyPath, in_szKeyPath, strlen(in_szKeyPath));

    printf("��Կ�ļ�·��[%s]", szKeyPath);

    /*  ����Կ�ļ� */
    if (NULL == (fp = fopen(szKeyPath, "rb")))
    {
        printf("����Կ�ļ�[%s]����", szKeyPath);
        return NULL;
    }
    /*  ��ȡ����Կ */
    if (NULL == (priRsa = PEM_read_RSA_PUBKEY(fp, &priRsa, NULL, NULL)))
    {
        printf("����˽Կ���ݳ���\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    printf("��ȡ��Կ\n");
    pOut = priRsa;
    return pOut;
}

int testvVerify(void)
{
    unsigned int flen, rsa_len, ienLen, iRet;
    RSA     *prsa = NULL;
    char    szEnData[] = "orderId=01010500201502000004reqTime=20150205012727ext=20151120ext2=1";
    char    szTmp[10240], szTmp1[10240];

    if (NULL == (prsa = getPrivateKey("masio-rsa-private.pem")))
    {
        RSA_free(prsa);
        printf("��ȡ˽Կʧ��\n");
        return -1;
    }
    
    //  RSA_print_fp(stdout, prsa, 11);
    flen = strlen(szEnData);
    printf("��ǩ������:[%s]\n", szEnData);

    memset(szTmp, 0, sizeof(szTmp));
    memset(szTmp1, 0, sizeof(szTmp1));
    //  �Դ�ǩ��������SHA1ժҪ
    SHA256((const unsigned char*)szEnData, flen, (unsigned char*)szTmp);
    //ʹ��˽Կ��SHA1ժҪ��ǩ��
    ienLen = RSA_sign(NID_sha256, (unsigned char *)szTmp, 20, (unsigned char*)szTmp1, &iRet, prsa);
    if (ienLen != 1)
    {
        printf("ǩ��ʧ��\n");
        RSA_free(prsa);
        return -1;
    }
    RSA_free(prsa);
    printf("ǩ���ɹ�\n");
    //ǩ����szTmp1������������Ҫת��base64����
    //mac=base64encode(szTmp1)����α��,����MACֵ�����Է�ȥУ��



    //��֤ǩ��
    //��֤ǩ��������Ҫ��ȡMACֵ������ǩ������,�ԡ�����ǩ�����ݡ���SHA1�����ժҪ���ڶ�MAC��basedecode(mac)��Ȼ����ú�����֤ǩ��
    if (NULL == (prsa = getPublicKey("masio-rsa-public.pem")))
    {
        RSA_free(prsa);
        printf("��ȡ˽Կʧ��\n");
        return -1;
    }
    flen = strlen(szEnData);
    printf("��ǩ������:[%s]\n", szEnData);//ǩ������ �� mac �������ͨ�ű����л�ã�������ʾֱ����ʹ��ͬһ����

    memset(szTmp, 0, sizeof(szTmp));
    // memset(szTmp1, 0, sizeof(szTmp1));
    //  �Դ�ǩ��������SHA1ժҪ
    SHA256((const unsigned char*)szEnData, flen, (unsigned char*)szTmp);

    ienLen = RSA_verify(NID_sha256, (unsigned char *)szTmp, 20, (unsigned char*)szTmp1, iRet, prsa);
    if (ienLen != 1)
    {
        printf("ǩ�����Ϸ�\n");
        RSA_free(prsa);
        return -1;
    }
    else
        printf("��ǩ�ɹ�\n");
    RSA_free(prsa);
    return 0;
}
#endif

void lua_open_crypto(lua_State* L)
{
    sol::state_view sol2(L);
    auto crypto = sol2.create_named_table("crypto");
    auto rsa = crypto.create_named("rsa");
    rsa.set_function("pri_encrypt", &crypto::rsa::pri::encrypt);
    rsa.set_function("pri_decrypt", &crypto::rsa::pri::decrypt);
    rsa.set_function("pub_encrypt", &crypto::rsa::pub::encrypt);
    rsa.set_function("pub_decrypt", &crypto::rsa::pub::decrypt);
    rsa.set_function("pri_encrypt2", &crypto::rsa::pri::encrypt2);
    rsa.set_function("pri_decrypt2", &crypto::rsa::pri::decrypt2);
    rsa.set_function("pub_encrypt2", &crypto::rsa::pub::encrypt2);
    rsa.set_function("pub_decrypt2", &crypto::rsa::pub::decrypt2);
}

int main(int argc, char** argv)
{
    sol::state s;
    s.open_libraries();
    lua_open_masio(s.lua_state());
    lua_open_crypto(s.lua_state());

    // testvVerify();

    s.script_file("example.lua");

    do {
        std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(50)));
    } while (!s["global_update"].call(50.0 / 1000));

    return 0;
}
