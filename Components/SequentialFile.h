#ifndef GG_PSQL_BD2_SEQUENTIALFILE_H
#define GG_PSQL_BD2_SEQUENTIALFILE_H

#include <string.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;

const int DELETED_REG = -2;

class RegPointer {
public:
    int index;
    char source;

    RegPointer() {
        this->index = -1;
        this->source = 'z';
    }

    RegPointer(int index, char source) {
        this->index = index;
        this->source = source;
    }
};

template<typename T>
class SequentialFile {
public:
    string fileName;
    string auxfileName;
    int size;
    int reorgThreshold;
    int headerLength;
    bool printHeader;

    SequentialFile(string fileName) {
        this->fileName = fileName;
        this->auxfileName = fileName + ".aux";
        this->reorgThreshold = 8;
        //Header: Size + lines Deleted + first row + source of first row
        this->headerLength = sizeof(int) * 3 + sizeof(char);
        printHeader = false;
    }

    void insertAll(vector<T> registros) {
        fstream out;
        bool existFile;
        int firstRecord;
        int deletedRows;
        char source;

        sort(registros.begin(), registros.end());

        //check if is first time
        ifstream filetmp (this->fileName);

        this->size = 0;
        if (filetmp.is_open()) {
            filetmp.read((char*) &this->size, sizeof(int));
            filetmp.read((char*) &deletedRows, sizeof(int));
            filetmp.read((char*) &firstRecord, sizeof(int));
            filetmp.read((char*) &source, sizeof(char));
            filetmp.close();
            existFile = true;
        } else {
            firstRecord = 0;
            source = 'd';
            deletedRows = 0;
            existFile = false;
        }

        this->size = this->size + registros.size();

        if (!existFile)
            out.open(this->fileName, ios::in | ios::out | ios::binary | ios::app );
        else
            out.open(this->fileName, ios::in | ios::out | ios::binary);

        out.write((char *) &this->size, sizeof(int));
        out.write((char *) &deletedRows, sizeof(int));
        out.write((char *) &firstRecord, sizeof(int));
        out.write((char *) &source, sizeof(char));

        out.seekg(0, ios::end);

        for(int i=0; i < size ; i++) {
            auto record = registros.at(i);
            record.next = i + 1;

            if (i == this->size - 1)
                record.next = -1;

            out.write((char *) &record, sizeof(record));
        }

        out.close();
    }

    pair<pair<RegPointer, RegPointer>, bool> exists(string key) {
        ifstream inFile;
        fstream auxFile;
        bool found;
        int l, u, m;
        T record;
        int firstRecord;
        int deletedRows;
        char source;
        char sourcefirstRecord;
        RegPointer actualNode;
        RegPointer prevNode;
        pair<pair<int, RegPointer>, bool> result;

        inFile.open(fileName, ios::binary);

        if (inFile.is_open()) {
            inFile.read((char*) &this->size, sizeof(int));
            inFile.read((char*) &deletedRows, sizeof(int));
            inFile.read((char*) &firstRecord, sizeof(int));
            inFile.read((char*) &sourcefirstRecord, sizeof(char));

            l = 0;
            u = this->size - 1;

            source = 'z';
            found=false;
            while(l <= u) {
                m = (l + u) / 2;

                if ( m == 0)
                    inFile.seekg(this->headerLength);
                else
                    inFile.seekg(m * sizeof(T) + this->headerLength);

                inFile.read((char*) &record, sizeof(T));

                if(record.name < key) {
                    //Maybe exists in auxiliar file?
                    if (record.destiny == 'a' ) {
                        result = findInAuxFile(key, record.next, m);

                        if (result.second) {
                            found=true;
                            source = 'a'; //auxiliar
                            break;
                        }
                    }
                    l = m + 1;
                    //cout << record.name << " < " << key << endl;
                } else if (record.name > key) {
                    if (record.destiny == 'a' ) {
                        //Maybe exists in auxiliar file?
                        result = findInAuxFile(key, record.next, m);

                        if (result.second) {
                            found=true;
                            source = 'a'; //auxiliar
                            break;
                        }
                    }
                    u = m - 1;
                    //cout << record.name << " > " << key << endl;
                } else {
                    source = 'd'; //principal
                    //cout << record.name << " eq " << key << endl;
                    if (record.next == DELETED_REG)
                        found=false;
                    else
                        found=true;

                    break;
                }
            }
            inFile.close();
        }

        if(!found) {
            //Maybe is the first logical record in auxiliar file?
            if ( sourcefirstRecord == 'a') {
                auxFile.open(this->auxfileName, ios::in);

                if (auxFile.is_open()) {
                    auxFile.seekg(firstRecord*sizeof(T) + sizeof(int));
                    auxFile.read((char*) &record, sizeof(T));

                    auxFile.close();
                }

                if (record.name == key && record.next != DELETED_REG ) {
                    actualNode = RegPointer(firstRecord, 'a');

                    auto tmp = make_pair(actualNode, prevNode);

                    return make_pair(tmp, true);
                } else {
                    auto tmp = make_pair(actualNode, prevNode);

                    return make_pair(tmp, found);
                }
            } else {
                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            }
        } else {
            if (source == 'd') {
                actualNode = RegPointer(m, source);

                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            } else if (source == 'a') {

                auto resIndex = (result.first).first;
                actualNode = RegPointer(resIndex, source);
                prevNode = (result.first).second;

                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            } else {
                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            }
        }
    }

