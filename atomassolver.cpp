#include "atomassolver.h"

AtomasSolver::AtomasSolver(AtomasEngine *engine)
{
    this->connectEngine(engine);
}

// Make THE IDEAL choice of the place to put the next atom
// Search for smart insert place and insert value
int AtomasSolver::solve()
{
    // Read length of the atom ring (will stay the same until function ends)
    this->atomsLength = this->engine->atoms.length();

    // Smart solving method
    if(ATOMAS_SOLVING_METHOD == choose_smart) return this->smartSolve();
    // Predictive solving
    else return this->predictiveSolve(ATOMAS_SOLVING_OPTIMIZATION);
}

int AtomasSolver::smartSolve()
{
    // Place to put the next atom to
    int insertPlace = -1;

    // Is the next atom a plus atom? -> Figure out where to put it
    if(this->engine->nextAtom == plus_atom) insertPlace = this->smartPlusInsert();
    // Next atom is a regular atom -> Figure out where to put it
    else insertPlace = this->smartAtomInsert();

    // Smart search successful
    if(insertPlace >= 0)
    {
        if(DEBUG_LEVEL > 1) qDebug() << "Smart insert of" << this->engine->nextAtom << "Atom at" << insertPlace;
    }
    // Smart search did not find any matching atom -> Insert at random place
    else
    {
        insertPlace = this->randomInsert();
        if(DEBUG_LEVEL > 1) qDebug() << "Random insert at" << insertPlace;
    }
    return insertPlace;
}

// Performs predictive solving (slow, but nearly ideal), with given optimization
int AtomasSolver::predictiveSolve(int optimization)
{
    return this->randomInsert();
}

// Returns the ideal place to put a plus atom
// Returns -1 if no smart place could be determined
int AtomasSolver::smartPlusInsert()
{
    // Place to put the next atom to
    int insertPlace = -1;
    // Determine first fusionable pair (smart)
    int fusionTestPlaces[2];
    // Test each pair until fusionable pair found or end of ring reached
    for(fusionTestPlaces[0]=0; (fusionTestPlaces[0] < this->atomsLength) && (insertPlace == -1); fusionTestPlaces[0]++)
    {
        // Fusion only possible with actual atoms
        if(this->engine->atoms.at(fusionTestPlaces[0]) > 0)
        {
            // Determine second fusion partner (wrap around if necessary)
            fusionTestPlaces[1] = abs((fusionTestPlaces[0]+1)%(this->atomsLength));
            // Test if fusion is possible
            if(this->engine->atoms.at(fusionTestPlaces[0]) == this->engine->atoms.at(fusionTestPlaces[1]))
            {
                // Fusionable pair found, stop searching
                insertPlace = fusionTestPlaces[0]+1;
            }
        }
    }

    return insertPlace;
}

// Returns the ideal place to put a normal atom
// Returns -1 if no smart place could be determined
// ToDo: Do plusatom-check for all matching atoms
int AtomasSolver::smartAtomInsert()
{
    // Place to put the next atom to
    int insertPlace = -1;
    // Create and fill list of all matching atoms
    QList<int> matchingAtoms;
    for(int i=0; i<this->atomsLength; i++) if(this->engine->atoms.at(i) == this->engine->nextAtom) matchingAtoms.append(i);
    // Look for plus atom near each matching atom until list ended or one found
    for(int i=0; (i<matchingAtoms.length()) && (insertPlace == -1); i++)
    {
        // More than two atoms in the ring?
        if(this->atomsLength > 2)
        {
            // Check if a plus atom is the left neighbour to the matching atom -> Insert left to the plus atom
            if(this->engine->atoms.at(abs((matchingAtoms.at(i)-1)%(this->atomsLength))) == plus_atom) insertPlace = abs((matchingAtoms.at(i)-1)%(this->atomsLength));
            // Check if a plus atom is the left neighbour to the matching atom -> Insert right to the plus atom
            else if(this->engine->atoms.at(abs((matchingAtoms.at(i)+1)%(this->atomsLength))) == plus_atom) insertPlace = abs((matchingAtoms.at(i)+2)%(this->atomsLength));
        }
    }
    // No plus atom + matching atom found -> insert next to matching atom if possible
    if(insertPlace == -1) insertPlace = this->engine->atoms.indexOf(this->engine->nextAtom);
    return insertPlace;
}

// Generates a random insert place
// Returns a random number between 0 and atomsLength+1
int AtomasSolver::randomInsert()
{
    return QRandomGenerator::global()->bounded(this->atomsLength+1);
}

// Connects solver to given engine instance (writes adress into this->engine)
void AtomasSolver::connectEngine(AtomasEngine *engine)
{
    this->engine = engine;
}
