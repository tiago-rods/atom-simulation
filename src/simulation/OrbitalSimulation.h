  #pragma once
  #include <memory>
  #include <vector>
  #include "ISimulationObserver.h"
  #include "core/physics/HydrogenOrbital.h"
  #include "core/physics/ISampler.h"
  #include "factory/OrbitalFactory.h"

class OrbitalSimulation {
public:
    OrbitalSimulation(int n, int l, int m, int particleCount = 10000);
    void resample();
    void addObserver(ISimulationObserver* obs);

    void changeN(int delta); // W/S
    void changeL(int delta); //E/D
    void changeM(int delta); //R/F

    QuantumNumbers quantumNumbers() const { return m_orbital->qn(); }
    int particleCount() const { return m_count; }
private:
    void notify(const std::vector<glm::vec3>& positions, const std::vector<float>& probs);

    OrbitalFactory m_factory;
    std::unique_ptr<HydrogenOrbital> m_orbital;
    std::unique_ptr<ISampler> m_sampler;
    int m_count;
    std::vector<ISimulationObserver*> m_observers; //entender o motivo de ser um ponteiro e como funciona esse observer
    

    // OrbitalSimulation(std::unique_ptr<HydrogenOrbital> orbital,
    //                   std::unique_ptr<ISampler> sampler,
    //                   int particleCount = 10000); //quantidade de particulas

    // void resample();
    // void addObserver(ISimulationObserver* obs);
};