    pair<pair<RegPointer, RegPointer>, bool> exists2(string key) {
        ifstream inFile;
        fstream auxFile;
        bool found;
        int low, high, guess;
        T record;
        int firstRecord;
        int deletedRows;
        char source;
        char sourcefirstRecord;
        RegPointer actualNode;
        RegPointer prevNode;
        pair<pair<int, RegPointer>, bool> result;

        inFile.open(fileName, ios::binary);
        cout << "key " << key << " len " << key.length() <<endl;

        if (inFile.is_open()) {
            inFile.read((char*) &this->size, sizeof(int));
            inFile.read((char*) &deletedRows, sizeof(int));
            inFile.read((char*) &firstRecord, sizeof(int));
            inFile.read((char*) &sourcefirstRecord, sizeof(char));

            low = 0;
            high = this->size - 1;
            cout << this->size << endl;

            source = 'z';
            found=false;
            while(low <= high) {
                guess = (high +  low) / 2;

                if (guess == 0)
                    inFile.seekg(this->headerLength);
                else
                    inFile.seekg(guess * sizeof(T) + this->headerLength);

                inFile.read((char*) &record, sizeof(T));

                if(record.name == key ) {
                    source = 'd'; //principal
                    cout << record.name << " eq " << key << endl;
                    if (record.next == DELETED_REG)
                        found=false;
                    else
                        found=true;

                    break;
                }

                if(key > record.name ) {
                    //Maybe exists in auxiliar file?
                    if (record.destiny == 'a' ) {
                        result = findInAuxFile(key, record.next, guess);

                        if (result.second) {
                            found=true;
                            source = 'a'; //auxiliar
                            break;
                        }
                    }
                    low = guess + 1;
                    cout << low << " " << high << " " << guess << " " << key << " > " << record.name << endl;
                } else if (key < record.name ) {
                    if (record.destiny == 'a' ) {
                        //Maybe exists in auxiliar file?
                        result = findInAuxFile(key, record.next, guess);

                        if (result.second) {
                            found=true;
                            source = 'a'; //auxiliar
                            break;
                        }
                    }
                    high = guess - 1;
                    cout << low << " " << high << " " <<  guess << " " << key << " < " << record.name << endl;
                }
            }
            inFile.close();
        }

        if(!found) {
            //Maybe is the first logical record in auxiliar file?
            if ( sourcefirstRecord == 'a') {
                auxFile.open(this->auxfileName, ios::in);

                if (auxFile.is_open()) {
                    auxFile.seekg(firstRecord*sizeof(T) + sizeof(int));
                    auxFile.read((char*) &record, sizeof(T));

                    auxFile.close();
                }

                if (record.name == key && record.next != DELETED_REG ) {
                    actualNode = RegPointer(firstRecord, 'a');

                    auto tmp = make_pair(actualNode, prevNode);

                    return make_pair(tmp, true);
                } else {
                    auto tmp = make_pair(actualNode, prevNode);

                    return make_pair(tmp, found);
                }
            } else {
                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            }
        } else {
            if (source == 'd') {
                actualNode = RegPointer(guess, source);

                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            } else if (source == 'a') {

                auto resIndex = (result.first).first;
                actualNode = RegPointer(resIndex, source);
                prevNode = (result.first).second;

                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            } else {
                auto tmp = make_pair(actualNode, prevNode);

                return make_pair(tmp, found);
            }
        }
    }

