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

private:
#ifdef COMPILE_TESTS
    const bool testing;
#endif

    std::string filename_, databaseDirectory_, backupDirectory_, pdfDirectory_;
    int idCounter;
    QMutex fileMutex;
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

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str(), false);

    std::ifstream file;
    file.open(filename_.c_str(), std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&idCounter), sizeof(idCounter));
        file.close();
    }
    else
    {
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

    Encrypter::encryptFile(filename_.c_str());
    fileMutex.unlock();
}

template<class recordType>
Database<recordType>::~Database()
{
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream newFile;
    newFile.open((filename_ + ".temp").c_str(), std::ios::out | std::ios::binary);
    if (newFile.is_open())
    {
        newFile.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));

        bool fileCopied = false;
        recordType tempRecord;
        std::fstream file;
        file.open(filename_.c_str(), std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            file.seekg(sizeof(idCounter), std::ios_base::beg);

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

        if (fileCopied)
        {
            remove(filename_.c_str());
            rename((filename_ + ".temp").c_str(), filename_.c_str());
        }
        else remove((filename_ + ".temp").c_str());
    }
    else std::cout << "Could not create temporary file " + filename_ + ".temp" << std::endl;

    Encrypter::encryptFile(filename_.c_str());
    fileMutex.unlock();

    backupFile();
}

template<class recordType>
void Database<recordType>::addRecord(recordType &record)
{
    record.validate();

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekp(0, std::ios_base::end);
        record.id = idCounter++;
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
bool Database<recordType>::updateRecord(const recordType &record)
{
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

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord == record)
            {
                file.seekp(-recordType::size(), std::ios::cur);
                record.writeToFile(file);
                file.close();
                Encrypter::encryptFile(filename_.c_str());
                fileMutex.unlock();
                return true;
            }
        }
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

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord.getId() == id)
            {
                file.seekp(-recordType::size(), std::ios::cur);
                recordType().writeToFile(file);
                file.close();
                Encrypter::encryptFile(filename_.c_str());
                fileMutex.unlock();
                return true;
            }
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
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
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

    return recordType();
}

template<class recordType> template <typename type>
std::tr1::shared_ptr< std::vector<recordType> >
Database<recordType>::findRecords(const std::string &fieldName, const type &searchTerm)
{
    recordListPtr returnList(new recordList);

    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
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
    std::string lowercaseFieldName = lowerCase(fieldName);
    recordListPtr returnList(new recordList);

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

    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (!records[i].hasMatchingField(lowercaseFieldName, searchTerm))
        {
            records.erase(records.begin() + i);
            --i;
        }
    }
}

template<typename recordType>
void Database<recordType>::keepRecords(recordList &records, bool (*keepRecordFunction)(const recordType &, void *),
                                       void *data)
{
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
    while (!fileMutex.tryLock(1000));
    Encrypter::decryptFile(filename_.c_str());

    std::fstream file;
    file.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        recordType tempRecord;

        file.seekg(0, std::ios_base::end);
        unsigned size = file.tellg();

        if (sizeof(idCounter) + (recordType::size() * (index + 1)) > size) // index + 1 because index starts from 0
        {
            file.close();
            Encrypter::encryptFile(filename_.c_str());
            fileMutex.unlock();
            return recordType();
        }

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

    return 0;
}

template<class recordType>
void Database<recordType>::sortRecords(recordList &records, const unsigned startIndex, const unsigned endIndex,
                                       int (*comparisonFunction)(const recordType &, const recordType &))
{
    if (endIndex - (startIndex - 1) < 2) return;

    unsigned pivotPointer = startIndex, moveablePointer = endIndex;

    recordType temp;
    while (pivotPointer != moveablePointer)
    {
        if (comparisonFunction(records[pivotPointer], records[moveablePointer]) > -1)
        {
            if (pivotPointer < moveablePointer)
            {
                temp = records[pivotPointer];
                records[pivotPointer] = records[moveablePointer];
                records[moveablePointer] = temp;

                int tempPointer = pivotPointer;
                pivotPointer = moveablePointer;
                moveablePointer = tempPointer;
            }
            else ++moveablePointer;
        }
        else
        {
            if (pivotPointer > moveablePointer)
            {
                temp = records[pivotPointer];
                records[pivotPointer] = records[moveablePointer];
                records[moveablePointer] = temp;

                int tempPointer = pivotPointer;
                pivotPointer = moveablePointer;
                moveablePointer = tempPointer;
            }
            else --moveablePointer;
        }
    }

    if (pivotPointer > 0) sortRecords(records, startIndex, pivotPointer - 1, comparisonFunction);
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

    if (databaseDirectory_.empty()) databaseDirectory_ = QDir::currentPath().toStdString();
    if (backupDirectory_.empty()) backupDirectory_ = QDir::currentPath().toStdString();
    if (pdfDirectory_.empty()) pdfDirectory_ = QDir::currentPath().toStdString();

    if (!databaseDirectory_.empty() && (databaseDirectory_[databaseDirectory_.length() - 1] != slashChar))
        databaseDirectory_ += slashChar;
    if (!backupDirectory_.empty() && (backupDirectory_[backupDirectory_.length() - 1] != slashChar))
        backupDirectory_ += slashChar;
    if (!pdfDirectory_.empty() && (pdfDirectory_[pdfDirectory_.length() - 1] != slashChar))
        pdfDirectory_ += slashChar;


    std::string previousDatabaseDirectory = databaseDirectory_,
            previousBackupDirectory = backupDirectory_,
            previousPdfDirectory = pdfDirectory_,
            previousFilename = filename_;

    Setting databaseDirectorySetting, backupDirectorySetting;

    try { databaseDirectorySetting = SettingForm::getDatabaseDirectory(); }
    catch (const std::exception &e) { databaseDirectorySetting = Setting(); }

    try { backupDirectorySetting = SettingForm::getBackupDirectory(); }
    catch (const std::exception &e) { backupDirectorySetting = Setting(); }

    try { pdfDirectory_ = SettingForm::getPdfDirectoryWithoutSlash().toStdString(); }
    catch (const std::exception &e) { pdfDirectory_ = Setting().getValue(); }

    databaseDirectory_ = databaseDirectorySetting.getValue();
    backupDirectory_ = backupDirectorySetting.getValue();

    if (!databaseDirectory_.empty() && (databaseDirectory_[databaseDirectory_.length() - 1] != slashChar))
        databaseDirectory_ += slashChar;
    if (!backupDirectory_.empty() && (backupDirectory_[backupDirectory_.length() - 1] != slashChar))
        backupDirectory_ += slashChar;

    filename_ = databaseDirectory_ + recordType::databaseFilename;

#ifdef COMPILE_TESTS
    if (testing) filename_ += ".test";
#endif

    if (!moveFiles) return;

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
    }

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

    std::string backupFolderName =
            backupDirectory_ + "backups" + slashChar + "backup_" + toString(Date(time(NULL)).year) + "_"
            + toString(QDate::currentDate().weekNumber());

    QDir backupDirectory(backupFolderName.c_str());
    if (!backupDirectory.exists()) backupDirectory.mkpath(backupFolderName.c_str());

    try { copyFile(filename_.c_str(), (backupFolderName + slashChar + recordType::databaseFilename).c_str()); }
    catch (const std::exception &e) { std::cout << e.what() << std::endl; }
}

#endif /* DATABASE_H_ */
