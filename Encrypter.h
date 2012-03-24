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

void encryptFile(const char *filename, bool giveFileNotFoundError = true);
void decryptFile(const char *filename, bool giveFileNotFoundError = true);

}

#endif // ENCRYPTER_H
