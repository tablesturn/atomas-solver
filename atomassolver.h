#ifndef ATOMASSOLVER_H
#define ATOMASSOLVER_H

#include <QList>
#include <QDebug>
#include <QRandomGenerator>

#include "main.h"
#include "atomasengine.h"

// Set Solving method
#define ATOMAS_SOLVING_METHOD choose_smart
// Set optimization rule
#define ATOMAS_SOLVING_OPTIMIZATION optimize_reduce

// Set number of game cycles to predict (predict method only)
#define ATOMAS_SOLVING_DEPTH 8

// Solving method to determine the best position to place the next atom
// choose_smart: Analyzing the actual situation (fast, but not ideal)
// choose_predictive: Predict the following atoms and perform a probability-driven ranking
enum{choose_smart, choose_predictive};

// Optimization type:
// optimize_score: Tries to achieve the highest score possible to predict the ideal next place
// optimize_reduce: Tries to reduce the number of atoms as much as possible
// optimize_heaviest: Tries to get the heaviest atom possible
enum{optimize_score, optimize_reduce, optimize_heaviest};

class AtomasSolver
{
// Public functions
public:
    // Constructor
    AtomasSolver(AtomasEngine *engine);
    // Connects solver to given engine instance (writes adress into this->engine)
    void connectEngine(AtomasEngine *engine);
    // Perform one solving iteration
    // Returns the place to put the next atom
    int solve();
    // Performs smart solving (fast, but not ideal)
    int smartSolve();
    // Performs predictive solving (slow, but nearly ideal), with given optimization
    int predictiveSolve(int optimization);

// Private functions
private:
// Insert functions
    // Returns the ideal place to put a plus atom
    // Returns -1 if no smart place could be determined
    int smartPlusInsert();
    // Returns the ideal place to put a normal atom
    // Returns -1 if no smart place could be determined
    int smartAtomInsert();
    // Generates a random insert place
    // Returns a random number between 0 and atomsLength+1
    int randomInsert();
// Helper functions

// Private variables
    // Points to the connected engine
    AtomasEngine *engine;
    // Stores the atoms.length for each solving iteration
    int atomsLength;
};

#endif // ATOMASSOLVER_H