    T search(string key) {
        ifstream inFile;
        fstream auxFile;
        T record;

        auto result = exists(key);
        //auto result = exists2(key);

        if (result.second) {
            auto tmp = result.first;
            auto index = (tmp.first).index;
            auto source = (tmp.first).source;

            if (source == 'd') { //principal

                inFile.open(this->fileName, ios::binary);

                if (inFile.is_open()) {
                    if ( index == 0)
                        inFile.seekg(this->headerLength);
                    else
                        inFile.seekg(index * sizeof(T) + this->headerLength);

                    inFile.read((char*) &record, sizeof(T));
                    inFile.close();

                    return record;
                } else {
                    return T(); //Alumno();
                }

            } else if (source == 'a') { //auxiliar

                auxFile.open(this->auxfileName, ios::in);

                if (auxFile.is_open()) {
                    auxFile.seekg(index*sizeof(T) + sizeof(int));
                    auxFile.read((char*) &record, sizeof(T));

                    auxFile.close();

                    return record;
                } else {
                    return T(); //Alumno();
                }
            }
        } //else {
        return T(); //Alumno();
        //}
    }

    T read_from_node(RegPointer node) {
        ifstream inFile;
        fstream auxFile;
        T record;

        if (node.source == 'd') {
            inFile.open(fileName, ios::binary);

            if (inFile.is_open()) {
                inFile.seekg(node.index * sizeof(T) + this->headerLength);
                inFile.read((char*) &record, sizeof(T));
                inFile.close();
            }
        } else {
            auxFile.open(this->auxfileName, ios::in);

            if (auxFile.is_open()) {
                auxFile.seekg(node.index*sizeof(T) + sizeof(int));
                auxFile.read((char*) &record, sizeof(T));
                auxFile.close();
            }
        }

        return record;
    }

    bool write_from_node(RegPointer node, T reg, T oldReg) {
        fstream inFile;
        fstream auxFile;
        int firstRecord;
        int deletedRows;
        int sizeAux;
        char sourcefirstRecord;
        bool result;

        if ( node.index == -1)
            return false;

        inFile.open(this->fileName, ios::in | ios::binary);

        if (inFile.is_open()) {
            inFile.read((char*) &this->size, sizeof(int));
            inFile.read((char*) &deletedRows, sizeof(int));
            inFile.read((char*) &firstRecord, sizeof(int));
            inFile.read((char*) &sourcefirstRecord, sizeof(char));

            inFile.close();
        } else
            return false;

        if (node.source == 'd') {
            inFile.open(this->fileName, ios::in | ios::out | ios::binary);

            if (inFile.is_open()) {

                if ( reg.next == DELETED_REG) { //deletion
                    //this->size--;
                    deletedRows = deletedRows + 1;
                    inFile.write((char*) &this->size, sizeof(int));
                    inFile.write((char*) &deletedRows, sizeof(int));

                    if ( firstRecord == node.index &&
                         sourcefirstRecord == node.source) {
                        inFile.write((char*) &oldReg.next, sizeof(int));
                        inFile.write((char*) &oldReg.destiny, sizeof(char));
                    }
                }

                inFile.seekg(node.index * sizeof(T) + this->headerLength);
                inFile.write((char*) &reg, sizeof(T));

                inFile.close();
                result = true;
            } else {
                result = false;
            }
        } else if (node.source == 'a') {
            auxFile.open(this->auxfileName, ios::in | ios::out | ios::binary);

            if (auxFile.is_open()) {

                inFile.open(this->fileName, ios::in | ios::out | ios::binary);

                if (inFile.is_open()) {
                    if ( reg.next == DELETED_REG) { //deletion

                        auxFile.read((char*) &sizeAux, sizeof(int));
                        sizeAux--;
                        auxFile.seekg(0);
                        auxFile.write((char*) &sizeAux, sizeof(int));
                        if ( firstRecord == node.index &&
                             sourcefirstRecord == node.source) {
                            inFile.seekg(sizeof(int)*2);
                            inFile.write((char*) &oldReg.next, sizeof(int));
                            inFile.write((char*) &oldReg.destiny, sizeof(char));
                        }
                    }
                    inFile.close();
                }

                auto offset = sizeof(int);
                auxFile.seekg(node.index * sizeof(T)+offset);
                auxFile.write((char*) &reg, sizeof(T));

                auxFile.close();
                result = true;
            } else {
                result = false;
            }
        } else {
            result = false;
        }

        return result;
    }

