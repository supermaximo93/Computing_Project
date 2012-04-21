/*
 * Database.h
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <tr1/memory>
#include <tr1/shared_ptr.h>

#include <QMutex>
#include <QDir>
#include <QDate>

#include "Encrypter.h"
#include "Record.h"
#include "Setting.h"
#include "dialogs/setting/SettingForm.h"
#include "Utils.h"

template<class recordType>
class Database
{
public:
    typedef std::vector<recordType> recordList;
    typedef std::tr1::shared_ptr<recordList> recordListPtr;

#ifdef COMPILE_TESTS
    Database(bool testing = false);
#else
    Database();
#endif
    ~Database();

    /*
     * Adds a new record to the database, and sets the record's ID. Throws an exception if the record does not pass its
     * validation (which should be done before addRecord is called anyway), or if the database file could not be opened
     */
    void addRecord(recordType &record);

    /*
     * Updates an existing record that has been found from the database or has just been added to it (i.e. the ID of the
     * record is greater than -1). If the record was updated successfully, then true is returned. An exception is thrown
     * if the record is a null record (i.e. ID < 0), or if the database file could not be opened
     */
    bool updateRecord(const recordType &record);

    void updateRecordAtPosition(const recordType &record);

    /*
     * Attempts to delete the record with the specified ID (if any), returning true on success. Throws an exception if
     * the database file could not be opened
     */
    bool deleteRecord(const int id);

    /*
     * Attempts to find the first record whose field specified matches the search term. Returns a null record
     * (i.e. ID < 0) if a record could not be found. Throws an exception if the database file could not be opened
     */
    template<typename type>
    recordType findRecord(const std::string &fieldName, const type &searchTerm);

    /*
     * Finds all the records whose field specified matches the search term, and returns them in a recordList (which may
     * be empty if no matching records where found). Throws an exception if the database file could not be opened
     */
    template<typename type>
    recordListPtr findRecords(const std::string &fieldName, const type &searchTerm);

    /*
     * Finds all the records whose field specified matches the search term from the recordList given, and returns them
     * in a recordList (which may be empty if no matching records where found)
     */
    template<typename type>
    recordListPtr findRecords(const recordList &recordsToSearch, const std::string &fieldName,
                              const type &searchTerm);

    /*
     * Removes all the records from the recordList given whose field specified does not match the search term given
     */
    template<typename type>
    void keepRecords(recordList &records, const std::string &fieldName, const type &searchTerm);

    /*
     * Removes all the records from the recordList given that when passed to the given function, make the function
     * return false
     */
    void keepRecords(recordList &records, bool (*keepRecordFunction)(const recordType &, void *), void *data);

    /*
     * Removes all the records from the recordList given whose field specified matches the search term given
     */
    template<typename type>
    void removeRecords(recordList &records, const std::string &fieldName, const type &searchTerm);

    /*
     * Removes all the records from the recordList given that when passed to the given function, make the function
     * return true
     */
    void removeRecords(recordList &records, bool (*keepRecordFunction)(const recordType &, void *), void *data);

    /*
     * Returns all of the records in the database. Throws an exception if the database file could not be opened
     */
    recordListPtr allRecords();

    /*
     * Returns the record at a particular position of the database file, given by an index. Throws an exception if the
     * database file could not be opened
     */
    recordType recordAt(const int index);

    /*
     * Returns the number of records in the database. Throws an exception if the database file could not be opened
     */
    unsigned recordCount();

    /*
     * Sorts the given records according to the given comparison function
     */
    void sortRecords(recordList &records, unsigned startIndex, unsigned endIndex,
                     int (*comparisonFunction)(const recordType &, const recordType &));

    const std::string & filename(), & databaseDirectory(), & backupDirectory(), & pdfDirectory();

    /*
     * Reloads the database file path from the settings database, moving the database files if the directory has
     * changed and the moveFiles parameter is true
     */
#ifdef COMPILE_TESTS
    void reloadFilename(bool moveFiles, bool testing);
#else
    void reloadFilename(bool moveFiles);
#endif

    /*
     * Backs up the database file to the location specified in the settings
     */
    void backupFile();

    void reloadIdCounter();
    int getIdCounter();
    void setIdCounter(int newIdCounter);

