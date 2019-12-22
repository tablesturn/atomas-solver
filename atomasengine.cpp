#include "atomasengine.h"

// Constructor
// Initializes the engine
AtomasEngine::AtomasEngine()
{
    // Configure parameters
    // Maximum number of atoms (else: game over)
    this->maxAtoms = 20;

    // Clear the atom ring
    this->atoms.clear();
    // Choose the first atom
    this->generateNext();
}

// Performs one iteration of the Atomas gameplay
// nextPlace: The place to put the next atom into
// Returns 0 if we're still alive, and 1 if game over (maximum atoms reached)
int AtomasEngine::iterate(int nextPlace)
{
    // Game over? -> No actions necessary
    if(this->atoms.length() > this->maxAtoms) return 1;

    // Put the next atom at the desired place
    this->atoms.insert(nextPlace, this->nextAtom);
    if(DEBUG_LEVEL > 1) qDebug() << this->atoms;
    // Perform all pending fusions
    this->fusionAll();
    if(DEBUG_LEVEL > 1) qDebug() << this->atoms;
    // Choose the next atom
    this->generateNext();
    return 0;
}

// Performs all pending fusions
// Returns the number of fusions performed
// ToDo: Handle occurence of multiple pluses
int AtomasEngine::fusionAll()
{
    int fusionPerformed = 0;
    // Fusion only possible with three or more elements
    if(this->atoms.length() > 2)
    {
        // Pick first plus atom
        int nextPlusAtomPlace = this->atoms.indexOf(plus_atom);
        // Repeat as long as no fusion ocurred and there is a next plus atom
        while(!fusionPerformed && nextPlusAtomPlace!=-1)
        {
            // Try to perform a fusion at the plus atom place
            fusionPerformed = this->tryFusionAt(nextPlusAtomPlace);
            // If no fusion took place, pick next plus atom
            if(!fusionPerformed) nextPlusAtomPlace = this->atoms.indexOf(plus_atom,nextPlusAtomPlace+1);
        }
    }
    if(fusionPerformed)
    {
        // One fusion performed
        if(DEBUG_LEVEL > 1) qDebug() << "Fusion:" << this->atoms;
        // Try to make another fusion if there are more plus atoms
        if(this->atoms.indexOf(plus_atom) >= 0) this->fusionAll();
    }
    // No fusion performed
    if(DEBUG_LEVEL > 1) qDebug() << "No fusion possible";
    return fusionPerformed;
}

// Tries to perform a fusion at a given place
int AtomasEngine::tryFusionAt(int fusionPlace)
{
    // The two atoms to fusion
    int fusionAtomPlace[2];
    // Determine places of fusion atoms with wrap-around
    fusionAtomPlace[0] = abs((fusionPlace-1)%this->atoms.length());
    fusionAtomPlace[1] = abs((fusionPlace+1)%this->atoms.length());

    // Check if fusion is possible, thus both surrounding atoms are regular atoms and equal
    if(((this->atoms.at(fusionAtomPlace[0])) > 0) && (this->atoms.at(fusionAtomPlace[0]) == this->atoms.at(fusionAtomPlace[1])))
    {
        // Fusion
        this->fusionRegularAt(fusionPlace);
        // A fusion ocurred
        return 1;
    }
    // No fusion ocurred
    else return 0;
}

// Performs a fusion at a given place (Does not check if this is valid!)
// ToDo: Check speed and maybe improve removing alorithm by starting from the back and remove the atoms, probably removeAll() is slow
void AtomasEngine::fusionRegularAt(int fusionPlace)
{
    // The two atoms to fusion
    int fusionAtomPlace[2];
    // Determine places of fusion atoms with wrap-around
    fusionAtomPlace[0] = abs((fusionPlace-1)%this->atoms.length());
    fusionAtomPlace[1] = abs((fusionPlace+1)%this->atoms.length());

    // Calculate fusion result (increment by one)
    this->atoms[fusionAtomPlace[0]]++;
    // Remove plus atom (set as empty)
    this->atoms[fusionPlace] = empty_atom;
    // Remove second fusion partner
    this->atoms[fusionAtomPlace[1]] = empty_atom;

    // Remove all empty atoms
    this->atoms.removeAll(empty_atom);
}

// Generates the next atom
// Returns the value of the next atom
// ToDo: Make next atom dynamic
int AtomasEngine::generateNext()
{
    // Random value to derive the next atom from
    int randomValue = QRandomGenerator::global()->bounded(2);
    // Plus atom?
    if(randomValue == 0) this->nextAtom = plus_atom;
    // 1 atom?
    else this->nextAtom = 1;

    if(DEBUG_LEVEL > 1) qDebug() << "Next atom:" << this->nextAtom;
    return this->nextAtom;
}
