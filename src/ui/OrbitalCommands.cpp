#include "OrbitalCommands.h"

void IncreaseN::execute()         { m_sim.changeN(+1); }
void DecreaseN::execute()         { m_sim.changeN(-1); }
void IncreaseL::execute()         { m_sim.changeL(+1); }
void DecreaseL::execute()         { m_sim.changeL(-1); }
void IncreaseM::execute()         { m_sim.changeM(+1); }
void DecreaseM::execute()         { m_sim.changeM(-1); }
void IncreaseParticles::execute() { m_sim.adjustParticleCount(+10000); }
void DecreaseParticles::execute() { m_sim.adjustParticleCount(-10000); }
