// Nei calcoli di complessita', n indica la dimensione del dizionario

#include "dictionary.h"

using namespace dict;


/****************************************************************/
nodo* deleteMin (nodo* sottoAlbero, bool dir) { // dir = { false: rimuovendo a sinistra, true: rimuovendo a destra }
    if (sottoAlbero->sinistra == nullptr && sottoAlbero->destra == nullptr)
        return sottoAlbero;
    
    if (dir) { // Cerchiamo a destra
        if (sottoAlbero->destra == nullptr)
            return sottoAlbero;
        else
            return deleteMin(sottoAlbero->destra, dir);
    } else { // Cerchiamo a sinistra
        if (sottoAlbero->sinistra == nullptr)
            return sottoAlbero;
        else
            return deleteMin(sottoAlbero->sinistra, dir);
    }
}

Error pippo (nodo* sottoAlbero, nodo* padre) {
    if (padre->sinistra == sottoAlbero) {
        padre->sinistra = deleteMin(sottoAlbero, false);
        padre->sinistra = sottoAlbero->sinistra;
    } else {
        padre->destra = deleteMin(sottoAlbero, true);
        padre->destra = sottoAlbero->destra;
    }

    /*
    if (sottoAlbero->sinistra == nullptr && sottoAlbero->destra == nullptr) // Nessun figlio (quindi foglia)
        lato = nullptr;
    else if (sottoAlbero->sinistra != nullptr && sottoAlbero->destra != nullptr) { // Due figli

    } else if (sottoAlbero->sinistra != nullptr) // 1 figlio (sinistra)
        lato = sottoAlbero->sinistra;
    else if (sottoAlbero->destra != nullptr) // 1 figlio (destra)
        lato = sottoAlbero->destra;
    */
   
    delete sottoAlbero;
}

Error deleteRicorsiva (nodo* &sottoAlbero, Key k) {
    if (sottoAlbero->sinistra != nullptr && sottoAlbero->sinistra->elem.key == k) {
        pippo(sottoAlbero->sinistra, sottoAlbero);
    } else if (sottoAlbero->destra != nullptr && sottoAlbero->destra->elem.key == k) {
        pippo(sottoAlbero->destra, sottoAlbero);
    } else {
        if (sottoAlbero->sinistra != nullptr)
            deleteRicorsiva(sottoAlbero->sinistra, k);
        if (sottoAlbero->destra != nullptr)
            deleteRicorsiva(sottoAlbero->destra, k);
    }
}

Error dict::deleteElem(const Key k, Dictionary& s) {
    deleteRicorsiva(s, k);
}


/****************************************************************/
Dictionary readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs) {cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; return createEmptyDict();}  
    // cout << "\n[dict::readFromFile] Apertura file completata\n";
    return readFromStream(ifs);
}



/****************************************************************/
Dictionary readFromStdin()
{
    cout << "\nInserire una sequenza di linee che rispettano la sintassi key: value.<enter>\nDigitare CTRL^ D per terminare l'inserimento\n";
    Dictionary d = readFromStream((std::cin));
// Questa serve per aggirare un bug di alcune versioni attuali di glibc.
    clearerr(stdin);
    return d;
}


/****************************************************************/
Dictionary readFromStream(istream& str)
{
    Dictionary d = createEmptyDict();      
    string key, kcopy;
    string value;
    getline (str, key, ':'); // cout << "\n[readFromStream] key: " << key;
    getline (str, value); // cout << "\n[readFromStream] value: " << value;
    while (!str.eof()) {         
        kcopy = key;
        removeBlanksAndLower(kcopy);
        insertElem(kcopy, value, d);
        getline (str, key, ':'); 
        getline (str, value); 
    }
    str.clear();
    return d;
}


/****************************************************************/
// Inizializzazione della lista con creazione del nodo sentinella; obbligatorio dopo la dichiarazione di una lista
Dictionary dict::createEmptyDict()
{
    return nullptr;
}


/****************************************************************/

Error insertRicorsiva (nodo* sottoAlbero, nodo* n) {
    if (sottoAlbero->elem.key == n->elem.key)
        return FAIL;
    
    if (n->elem.key < sottoAlbero->elem.key) {
        if (sottoAlbero->sinistra == nullptr) 
            sottoAlbero->sinistra = n;
        else
            insertRicorsiva(sottoAlbero->sinistra, n);
    } else {
        if (sottoAlbero->destra == nullptr)
            sottoAlbero->destra = n;
        else
            insertRicorsiva(sottoAlbero->destra, n);
    }

    return OK;
}

Error dict::insertElem(const Key k, const Value v,  Dictionary& s)
{
    Elem e = Elem();
    e.key = k;
    e.value = v;

    nodo* n = new nodo();
    n->elem = e;

    if (s == nullptr)
        s = n;
    else
        return insertRicorsiva(s, n);
    
    return OK;
}


/****************************************************************/
Value searchRicorsiva (nodo* sottoAlbero, Key k) {
    if (sottoAlbero == nullptr) {
        return emptyValue;
    }

    Key currK = sottoAlbero->elem.key;
    if (currK == k) {
        return sottoAlbero->elem.value;
    }

    if (k < currK) {
        return searchRicorsiva(sottoAlbero->sinistra, k);
    } 
    if (k > currK) {
        return searchRicorsiva(sottoAlbero->destra, k);
    }
}

Value dict::search(const Key k, const Dictionary& s)
{
    return searchRicorsiva(s, k);
}


/****************************************************************/
void printRicorsiva (nodo* sottoAlbero) {
    if (sottoAlbero == nullptr)
        return;
    
    printRicorsiva(sottoAlbero->sinistra);
    std::cout << sottoAlbero->elem.key << " : " << sottoAlbero->elem.value << std::endl;
    printRicorsiva(sottoAlbero->destra);
}

void print(const Dictionary& s) {
    printRicorsiva(s);
}
