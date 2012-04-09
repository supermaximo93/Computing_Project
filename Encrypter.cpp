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
#include <QFile>

#include "Encrypter.h"

static const string key = "7d0897da070ef04eecdb8e7e2aed7cbe";
static QMutex mutex;

void Encrypter::encryptFile(const char *filename, const bool giveFileNotFoundError)
{
    // Lock a mutex to make sure that only one encryption can occur at a time (just in case they happen to be operating
    // on the same file)
    while (!mutex.tryLock(1000));

    // Open the file and also create a temporary file
    ifstream file;
    file.open(filename, ios::binary);
    if (file.is_open())
    {
        string encryptedFileName = string(filename) + ".temp.enc";
        ofstream encryptedFile;
        encryptedFile.open(encryptedFileName.c_str(), ios::binary);

        if (encryptedFile.is_open())
        {
            const unsigned keySize = key.size();
            unsigned count = 0;
            char data[] = { '\0', '\0' };

            // Read in each byte of the original file, XOR it with an element with a key, and write it to the temporary
            // file, until the end of the original file is reached
            while (true)
            {
                file.read(data, 1);
                if (file.eof()) break;
                data[0] ^= key[count % keySize];
                encryptedFile.write(data, 1);
                ++count;
            }

            encryptedFile.close();
            file.close();

            // Delete the original file and rename the temporary file to the original file name
            remove(filename);
            rename(encryptedFileName.c_str(), filename);
            if (QFile::exists(encryptedFileName.c_str())) remove(encryptedFileName.c_str());
        }
        else cout << "File " << filename << " could not be encrypted" << endl;
        if (file.is_open()) file.close();
    }
    else if (giveFileNotFoundError) cout << "File " << filename << " could not be opened" << endl;

    // Unlock the mutex to allow other threads to use the encryption function
    mutex.unlock();
}

void Encrypter::decryptFile(const char *filename, const bool giveFileNotFoundError)
{
    // The encryption method works both ways, so just call Encrypter::encryptFile
    encryptFile(filename, giveFileNotFoundError);
}