    bool delete_key(string key) {
        auto result = exists(key);
        //auto result = exists2(key);
        auto actualNode = (result.first).first;
        auto prevNode = (result.first).second;

        if (result.second) {

            auto prevNodetmp = getPrevious(actualNode.index, key, actualNode.source, prevNode);
            auto actualReg = read_from_node(actualNode);
            auto prevReg = read_from_node(prevNodetmp);

            prevReg.next = actualReg.next;
            prevReg.destiny = actualReg.destiny;

            auto oldReg = actualReg;

            actualReg.next = DELETED_REG;
            actualReg.destiny = actualNode.source;

            auto resPrev = write_from_node(prevNodetmp, prevReg, oldReg);
            auto resAct = write_from_node(actualNode, actualReg, oldReg);

            return true;
        } else {
            return false;
        }
    }

    RegPointer getPrevious(int index, string key, char source, RegPointer node) {
        ifstream inFile;
        fstream auxFile;
        T record;
        bool found;
        int firstRecord;
        char sourcefirstRecord;

        if ( source == 'd' ) {

            inFile.open(this->fileName, ios::binary);

            if (inFile.is_open()) {
                inFile.read((char*) &this->size, sizeof(int));
                inFile.read((char*) &firstRecord, sizeof(int));
                inFile.read((char*) &sourcefirstRecord, sizeof(char));

                //is the first record?
                if (index == 0 ) {
                    if ( sourcefirstRecord == 'd' ) {
                        if (firstRecord == 0) {
                            //return make_pair(-1, source);
                            return RegPointer(-1, source);
                        }
                    }
                    //find in auxiliar file
                    auto result = findInAuxFile(key, firstRecord, index);

                    auto tmp = result.first;
                    auto nodePrev = tmp.second;

                    return RegPointer(nodePrev.index, nodePrev.source);
                } else {
                    auto indextmp = index - 1;

                    found = false;
                    while(indextmp >= 0) {
                        if ( indextmp == 0) {
                            inFile.seekg(this->headerLength);
                        } else {
                            inFile.seekg(indextmp * sizeof(T) + this->headerLength);
                        }

                        inFile.read((char*) &record, sizeof(T));

                        if (record.next != DELETED_REG) {
                            found = true;
                            break;
                        }
                        indextmp--;
                    }

                    if(found) {
                        char sourceTmp = 'd';

                        if(record.destiny == 'a') {
                            auto result = findInAuxFile(key, record.next, indextmp);

                            auto tmp = result.first;
                            indextmp = tmp.first;
                            sourceTmp = 'a';
                        }
                        return RegPointer(indextmp, sourceTmp);
                    } else {
                        //maybe it's in auxiliar
                        auto result = findInAuxFile(key, firstRecord, 0);

                        auto tmp = result.first;
                        auto nodePrev = tmp.second;

                        return RegPointer(nodePrev.index, nodePrev.source);
                    }
                }
            }

        } else if ( source == 'a' ){

            return RegPointer(node.index, node.source);
        }
    }

    vector<T> search(string begin, string end) {
        ifstream inFile;
        T record;
        vector<T> result;

        inFile.open(this->fileName, ios::binary);

        result.clear();

        if (inFile.is_open()) {
            inFile.read((char*) &this->size, sizeof(int));
            //auto offset = sizeof(int) * 2 + sizeof(char);

            for(int index=0; index < this->size;index++ ) {
                inFile.seekg(index * sizeof(T) + this->headerLength);
                inFile.read((char*) &record, sizeof(T));

                if (record.name >= begin && record.name <= end ) {

                    result.push_back(record);
                }
            }
            inFile.close();
        }
        return result;
    }

