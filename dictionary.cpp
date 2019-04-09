// Nei calcoli di complessita', n indica la dimensione del dizionario

#include "dictionary.h"

using namespace dict;


/****************************************************************/
nodo* minoreDeiMaggiori (nodo* node) { // Cerco la foglia piu' a sinistra
    if (node->sinistra != nullptr)
        return minoreDeiMaggiori(node->sinistra);

    return node;
}

nodo* deleteNode (nodo* sottoAlbero, Key k) {
    if (sottoAlbero == nullptr) // Se non esiste con la chaive
        return nullptr; // Restituisco nullptr (che verra' interpretato come errore)
  
    if (k < sottoAlbero->elem.key) // Se la chiave e' minore
        sottoAlbero->sinistra = deleteNode(sottoAlbero->sinistra, k); // Continuo a sinistra
    else if (k > sottoAlbero->elem.key) // Se la chiave e' maggiore
        sottoAlbero->destra = deleteNode(sottoAlbero->destra, k); // Continuo a destra
    else { // Se ho trovato la chiave
        if (sottoAlbero->sinistra == NULL) { // Se non ha un figlio a sinistra
            struct nodo* temp = sottoAlbero->destra;
            delete sottoAlbero; // Elimino il nodo dalla memoria
            return temp; // Restituisco la parte destra
        } else if (sottoAlbero->destra == NULL) { // Se non ha un figlio a destra
            struct nodo* temp = sottoAlbero->sinistra; 
            delete sottoAlbero; // Elimino il nodo dalla memoria
            return temp; // Restituisco la parte sinistra
        } else { // Se ha entrambi i figli
            nodo* temp = minoreDeiMaggiori(sottoAlbero->destra);  // Prendo il figlio minore nel sotto albero di destra (il nodo successivo)
            sottoAlbero->elem = temp->elem; // Copio il contenuto del nodo
            sottoAlbero->destra = deleteNode(sottoAlbero->destra, temp->elem.key); // Elimino il nodo successivo (perche' ho spostato il suo contenuto nel nodo corrente)
        }
    }

    return sottoAlbero;
} 

Error dict::deleteElem(const Key k, Dictionary& s) {
    nodo* newsottoAlbero = deleteNode(s, k);

    if (newsottoAlbero == nullptr)
        return FAIL;
    s = newsottoAlbero;

    return OK;
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
Dictionary dict::createEmptyDict() {
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
    if (sottoAlbero == nullptr)
        return emptyValue;

    Key currK = sottoAlbero->elem.key;
    if (currK == k)
        return sottoAlbero->elem.value;

    if (k < currK)
        return searchRicorsiva(sottoAlbero->sinistra, k);
    if (k > currK)
        return searchRicorsiva(sottoAlbero->destra, k);
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
