#pragma once
#include "ICommand.h"
#include "simulation/OrbitalSimulation.h"

class IncreaseN : public ICommand {
public:
    explicit IncreaseN(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class DecreaseN : public ICommand {
public:
    explicit DecreaseN(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};