private:
#ifdef COMPILE_TESTS
    const bool testing;
#endif

    std::string filename_, databaseDirectory_, backupDirectory_, pdfDirectory_;
    int idCounter;    // Used to give a unique ID to records. Always incremented, never decremented
    QMutex fileMutex; // A mutex that is locked when the database is doing file operations so that other threads cannot
                      // use the file at the same time (which would cause lots of problems!)
};

template<class recordType>
#ifdef COMPILE_TESTS
Database<recordType>::Database(const bool testing) : testing(testing)
#else
Database<recordType>::Database()
#endif
{
    filename_ = databaseDirectory_ = backupDirectory_ = pdfDirectory_ = "";

#ifdef COMPILE_TESTS
    reloadFilename(false, testing);
#else
    reloadFilename(false);
#endif

    // Lock the fileMutex to make sure that the database file is not accessed by other threads
    while (!fileMutex.tryLock(1000));

    // Decrypt the file indicating that we don't want any error dialogs to pop up
    Encrypter::decryptFile(filename_.c_str(), false);

    std::ifstream file;
    file.open(filename_.c_str(), std::ios::binary);
    if (file.is_open())
    {
        // If the file exists, go to the beginning of the file and read the value of the ID counter into the database
        file.seekg(0, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&idCounter), sizeof(idCounter));
        file.close();
    }
    else
    {
        // The file doesn't exist, so set the ID counter to 0 and write it to a new database file
        idCounter = 0;
        std::ofstream file;
        file.open(filename_.c_str(), std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));
            file.close();
        }
        else std::cout << "Could not create " + filename_ << std::endl;
    }

    // We're done with the file so encrypt it and unlock the file mutex so that other threads can access the file
    Encrypter::encryptFile(filename_.c_str());
    fileMutex.unlock();
}

template<class recordType>
Database<recordType>::~Database()
{
    // Lock the file mutex and decrypt the database file
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    // Create a new file to copy all of the valid database records to. A database is invalid if it has an ID that is
    // less than 0 (i.e. it has been deleted)
    std::fstream newFile;
    newFile.open((filename_ + ".temp").c_str(), std::ios::out | std::ios::binary);
    if (newFile.is_open())
    {
        // Write the value of the ID counter first
        newFile.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));

        bool fileCopied = false;
        recordType tempRecord;
        std::fstream file;
        file.open(filename_.c_str(), std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            // Jump past the position of the ID counter to the start of the records
            file.seekg(sizeof(idCounter), std::ios_base::beg);

            // Copy all of the records to the new file that are valid records
            while (true)
            {
                tempRecord.readFromFile(file);
                if (file.eof()) break;
                if (!tempRecord.null()) tempRecord.writeToFile(newFile);
            }
            fileCopied = true;
            file.close();
        }
        else std::cout << "Could not open file " + filename_ << std::endl;
        newFile.close();

        // If the copy was successful, delete the old file and rename the new one to the original filename
        if (fileCopied)
        {
            remove(filename_.c_str());
            rename((filename_ + ".temp").c_str(), filename_.c_str());
            if (QFile::exists((filename_ + ".temp").c_str())) remove((filename_ + ".temp").c_str());
        }
        else remove((filename_ + ".temp").c_str());
    }
    else std::cout << "Could not create temporary file " + filename_ + ".temp" << std::endl;

    // We're done with the file so encrypt it and unlock the file mutex
    Encrypter::encryptFile(filename_.c_str());
    fileMutex.unlock();

    backupFile();
}

template<class recordType>
void Database<recordType>::addRecord(recordType &record)
{
    // Make sure the record is valid before continuing. If it isn't then an exception will be thrown. Bear in mind that
    // the record's ID doesn't need to be set
    record.validate();

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        // Jump to the end of the database file, set the record's ID to the value of the ID counter, increment the ID
        // counter and then write the record to a file
        file.seekp(0, std::ios_base::end);
        record.id = idCounter++;
        record.writeToFile(file);

        // Jump to the beginning of the file to update the ID counter. This is necessary because if the program is
        // halted early for any reason, then the ID counter needs to remain in tact otherwise the database runs the
        // risk of having records with the same IDs
        file.seekp(0, std::ios_base::beg);
        file.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));

        file.seekg(0, std::ios_base::end);
        record.filePosition = ((static_cast<size_t>(file.tellg()) - sizeof(idCounter)) / recordType::size()) - 1;

        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }
}

