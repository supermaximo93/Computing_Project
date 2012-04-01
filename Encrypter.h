/*
 * Encrypter.h
 *
 *  Created on: 24 March 2012
 *      Author: Max Foster
 */

#ifndef ENCRYPTER_H
#define ENCRYPTER_H

namespace Encrypter
{

// Encrypts/decrypts the specified file. If giveFileNotFoundError is true, then a message will be logged if the file
// could not be found or the file could not be encrypted/decrypted for any reason
void encryptFile(const char *filename, bool giveFileNotFoundError = true);
void decryptFile(const char *filename, bool giveFileNotFoundError = true);

}

#endif // ENCRYPTER_H
