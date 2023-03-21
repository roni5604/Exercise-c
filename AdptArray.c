#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

// the main struct
typedef struct AdptArray_ {
    PElement* elements;
    int size;
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
} *PAdptArray,AdptArray;

// create a new array with the given copy, delete and print functions
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    if (!copyFunc || !delFunc || !printFunc) {
        return NULL;
    }
    PAdptArray pArray = (PAdptArray) malloc(sizeof(AdptArray));
    if (!pArray) {
        return NULL;
    }
    pArray->elements = NULL;
    pArray->size = 0;
    pArray->copyFunc = copyFunc;
    pArray->delFunc = delFunc;
    pArray->printFunc = printFunc;
    return pArray;
}

// delete the array and free all memory
void DeleteAdptArray(PAdptArray pArray) {
    if (!pArray) {
        return ;
    }
   if (pArray->elements) {
        for (int i = 0; i < pArray->size; i++) {
            if (pArray->elements[i]) {
                pArray->delFunc(pArray->elements[i]);
            }
        }
        free(pArray->elements);
    }
   
    free(pArray);
}

// add a new element to the array at the given index 
Result SetAdptArrayAt(PAdptArray pArray, int index, PElement element) {
    if (!pArray || index < 0 || !element) {
        return FAIL;
    }
    else if (pArray->size == 0)
    {
       
        pArray->elements = (PElement*) malloc(sizeof(PElement) * (index + 1));
        pArray->size = index + 1;
        for (int i = 0; i < pArray->size; i++)
        {
            pArray->elements[i] = NULL;
        }
        
        pArray->elements[index] = pArray->copyFunc(element);
        return SUCCESS;
    }
    else if (index < pArray->size) {
        PElement oldElement = pArray->elements[index];
        if (oldElement) {
            pArray->delFunc(oldElement);
        }
        pArray->elements[index] = pArray->copyFunc(element);
        return SUCCESS;
    }
    else if (index >= pArray->size)
    {
        PElement* newElement=(PElement*) calloc(index + 1, sizeof(PElement) );
        for (int i = 0; i < pArray->size; i++)
        {
            if (pArray->elements[i])
            {
                newElement[i]= pArray->copyFunc(pArray->elements[i]);
                pArray->delFunc(pArray->elements[i]);
            }
        }
        pArray->size = index + 1;
        free(pArray->elements);
        pArray->elements = newElement;
        pArray->elements[index] = pArray->copyFunc(element);
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

// get the element at the given index 
PElement GetAdptArrayAt(PAdptArray pArray, int index) {
    if (!pArray || index < 0 || index >= pArray->size) {
        return NULL;
    }
    if (!pArray->elements[index])
    {
        return NULL;
    }
    return pArray->copyFunc(pArray->elements[index]);
}

// get the size of the array
int GetAdptArraySize(PAdptArray pArray) {
    if (!pArray) {
        return 0;
    }
    else{
    return pArray->size;}
}

// print the array
void PrintDB(PAdptArray pArray) {
    if (!pArray) {
        return;
    }
    else{
    for (int i = 0; i < pArray->size; i++) {
        if ((pArray->elements[i]))
        {
            pArray->printFunc(pArray->elements[i]);
        }
    }
    }
}

