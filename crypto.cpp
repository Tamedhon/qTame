#include "crypto.h"

namespace Crypto
{
    QString Crypto::EncryptBase64(QString text)
    {
        settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
        settings->setIniCodec("UTF-8");
        settings->beginGroup("User");
        QString iv = generateIv();
        QByteArray hashKey = QCryptographicHash::hash(settings->value("key").toString().toLocal8Bit(), QCryptographicHash::Sha256);
        settings->endGroup();
        QByteArray hashIv = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

        return (iv + QAESEncryption::Crypt(QAESEncryption::AES_256,
                                          QAESEncryption::CBC,
                                          text.toLocal8Bit(),
                                          hashKey,
                                          hashIv).toBase64());
    }

    QString Crypto::DecryptBase64(QString text)
    {
        settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
        settings->setIniCodec("UTF-8");
        settings->beginGroup("User");
        QByteArray hashKey = QCryptographicHash::hash(settings->value("key").toString().toLocal8Bit(), QCryptographicHash::Sha256);
        settings->endGroup();
        QByteArray hashIv = QCryptographicHash::hash(text.left(32).toLocal8Bit(), QCryptographicHash::Md5);

        return QAESEncryption::RemovePadding(QAESEncryption::Decrypt(QAESEncryption::AES_256,
                                             QAESEncryption::CBC,
                                             QByteArray::fromBase64(text.mid(32).toLocal8Bit()),
                                             hashKey,
                                             hashIv,
                                             QAESEncryption::ZERO), QAESEncryption::ISO);
    }

    QString Crypto::generateIv()
    {
        QString result("");
        const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int string_length = sizeof(alphanum)-1;
        srand(time(0));
        for(int i = 0; i < 32; i++)
        {
            result.append(alphanum[rand() % string_length]);
        }
        return result;
    }
}