template<class recordType>
bool Database<recordType>::updateRecord(const recordType &record)
{
    // Make sure the record is valid. The record ID must be set
    if (record.null()) throw(std::runtime_error("Record ID must be initialised"));
    record.validate();

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    recordType tempRecord;
    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(sizeof(idCounter), std::ios_base::beg);

        // Read in records until we get to a record which has a matching ID or we get to the end of the file
        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord == record)
            {
                // Jump to the position just before the matching record and write the new record data to the file
                file.seekp(file.tellg() - static_cast<std::fstream::pos_type>(recordType::size()), std::ios::beg);
                record.writeToFile(file);
                file.close();
                Encrypter::encryptFile(filename_.c_str());
                fileMutex.unlock();
                return true;
            }
        }

        // The record could not be found so return false
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        return false;
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }
}

template<class recordType>
void Database<recordType>::updateRecordAtPosition(const recordType &record)
{
    if (record.filePosition < 0) throw std::runtime_error("Record file position must be positive");

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekp(sizeof(idCounter) + (recordType::size() * record.filePosition), std::ios_base::beg);
        record.writeToFile(file);
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }
}

template<class recordType>
bool Database<recordType>::deleteRecord(const int id)
{
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    recordType tempRecord;
    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(sizeof(idCounter), std::ios_base::beg);

        // Read in records until we get to a record which has a matching ID or we get to the end of the file
        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord.getId() == id)
            {
                // Jump to the position just before the matching record and write a blank/null record to the file
                file.seekp(file.tellg() - static_cast<std::fstream::pos_type>(recordType::size()), std::ios::beg);
                recordType().writeToFile(file);
                file.close();
                Encrypter::encryptFile(filename_.c_str());
                fileMutex.unlock();
                return true;
            }
        }

        // Record could not be found
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }

    std::cout << "Record could not be found. No records were deleted" << std::endl;
    return false;
}

template<class recordType> template<typename type>
recordType Database<recordType>::findRecord(const std::string &fieldName, const type &searchTerm)
{
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        // Record::hasMatchingField takes field names as lowercase, so convert the fieldName parameter to lowercase
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;
        tempRecord.filePosition = -1;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            // Read in records from the file until a valid record with the matching field is found
            tempRecord.readFromFile(file);
            ++tempRecord.filePosition;
            if (file.eof()) break;
            if (tempRecord.null()) continue;
            if (tempRecord.hasMatchingField(lowercaseFieldName, searchTerm))
            {
                file.close();
                Encrypter::encryptFile(filename_.c_str());
                fileMutex.unlock();
                return tempRecord;
            }
        }

        // Matching record not found
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }

    // No matching records found; return a blank record
    return recordType();
}

template<class recordType> template <typename type>
std::tr1::shared_ptr< std::vector<recordType> >
Database<recordType>::findRecords(const std::string &fieldName, const type &searchTerm)
{
    // Similar to Database::findRecord but matching records are added to a list instead of returning the first match
    recordListPtr returnList(new recordList);

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;
        tempRecord.filePosition = -1;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            // Read in records from the file, adding any valid records that have a maching field to the list
            tempRecord.readFromFile(file);
            ++tempRecord.filePosition;
            if (file.eof()) break;
            if (tempRecord.null()) continue;
            if (tempRecord.hasMatchingField(lowercaseFieldName, searchTerm)) returnList->push_back(tempRecord);
        }
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }

    return returnList;
}

template<class recordType> template<typename type>
std::tr1::shared_ptr< std::vector<recordType> >
Database<recordType>::findRecords(const std::vector<recordType> &recordsToSearch, const std::string &fieldName,
                                  const type &searchTerm)
{
    // Similar to Database::findRecords, except that an existing list is searched instead of the database file
    std::string lowercaseFieldName = lowerCase(fieldName);
    recordListPtr returnList(new recordList);

    // Loop through the records in the list, adding any records that have a matching field to the new list
    for (unsigned i = 0; i < recordsToSearch.size(); ++i)
    {
        if (recordsToSearch[i].hasMatchingField(lowercaseFieldName, searchTerm))
            returnList->push_back(recordsToSearch[i]);
    }

    return returnList;
}