    void add(T record) {
        int auxFileSize;
        //Alumno alumnotmp;
        T recordPrev;
        fstream auxFile;
        fstream inFile;
        bool existFile;
        int offset;
        //logical position to insert
        auto indexInsert = this->getIndex(record.name);

        ifstream filetmp(this->auxfileName);

        if (filetmp.is_open()) {
            filetmp.read((char*) &auxFileSize, sizeof(int));
            filetmp.close();
            existFile = true;
            auxFileSize++;
        } else {
            existFile = false;
            auxFileSize = 1;
        }

        if (!existFile)
            auxFile.open(this->auxfileName, ios::in | ios::out | ios::binary | ios::app );
        else
            auxFile.open(this->auxfileName, ios::in | ios::out | ios::binary);

        auxFile.write((char *) &auxFileSize, sizeof(int));

        if ( indexInsert.second == 'd') {
            inFile.open(this->fileName, ios::in | ios::out | ios::binary);

            if (inFile.is_open()) {
                inFile.read((char*) &this->size, sizeof(int));

                //offset = sizeof(int)*2 + sizeof(char);
                offset = this->headerLength;
                inFile.seekg(indexInsert.first * sizeof(T) + offset);
                inFile.read((char*) &recordPrev, sizeof(T));

                record.destiny = recordPrev.destiny;
                if (indexInsert.first != 0)
                    record.next = recordPrev.next;
                else
                    record.next = 0;

                recordPrev.destiny = 'a';
                recordPrev.next = auxFileSize - 1;

                if (indexInsert.first != 0) {
                    inFile.seekg(indexInsert.first * sizeof(T)+offset);
                    inFile.write((char*) &recordPrev, sizeof(T));
                } else {
                    //update first record pointer
                    inFile.seekg(sizeof(int) * 2); // 1
                    inFile.write((char*) &recordPrev.next, sizeof(int));
                    inFile.write((char*) &recordPrev.destiny, sizeof(char));
                }

                inFile.close();
            }
        } else { //aux
            offset = sizeof(int);
            auxFile.seekg(indexInsert.first * sizeof(T)+offset);
            auxFile.read((char*) &recordPrev, sizeof(T));

            record.destiny = recordPrev.destiny;
            record.next = recordPrev.next;

            recordPrev.destiny = 'a';
            recordPrev.next = auxFileSize - 1;

            auxFile.seekg(indexInsert.first * sizeof(T)+offset);
            auxFile.write((char*) &recordPrev, sizeof(T));
        }

        auxFile.seekg(0, ios::end);
        auxFile.write((char *) &record, sizeof(record));
        auxFile.close();

        if ( auxFileSize == this->reorgThreshold )
            this->reorg();
    }

    pair<int,char> getIndex(string key) {
        ifstream inFile;
        int l, u, m;
        T record;
        int deletedRows;

        inFile.open(fileName, ios::binary);

        if (inFile.is_open()) {
            inFile.read((char*) &this->size, sizeof(int));
            inFile.read((char*) &deletedRows, sizeof(int));
            l = 0;
            u = this->size - 1;

            while(l <= u) {
                m = (l + u) / 2;

                if ( m == 0)
                    inFile.seekg(this->headerLength);
                else
                    inFile.seekg(m * sizeof(T) + this->headerLength);

                inFile.read((char*) &record, sizeof(T));

                if(record.name < key) {
                    l = m + 1;
                } else if (record.name > key) {
                    u = m - 1;
                } else {

                    if (record.destiny == 'd') //principal
                        return make_pair(m, record.destiny);
                    else {
                        auto tmp = findInAuxFile(key, m, m).first;
                        return make_pair(tmp.first, 'a');
                    }
                }
            }

            if(u==-1)
                u = 0;

            inFile.seekg(u * sizeof(T) + this->headerLength);
            inFile.read((char*) &record, sizeof(T));
            inFile.close();
        }

        if (record.destiny == 'd') //principal
            return make_pair(u, record.destiny);
        else {
            auto tmp = findInAuxFile(key, record.next, record.next).first;
            return make_pair(tmp.first, 'a');
        }
    }

