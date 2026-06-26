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

class IncreaseL : public ICommand {
public:
    explicit IncreaseL(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class DecreaseL : public ICommand {
public:
    explicit DecreaseL(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class IncreaseM : public ICommand {
public:
    explicit IncreaseM(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class DecreaseM : public ICommand {
public:
    explicit DecreaseM(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class IncreaseParticles : public ICommand {
public:
    explicit IncreaseParticles(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};

class DecreaseParticles : public ICommand {
public:
    explicit DecreaseParticles(OrbitalSimulation& sim) : m_sim(sim) {}
    void execute() override;
private:
    OrbitalSimulation& m_sim;
};
