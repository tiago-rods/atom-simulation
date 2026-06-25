# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Projeto

Visualizador 3D de orbitais atômicos do hidrogênio usando C++ e OpenGL. Inspirado em https://github.com/kavan010/Atoms. Projeto pessoal com duplo objetivo: aprender C++ moderno + OpenGL e aprofundar física quântica.

**O que é:** visualização das soluções exatas da equação de Schrödinger independente do tempo para o átomo de hidrogênio, parametrizadas pelos números quânticos (n, l, m), renderizadas como nuvem de partículas amostradas de |ψ|².

**O que não é:** simulação temporal (sem Crank-Nicolson, sem grade de função de onda).

---

## Stack

| Ferramenta | Papel |
|---|---|
| CMake ≥ 3.20 | Build system |
| GLFW 3 | Janela + contexto OpenGL + input |
| GLAD | Loader de funções OpenGL 3.3+ |
| GLM | Matemática (matrizes, vetores, perspectiva) |
| GoogleTest (Sprint 2+) | Testes unitários do core físico |

Sem Eigen — matemática das funções especiais é implementada no próprio `SpecialFunctions.cpp`.

### Comandos de build

```bash
# Configurar (baixa dependências na primeira vez — pode demorar)
cmake --preset default

# Compilar
cmake --build --preset default

# Rodar
./build/atom-simulation.exe

# Rodar testes (Sprint 2+)
cmake --build --preset default --target atom_tests
./build/tests/atom_tests.exe
```

### Ambiente (Windows)

| Ferramenta | Caminho |
|---|---|
| Compilador C/C++ | `C:/msys64/ucrt64/bin/gcc.exe` / `g++.exe` |
| Ninja | `C:/Users/talve/AppData/Local/Microsoft/WinGet/Packages/Ninja-build.Ninja_Microsoft.Winget.Source_8wekyb3d8bbwe/ninja.exe` |
| vcpkg | `C:/vcpkg` (não usado ativamente — dependências via FetchContent) |

O preset já configura tudo isso automaticamente. **Não use** `cmake -B build` direto — faltará o PATH do compilador.

Dependências baixadas automaticamente pelo CMake (FetchContent) em `build/_deps/`:
- GLFW 3.4, GLM 1.0.1, GLAD v2.0.8 (OpenGL 3.3 Core)

---

## Física

Solução analítica exata em coordenadas esféricas:

```
ψ_{n,l,m}(r,θ,φ) = R_{n,l}(r) · Y_l^m(θ,φ)
```

- **Parte radial** — polinômios de Laguerre associados: `R_{n,l}(r) = norm · e^{-ρ/2} · ρ^l · L_{n-l-1}^{2l+1}(ρ)` onde `ρ = 2r/(n·a₀)`
- **Parte angular** — harmônicos esféricos: `Y_l^m(θ,φ) = norm · P_l^m(cosθ) · e^{imφ}`
- **Densidade de probabilidade:** `|ψ|²(r,θ,φ) = |R_{n,l}|² · |Y_l^m|²`
- **Energia:** `E_n = -13.6 eV / n²`

### Método numérico — Amostragem

Partículas posicionadas conforme |ψ|² — sem discretização de grade:

- **Rejection Sampling** (Sprint 2): gera (r,θ,φ) aleatórios e aceita com probabilidade proporcional a |ψ|²
- **CDF Sampling** (Sprint 4): pré-computa CDF radial e angular separadamente para maior eficiência

---

## Arquitetura e Estrutura de Pastas