template<class recordType> template<typename type>
void Database<recordType>::keepRecords(std::vector<recordType> &records, const std::string &fieldName,
                                       const type &searchTerm)
{
    std::string lowercaseFieldName = lowerCase(fieldName);

    // Loop through the records in the list, erasing any records that do not have a matching field
    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (!records[i].hasMatchingField(lowercaseFieldName, searchTerm))
        {
            records.erase(records.begin() + i);
            --i; // decrement the counter because the record list size has been decremented
        }
    }
}

template<typename recordType>
void Database<recordType>::keepRecords(recordList &records, bool (*keepRecordFunction)(const recordType &, void *),
                                       void *data)
{
    // Erase any records from the list that, when passed to the given function, make said function return false
    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (!keepRecordFunction(records[i], data))
        {
            records.erase(records.begin() + i);
            --i;
        }
    }
}

template <class recordType> template<typename type>
void Database<recordType>::removeRecords(std::vector<recordType> &records, const std::string &fieldName,
                                         const type &searchTerm)
{
    std::string lowercaseFieldName = lowerCase(fieldName);

    // Loop through the records in the list, erasing any records that have a matching field
    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (records[i].hasMatchingField(lowercaseFieldName, searchTerm))
        {
            records.erase(records.begin() + i);
            --i;
        }
    }
}

template<typename recordType>
void Database<recordType>::removeRecords(recordList &records, bool (*removeRecordFunction)(const recordType &, void *),
                                         void *data)
{
    // Erase any records from the list that, when passed to the given function, make said function return true
    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (removeRecordFunction(records[i], data))
        {
            records.erase(records.begin() + i);
            --i;
        }
    }
}

template<class recordType>
std::tr1::shared_ptr< std::vector<recordType> > Database<recordType>::allRecords()
{
    recordListPtr returnList(new recordList);

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        // Read in records until the end of the file is reached, adding any valid records to the list
        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (!tempRecord.null()) returnList->push_back(tempRecord);
        }
        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
    }
    else
    {
        fileMutex.unlock();
        Encrypter::encryptFile(filename_.c_str());
        throw(std::runtime_error("Could not open file " + filename_));
    }

    return returnList;
}

template<class recordType>
recordType Database<recordType>::recordAt(const int index)
{
    if (index < 0) return recordType(); // If index is too low return a blank record

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        recordType tempRecord;

        file.seekg(0, std::ios_base::end);
        unsigned size = file.tellg();

        // If the index is too large (i.e. past the file size) then close the file and return a blank record
        if (sizeof(idCounter) + (recordType::size() * (index + 1)) > size) // index + 1 because index starts from 0
        {
            file.close();
            Encrypter::encryptFile(filename_.c_str());
            fileMutex.unlock();
            return recordType();
        }

        // Jump to the position in the file corresponding to the index and read the data from the file at that position
        file.seekg(sizeof(idCounter) + (recordType::size() * index), std::ios_base::beg);
        tempRecord.readFromFile(file);

        file.close();
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        return tempRecord;
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }

    // Record could not be found for whatever reason; return a blank record
    return recordType();
}

template<class recordType>
unsigned Database<recordType>::recordCount()
{
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(sizeof(idCounter), std::ios_base::beg);

        recordType tempRecord;
        unsigned count = 0;

        // Read in all of the records in the file, counting those that are valid
        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (!tempRecord.null()) ++count;
        }
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        file.close();

        return count;
    }
    else
    {
        Encrypter::encryptFile(filename_.c_str());
        fileMutex.unlock();
        throw(std::runtime_error("Could not open file " + filename_));
    }

    // File could not be accessed for whatever reason; return 0 (i.e. empty file)
    return 0;
}

