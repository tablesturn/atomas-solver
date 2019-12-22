#ifndef ATOMASENGINE_H
#define ATOMASENGINE_H

#include <QList>
#include <QDebug>
#include <QRandomGenerator>

#include "main.h"

// Assignment of special atoms to numbers
enum {empty_atom=0, plus_atom=-1, minus_atom=-2, blackplus_atom=-3, luxon=-4, neutrino=-5};

class AtomasEngine
{
// Public functions
public:
    // Constructor
    // Resets the engine state
    AtomasEngine();
    // Performs one iteration of the Atomas gameplay
    // nextPlace: The place to put the next atom into
    // Returns 0 if we're still alive, and 1 if game over (maximum atoms reached)
    int iterate(int nextPlace);
// Public variables
    // Variables
    // The momentary atom ring
    QList<int> atoms;
    // The next atom
    int nextAtom;
// Private functions
private:
    // Performs all pending fusions
    // Returns the number of fusions performed
    int fusionAll();
    // Tries to perform a fusion at a given place
    int tryFusionAt(int fusionPlace);
    // Performs a fusion at a given place (Does not check if this is valid!)
    void fusionRegularAt(int fusionPlace);
    // Generates the next atom
    int generateNext();
// Private variables
    // Number of maximum atoms possible
    int maxAtoms;
};

#endif // ATOMASENGINE_H