```
atom-simulation/
├── CMakeLists.txt
├── extern/                              # GLFW, GLAD, GLM via FetchContent
├── shaders/
│   ├── particle.vert                    # Posição 3D → clip space
│   └── particle.frag                    # Colormap por probabilidade + Phong
├── tests/
│   ├── math/SpecialFunctionsTest.cpp
│   └── physics/OrbitalTest.cpp
└── src/
    ├── main.cpp
    ├── app/Application.{h,cpp}          # Loop principal, init GLFW/GL, event dispatch
    ├── core/
    │   ├── math/
    │   │   ├── SpecialFunctions.{h,cpp} # Laguerre associado, Legendre associado, Y_l^m
    │   │   └── SphericalCoords.{h,cpp}  # (r,θ,φ) ↔ (x,y,z)
    │   ├── physics/
    │   │   ├── QuantumNumbers.h          # Struct {n,l,m} + validação (l<n, |m|≤l)
    │   │   ├── HydrogenOrbital.{h,cpp}  # Calcula ψ e |ψ|² dado (n,l,m,r,θ,φ)
    │   │   └── ISampler.h               # Interface Strategy: sample(orbital, N) → vector<glm::vec3>
    │   └── sampling/
    │       ├── RejectionSampler.{h,cpp} # Implementa ISampler — rejection sampling
    │       └── CDFSampler.{h,cpp}       # Implementa ISampler — CDF inversa (Sprint 4)
    ├── rendering/
    │   ├── Shader.{h,cpp}               # Compila/linka GLSL, uniforms
    │   ├── ParticleBuffer.{h,cpp}       # VBO para N posições + probabilidades
    │   ├── Camera.{h,cpp}               # Câmera orbital (drag = rotaciona, scroll = zoom)
    │   └── OrbitalRenderer.{h,cpp}      # Facade OpenGL; implementa ISimulationObserver
    ├── simulation/
    │   ├── ISimulationObserver.h        # onParticlesUpdated(vector<glm::vec3>&)
    │   └── OrbitalSimulation.{h,cpp}    # Subject: chama ISampler, notifica observers
    ├── ui/
    │   ├── ICommand.h                   # execute() puro
    │   └── OrbitalCommands.{h,cpp}      # IncreaseN, DecreaseL, ChangeM, ResetCamera...
    └── factory/
        ├── IOrbitalFactory.h
        └── OrbitalFactory.{h,cpp}       # createOrbital(n,l,m) → HydrogenOrbital + ISampler
```

### SOLID aplicado

- **S** — `SpecialFunctions` só faz matemática; `HydrogenOrbital` só calcula ψ; `OrbitalRenderer` só renderiza
- **O** — Novo sampler: implementa `ISampler`. Novo tipo de orbital: nova classe, sem tocar `OrbitalSimulation`
- **L** — `CDFSampler` e `RejectionSampler` são substituíveis entre si via `ISampler`
- **I** — `ISimulationObserver` tem apenas `onParticlesUpdated()`; `ICommand` tem apenas `execute()`
- **D** — `OrbitalSimulation` recebe `ISampler` e `HydrogenOrbital` por injeção de dependência no construtor

---

## Design Patterns

| Pattern | Onde | Por quê |
|---|---|---|
| **Strategy** | `ISampler` | Trocar rejection ↔ CDF sampling sem alterar `OrbitalSimulation` |
| **Observer** | `ISimulationObserver` | `OrbitalSimulation` notifica `OrbitalRenderer` ao gerar novas partículas |
| **Factory Method** | `OrbitalFactory` | Encapsula criação de orbital + sampler por (n,l,m) |
| **Command** | `ICommand` / `OrbitalCommands` | Encapsula teclas do teclado; permite futura fila ou undo |
| **Facade** | `OrbitalRenderer` | Ponto único de contato com VAO/VBO/shader/camera |

---

## Controles (Sprint 3+)

| Tecla | Ação |
|---|---|
| W / S | n +1 / -1 |
| E / D | l +1 / -1 |
| R / F | m +1 / -1 |
| T / G | partículas +10k / -10k |
| Espaço | pausar/retomar reamostram. |
| Mouse drag | orbitar câmera |
| Scroll | zoom |

---

## Estado atual

Sprint 1 e Sprint 2 concluídos. A janela abre, renderiza 10k pontos com câmera orbitável, e o core matemático/físico está implementado. O próximo passo é o Sprint 3: conectar `OrbitalRenderer` como observer e implementar os comandos de teclado.

### O que está implementado e funcional
- `Application` — loop GLFW, GLAD inicializado, resize callback, ESC fecha
- `Shader` — lê arquivo GLSL, compila, linka, uniforms mat4/vec3/float
- `Camera` — órbita com drag, zoom com scroll, conectada ao input
- `ParticleBuffer` — VAO/VBO com upload e `GL_POINTS`
- `SphericalCoords` — conversão (r,θ,φ) ↔ (x,y,z)
- `QuantumNumbers` — struct com validação de (n,l,m)
- `SpecialFunctions` — `assocLaguerre`, `assocLegendre`, `sphericalHarmonic` implementados
- `HydrogenOrbital` — `psi(r,θ,φ)` e `psiSquared(r,θ,φ)` implementados (verificado matematicamente para 1s)
- `RejectionSampler` — `sample()` implementado com estimativa de M e jacobiano r²sin(θ)
- `OrbitalSimulation` — padrão Observer completo
- `OrbitalFactory` — cria `HydrogenOrbital` + `RejectionSampler`
- Shaders `particle.vert/.frag` — básicos para Sprint 1

