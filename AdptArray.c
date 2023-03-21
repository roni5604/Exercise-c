#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

// the main struct
typedef struct AdptArray_ {
    PElement* elements; // the array of elements
    int size; // the size of the array
    COPY_FUNC copyFunc; // the copy function
    DEL_FUNC delFunc; // the delete function
    PRINT_FUNC printFunc;  // the print function
} *PAdptArray,AdptArray;

// create a new array with the given copy, delete and print functions
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    //if one of the functions is null
    if (!copyFunc || !delFunc || !printFunc) {
        return NULL;
    }
    PAdptArray pArray = (PAdptArray) malloc(sizeof(AdptArray));//malloc is a function that allocates memory in the heap 
    //if the allocation failed
    if (!pArray) {
        return NULL;
    }
    pArray->elements = NULL; //initialize the array to null
    pArray->size = 0; //initialize the size to 0
    pArray->copyFunc = copyFunc; 
    pArray->delFunc = delFunc;
    pArray->printFunc = printFunc;
    return pArray; //return the array
}

// delete the array and free all memory
void DeleteAdptArray(PAdptArray pArray) {
    if (!pArray) {
        return ;
    }
    //if the array is not empty
   if (pArray->elements) {
        for (int i = 0; i < pArray->size; i++) {
            //if the element is not null
            if (pArray->elements[i]) {
                pArray->delFunc(pArray->elements[i]);
            }
        }
        free(pArray->elements); //free the array
    }
   
    free(pArray); //free the struct
}

// add a new element to the array at the given index 
Result SetAdptArrayAt(PAdptArray pArray, int index, PElement element) {
    //if the array is null or the index is negative or the element is null
    if (!pArray || index < 0 || !element) {
        return FAIL;
    }
    else if (pArray->size == 0)
    {
        pArray->elements = (PElement*) malloc(sizeof(PElement) * (index + 1));//malloc is a function that allocates memory in the heap
        pArray->size = index + 1; //update the size
        for (int i = 0; i < pArray->size; i++)
        {
            pArray->elements[i] = NULL;
        }
        
        pArray->elements[index] = pArray->copyFunc(element);
        return SUCCESS;
    }
    else if (index < pArray->size) {
        PElement oldElement = pArray->elements[index];//save the old element
        if (oldElement) {
            pArray->delFunc(oldElement);//delete the old element
        }
        pArray->elements[index] = pArray->copyFunc(element);//copy the new element
        return SUCCESS;
    }
    else if (index >= pArray->size)
    {
        PElement* newElement=(PElement*) calloc(index + 1, sizeof(PElement) );//calloc is a function that allocates memory in the heap
        for (int i = 0; i < pArray->size; i++)
        {
            if (pArray->elements[i])
            {
                newElement[i]= pArray->copyFunc(pArray->elements[i]);//copy the old elements
                pArray->delFunc(pArray->elements[i]);//delete the old elements
            }
        }
        pArray->size = index + 1;//update the size
        free(pArray->elements);//free the old array
        pArray->elements = newElement;//update the array
        pArray->elements[index] = pArray->copyFunc(element);//copy the new element
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
        //if the element is not null
        if ((pArray->elements[i]))
        {
            pArray->printFunc(pArray->elements[i]);//print the element
        }
    }
    }
}