template<class recordType>
void Database<recordType>::sortRecords(recordList &records, const unsigned startIndex, const unsigned endIndex,
                                       int (*comparisonFunction)(const recordType &, const recordType &))
{
    // If the number of records between the array bounds is less than 2, no sorting needs to be done
    if (endIndex - (startIndex - 1) < 2) return;

    unsigned pivotPointer = startIndex, moveablePointer = endIndex;
    recordType temp;

    // While the pointers are not pointing to the same record
    while (pivotPointer != moveablePointer)
    {
        // If the record at the pivot does not come before the record at the moveable pointer
        if (comparisonFunction(records[pivotPointer], records[moveablePointer]) > -1)
        {
            // Swap the records and pointers if they are out of order
            if (pivotPointer < moveablePointer)
            {
                temp = records[pivotPointer];
                records[pivotPointer] = records[moveablePointer];
                records[moveablePointer] = temp;

                int tempPointer = pivotPointer;
                pivotPointer = moveablePointer;
                moveablePointer = tempPointer;
            }
            else ++moveablePointer; // Otherwise increment the moveable pointer
        }
        else // the record at the pivot comes after or is equal to the record at the moveable pointer, so...
        {
            // Swap the records and pointers if they are out of order
            if (pivotPointer > moveablePointer)
            {
                temp = records[pivotPointer];
                records[pivotPointer] = records[moveablePointer];
                records[moveablePointer] = temp;

                int tempPointer = pivotPointer;
                pivotPointer = moveablePointer;
                moveablePointer = tempPointer;
            }
            else --moveablePointer; // Otherwise decrement the moveable pointer
        }
    }

    // If the pivot pointer is not at the beginning of the list, sort the records before the pivot pointer
    if (pivotPointer > 0) sortRecords(records, startIndex, pivotPointer - 1, comparisonFunction);

    // If the pivot pointer is not at the end of the list, sort the records after the pivot pointer
    if (pivotPointer < records.size() - 1) sortRecords(records, pivotPointer + 1, endIndex, comparisonFunction);
}

template<class recordType>
const std::string & Database<recordType>::filename()
{
    return filename_;
}

template<class recordType>
const std::string & Database<recordType>::databaseDirectory()
{
    return databaseDirectory_;
}

template<class recordType>
const std::string & Database<recordType>::backupDirectory()
{
    return backupDirectory_;
}

template<class recordType>
const std::string & Database<recordType>::pdfDirectory()
{
    return pdfDirectory_;
}