    pair<pair<int, RegPointer>,bool> findInAuxFile(string key, int index, int index__1) {
        int auxFileSize;
        int indexAux;
        int prevIndex;
        char prevSource;
        T record;
        bool found;
        RegPointer previous;

        ifstream auxtmp(this->auxfileName);
        auto offset = sizeof(int);

        found = false;
        if (auxtmp.is_open()) {
            auxtmp.read((char*) &auxFileSize, sizeof(int));

            indexAux = index;
            auxtmp.seekg(indexAux * sizeof(T) + offset);
            auxtmp.read((char*) &record, sizeof(T));

            prevIndex = index__1; //indexAux;
            prevSource = 'd';
            while(key > record.name) {
                //indexAux = index;
                if (record.destiny == 'd') {
                    found = false;
                    //prevSource = 'd'; // LAST CHANGE
                    break;
                } else {
                    //prevIndex = record.next; // LAST CHANGE
                    prevIndex = indexAux; // LAST CHANGE
                    indexAux = record.next; // LAST CHANGE
                    prevSource = 'a';
                    auxtmp.seekg(record.next * sizeof(T) + offset);
                    auxtmp.read((char*) &record, sizeof(T));
                }
            }
            auxtmp.close();
        }

        if (key == record.name)
            if (record.next == DELETED_REG)
                found = false;
            else
                found = true;
        else
            found = false;
        previous = RegPointer(prevIndex, prevSource);
        return make_pair(make_pair(indexAux, previous), found);
    }

    vector<T> load(char fileSource) {
        vector<T> tmp;
        T record;
        fstream stream;
        string tmpFilename;
        int size;
        int deletedRows;
        int firstRecord;
        char source;

        if(fileSource == 'd')
            tmpFilename = this->fileName;
        else
            tmpFilename = this->auxfileName;

        stream.open(tmpFilename, ios::in);

        tmp.clear();
        if (stream.is_open()) {
            if(fileSource == 'd') {
                stream.read((char*) &this->size, sizeof(int));
                stream.read((char*) &deletedRows, sizeof(int));
                stream.read((char*) &firstRecord, sizeof(int));
                stream.read((char*) &source, sizeof(char));
                size = this->size;
            } else {
                stream.read((char*) &size, sizeof(int));
            }

            if (printHeader) {
                if(fileSource == 'd') {
                    cout << "Size " << size << " Del " << deletedRows << endl;
                    cout << "first-> "<<firstRecord<<"-"<<source<<endl;
                } else if(fileSource == 'a')
                    cout << "Size " << size << endl;
            }

            while(stream.read((char*) &record, sizeof(T))) {
                if(record.next != DELETED_REG)
                    tmp.push_back(record);
            }
            stream.close();
        } else {
            cout << " File " << tmpFilename <<" Not found " <<endl;
        }
        return tmp;
    }

    void reorg() {
        vector<T> tmp;
        fstream mainFile;
        fstream auxFile;
        int offset;
        int offsetAux;
        int nextIndex;
        T record;
        char source;

        offset = sizeof(int)*2; // before 1

        tmp.clear();

        mainFile.open(this->fileName, ios::in);
        auxFile.open(this->auxfileName, ios::in);

        if(mainFile.is_open()) {
            mainFile.seekg(offset);
            mainFile.read((char*) &nextIndex, sizeof(int));//first index
            mainFile.read((char*) &source, sizeof(char));

            //offset = sizeof(int)*2 + sizeof(char);
            offsetAux = sizeof(int);

            //cout << "nextIndex->" << nextIndex << " " << source<<endl;
            while(nextIndex != -1) {
                if (source == 'd') {
                    mainFile.seekg(nextIndex*sizeof(T) + this->headerLength);
                    mainFile.read((char*) &record, sizeof(T));
                } else {
                    if (auxFile.is_open()) {
                        auxFile.seekg(nextIndex*sizeof(T) + offsetAux);
                        auxFile.read((char*) &record, sizeof(T));
                    }
                }
                source = record.destiny;
                record.destiny = 'd';

                tmp.push_back(record);
                nextIndex = record.next;
                //cout << "nextIndex->" << nextIndex << " " << source<<endl;
            }

            if(auxFile.is_open())
                auxFile.close();

            mainFile.close();
        }

        const char * file1 = this->fileName.c_str();
        const char * file2 = this->auxfileName.c_str();

        remove(file1);
        remove(file2);
        this->insertAll(tmp);
    }

    int length() {
        fstream mainFile;
        fstream auxFile;
        int sizeMain=0;
        int deletedRows=0;
        int sizeAux=0;

        mainFile.open(this->fileName, ios::in);
        auxFile.open(this->auxfileName, ios::in);

        if(mainFile.is_open()) {
            mainFile.read((char*) &sizeMain, sizeof(int));
            mainFile.read((char*) &deletedRows, sizeof(int));

            mainFile.close();
        }

        if(auxFile.is_open()) {
            auxFile.read((char*) &sizeAux, sizeof(int));

            auxFile.close();
        }

        return sizeMain - deletedRows + sizeAux;
    }
};
#endif