### O que é stub (retorna zero / vazio)
- `CDFSampler::sample()` — implementar no Sprint 4
- `OrbitalCommands::execute()` — implementar no Sprint 3
- `OrbitalRenderer::draw()` — ainda não conectado ao OrbitalSimulation (Sprint 3)

### Notas de implementação
- `SpecialFunctions.cpp` tem uma função `semiFatorial` local (static) que calcula n! regular (nome enganoso, mas usado consistentemente — não alterar sem revisar todos os usos)
- `RejectionSampler` usa `rMax = 3.0 * n²` em unidades de Bohr; a estimativa de M amostra 5000 pontos com margem de 10%
- Bohr units: a₀ = 1.0 em todo o código de física

---

## Sprints (~4-5 semanas)

### Sprint 1 — Fundação 3D (Semana 1) ✅
**Objetivo:** Janela OpenGL 3D com câmera orbitável e pontos renderizados.

- [x] `CMakeLists.txt` com GLFW, GLAD, GLM
- [x] Estrutura de pastas completa
- [x] `Application`: loop, GLFW init, resize callback
- [x] `Shader`: compilar `particle.vert/.frag`; uniform MVP matrix
- [x] `ParticleBuffer`: VBO de N posições
- [x] `Camera`: orbit com drag de mouse, zoom com scroll
- [x] GLAD inicializado, Camera conectada ao mouse/scroll, 10k pontos brancos renderizados

### Sprint 2 — Core Matemático + Física (Semana 2) ✅
**Objetivo:** Calcular |ψ|² para qualquer (n,l,m) e amostrar pontos no espaço 3D.

- [x] `SphericalCoords`: conversão (r,θ,φ) ↔ (x,y,z)
- [x] `SpecialFunctions`: Laguerre associado `L_n^α(x)`, Legendre associado `P_l^m(x)`, harmônicos esféricos reais
- [x] `HydrogenOrbital`: `psi(r,θ,φ)` e `psiSquared(r,θ,φ)`
- [x] `RejectionSampler`: implementa `ISampler`
- [ ] `OrbitalSimulation`: chama sampler, notifica observers (estrutura pronta, integração no Sprint 3)
- [ ] Testes: normalização ∫|ψ|²dV ≈ 1 para orbital 1s

### Sprint 3 — Visualização de Orbitais (Semana 3)
**Objetivo:** Ver nuvens de probabilidade coloridas em 3D em tempo real.

- [ ] `OrbitalRenderer` como `ISimulationObserver`: upload de partículas para VBO
- [ ] Fragment shader com colormap (azul=baixa → vermelho=alta probabilidade)
- [ ] Phong lighting básico
- [ ] `OrbitalCommands` + mapeamento de teclas em `Application`
- [ ] `OrbitalFactory`: criar orbitais 1s, 2p, 3d interativamente
- [ ] Overlay de texto com (n, l, m) atual

### Sprint 4 — Polish + CDF Sampling (Semana 4+)
**Objetivo:** Performance e qualidade visual para 50k–250k partículas.

- [ ] `CDFSampler`: pré-computa CDF radial + angular (muito mais eficiente que rejection)
- [ ] Animação suave: regen partículas por frames ou background thread
- [ ] Filtro de corte (exibir só z < 0 para ver interior do orbital)
- [ ] Atualizar `README.md` com instruções de build e controles

---

## Módulos de Estudo

| # | Tema | Paralelo ao | Recursos |
|---|---|---|---|
| M1 | C++ Moderno (herança, smart pointers, STL, move semantics) | Sprint 1 | learncpp.com seções 14-20, cppreference.com |
| M2 | Design Patterns & SOLID (Strategy, Observer, Factory, Command, Facade) | Sprint 1-2 | Refactoring.Guru |
| M3 | OpenGL 3D (VAO/VBO, GLSL, câmera, Phong, GL_POINTS) | Sprint 1-3 | learnopengl.com — Getting Started, Lighting, Advanced OpenGL |
| M4 | Física do Hidrogênio (n/l/m, harmônicos esféricos, Laguerre, interpretação de Born) | Sprint 2 | Griffiths — Intro to QM caps. 4.1-4.3 |
| M5 | Amostragem Probabilística (rejection sampling, CDF inversa, coords. esféricas) | Sprint 2-4 | Wikipedia: "Rejection sampling", "Inverse transform sampling" |

---

## Contexto do usuário

- Iniciante em C++ e OpenGL — prefere explicações dos conceitos junto com o código
- Projeto pessoal de aprendizado: C++ moderno + OpenGL + física quântica em paralelo
