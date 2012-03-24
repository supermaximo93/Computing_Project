/*
 * Encrypter.cpp
 *
 *  Created on: 24 March 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <fstream>
using namespace std;

#include <QMutex>

#include "Encrypter.h"

static const string key = "this is a key";
static QMutex mutex;

void Encrypter::encryptFile(const char *filename, const bool giveFileNotFoundError)
{
    while (!mutex.tryLock(1000));

    ifstream file;
    file.open(filename, ios::binary);
    if (file.is_open())
    {
        ofstream encryptedFile;
        encryptedFile.open((string(filename) + ".temp.enc").c_str(), ios::binary);

        if (encryptedFile.is_open())
        {
            const unsigned keySize = key.size();
            unsigned count = 0;
            char data[] = { '\0', '\0' };

            while (true)
            {
                file.read(data, 1);
                if (file.eof()) break;
                data[0] ^= key[count % keySize];
                encryptedFile.write(data, 1);
                ++count;
            }

            encryptedFile.close();

            remove(filename);
            rename((string(filename) + ".temp.enc").c_str(), filename);
        }
        else cout << "File " << filename << " could not be encrypted" << endl;
        file.close();
    }
    else if (giveFileNotFoundError) cout << "File " << filename << " could not be opened" << endl;

    mutex.unlock();
}

void Encrypter::decryptFile(const char *filename, const bool giveFileNotFoundError)
{
    encryptFile(filename, giveFileNotFoundError);
}