template<class recordType>
#ifdef COMPILE_TESTS
void Database<recordType>::reloadFilename(const bool moveFiles, bool testing)
#else
void Database<recordType>::reloadFilename(const bool moveFiles)
#endif
{

#ifdef _WIN32
    const char slashChar = '\\';
#else
    const char slashChar = '/';
#endif

    // If the directory strings are not set, set them to the current directory
    if (databaseDirectory_.empty()) databaseDirectory_ = QDir::currentPath().toStdString();
    if (backupDirectory_.empty()) backupDirectory_ = QDir::currentPath().toStdString();
    if (pdfDirectory_.empty()) pdfDirectory_ = QDir::currentPath().toStdString();

    // Put slashes on the end of the directory names if they don't have them already
    if (!databaseDirectory_.empty() && (databaseDirectory_[databaseDirectory_.length() - 1] != slashChar))
        databaseDirectory_ += slashChar;
    if (!backupDirectory_.empty() && (backupDirectory_[backupDirectory_.length() - 1] != slashChar))
        backupDirectory_ += slashChar;
    if (!pdfDirectory_.empty() && (pdfDirectory_[pdfDirectory_.length() - 1] != slashChar))
        pdfDirectory_ += slashChar;

    // Keep a copy of the previous directory names, in case a directory has changed and files need to be moved
    std::string previousDatabaseDirectory = databaseDirectory_,
            previousBackupDirectory = backupDirectory_,
            previousPdfDirectory = pdfDirectory_,
            previousFilename = filename_;

    // Get the database directory, backup directory, and PDF directory from the settings
    Setting databaseDirectorySetting, backupDirectorySetting;

    try { databaseDirectorySetting = SettingForm::getDatabaseDirectory(); }
    catch (const std::exception &e) { databaseDirectorySetting = Setting(); }

    try { backupDirectorySetting = SettingForm::getBackupDirectory(); }
    catch (const std::exception &e) { backupDirectorySetting = Setting(); }

    try { pdfDirectory_ = SettingForm::getPdfDirectoryWithoutSlash().toStdString(); }
    catch (const std::exception &e) { pdfDirectory_ = Setting().getValue(); }

    databaseDirectory_ = databaseDirectorySetting.getValue();
    backupDirectory_ = backupDirectorySetting.getValue();

    // Put slashes on the end if necessary
    if (!databaseDirectory_.empty() && (databaseDirectory_[databaseDirectory_.length() - 1] != slashChar))
        databaseDirectory_ += slashChar;
    if (!backupDirectory_.empty() && (backupDirectory_[backupDirectory_.length() - 1] != slashChar))
        backupDirectory_ += slashChar;

    // Construct a full database file path
    filename_ = databaseDirectory_ + recordType::databaseFilename;

#ifdef COMPILE_TESTS
    if (testing) filename_ += ".test";
#endif

    if (!moveFiles) return;

    // If the database directory has changed, move the database file to its new location
    if (databaseDirectory_ != previousDatabaseDirectory)
    {
        while (!fileMutex.tryLock(1000));

        try { moveFile(previousFilename.c_str(), filename_.c_str()); }
        catch (const std::exception &e)
        {
            fileMutex.unlock();
            throw std::runtime_error(e.what());
        }

        fileMutex.unlock();

        reloadIdCounter();
    }

    // If the backup directory has changed, move the backups folder to its new location
    if (backupDirectory_ != previousBackupDirectory)
    {
        while (!fileMutex.tryLock(1000));

        try { moveDirectory((previousBackupDirectory + "backups").c_str(), (backupDirectory_ + "backups").c_str()); }
        catch (const std::exception &e)
        {
            fileMutex.unlock();
            throw std::runtime_error(e.what());
        }

        fileMutex.unlock();
    }

    // If the PDF directory has changed, move the PDFs folder to its new location
    if (pdfDirectory_ != previousPdfDirectory)
    {
        while (!fileMutex.tryLock(1000));

        try { moveDirectory(previousPdfDirectory.c_str(), pdfDirectory_.c_str()); }
        catch (const std::exception &e)
        {
            fileMutex.unlock();
            throw std::runtime_error(e.what());
        }

        fileMutex.unlock();
    }
}

template<class recordType>
void Database<recordType>::backupFile()
{
#ifdef COMPILE_TESTS
    if (testing) return;
#endif

#ifdef _WIN32
    const char slashChar = '\\';
#else
    const char slashChar = '/';
#endif

    // Construct a backup directory name in the form '<backup directory>backups/backup_<year>_<week number>'
    // This structure makes sure that backups are kept in weekly intervals
    std::string backupFolderName =
            backupDirectory_ + "backups" + slashChar + "backup_" + toString(Date(time(NULL)).year) + "_"
            + toString(QDate::currentDate().weekNumber());

    // If the backup directory doesn't exist, create it
    QDir backupDirectory(backupFolderName.c_str());
    if (!backupDirectory.exists()) backupDirectory.mkpath(backupFolderName.c_str());

    // Create a copy of the database file and put it in the backup directory
    try { copyFile(filename_.c_str(), (backupFolderName + slashChar + recordType::databaseFilename).c_str()); }
    catch (const std::exception &e) { std::cout << e.what() << std::endl; }
}

template<class recordType>
void Database<recordType>::reloadIdCounter()
{
    while (!fileMutex.tryLock(1000));

    std::ifstream file;
    file.open(filename_.c_str(), std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&idCounter), sizeof(idCounter));
        file.close();
    }
    else std::cout << "Could not open file " << filename_ << std::endl;

    fileMutex.unlock();
}

template<class recordType>
int Database<recordType>::getIdCounter()
{
    return idCounter;
}

template<class recordType>
void Database<recordType>::setIdCounter(const int newIdCounter)
{
    idCounter = newIdCounter;

    while (!fileMutex.tryLock(1000));

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekp(0, std::ios_base::beg);
        file.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));
        file.close();
    }
    else std::cout << "Could not open file " << filename_ << std::endl;

    fileMutex.unlock();
}

#endif /* DATABASE_H_ */
