#ifndef CRYPTO_H
#define CRYPTO_H

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QSettings>
#include "qaesencryption.h"

namespace Crypto
{
    class Crypto
    {
    public:
        /*! \brief Create encrypted string using individual initial vector.*/
        QString EncryptBase64(QString text);

        /*! \brief Decrypt encrypted string using individual initial vector.*/
        QString DecryptBase64(QString text);

    private:
        QSettings *settings;

        /*! \brief Generate an 32 byte Initialization Vector
         *  \abstract Should be random and unique for every encryption run. */
        QString generateIv();
    };
}
#endif // CRYPTO_H
