// Atomas-Simulation-1 Levin Siegert 14.11.2019
// This is a program to simulate and develop the best algorithm to play Atomas, a game by Sirnic

#include <QCoreApplication>
#include <QList>
#include <QDebug>
#include <QRandomGenerator>

#include "main.h"
#include "atomasengine.h"
#include "atomassolver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Statistic variables
    int averageIterations=0, maximumIterations=0;

    // Execute multiple games
    for(int gamenumber=0; gamenumber < GAMES; gamenumber++)
    {
        // Atomas engine
        AtomasEngine engine;
        // Solver instance
        AtomasSolver solver(&engine);

        int iteration = 0;
        // Iterate through each step
        while(!engine.iterate(solver.solve())) iteration++;

        // Output number of iterations done
        if(DEBUG_LEVEL > 0) qDebug("Iterations: %d", iteration);
        averageIterations += iteration;
        if(iteration > maximumIterations) maximumIterations = iteration;
    }
    qDebug("Average iterations of %d games: %.2f", GAMES, static_cast<double>(averageIterations)/static_cast<double>(GAMES));
    qDebug("Maximum iterations reached: %d", maximumIterations);
    return a.exec();
}
