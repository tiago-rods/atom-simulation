# Atom Simulation

Visualizador 3D interativo dos orbitais atômicos do hidrogênio, escrito em C++ moderno com OpenGL. Os pontos são amostrados diretamente de $|\psi|^2$, produzindo a nuvem de probabilidade do elétron para qualquer combinação de números quânticos $(n, l, m)$.

Inspirado em [kavan010/Atoms](https://github.com/kavan010/Atoms). Projeto pessoal de aprendizado: C++ moderno + OpenGL + física quântica em paralelo.

---

## Sumário

1. [Como compilar e rodar](#como-compilar-e-rodar)
2. [Física: o átomo de hidrogênio](#física-o-átomo-de-hidrogênio)
3. [Matemática: as funções especiais](#matemática-as-funções-especiais)
4. [Método numérico: Rejection Sampling](#método-numérico-rejection-sampling)
5. [OpenGL: como a cena é renderizada](#opengl-como-a-cena-é-renderizada)
6. [Estrutura do projeto](#estrutura-do-projeto)
7. [Controles](#controles)
8. [Dependências](#dependências)

---

## Como compilar e rodar

### Pré-requisitos

- CMake ≥ 3.20
- GCC/G++ (MinGW-w64 / MSYS2 ucrt64 no Windows)
- Ninja
- Conexão com internet na primeira compilação (dependências baixadas automaticamente)

### Comandos

```bash
# 1. Configurar o projeto (baixa GLFW, GLAD e GLM automaticamente)
cmake --preset default

# 2. Compilar
cmake --build --preset default

# 3. Rodar
./build/atom-simulation.exe

# 4. Rodar testes unitários (Sprint 2+)
cmake --build --preset default --target atom_tests
./build/tests/atom_tests.exe
```

> **Atenção (Windows):** use sempre `cmake --preset default`. Não use `cmake -B build` diretamente — o preset configura os caminhos do compilador e do Ninja automaticamente.

---

## Física: o átomo de hidrogênio

### A equação de Schrödinger

O átomo de hidrogênio é o único sistema quântico com solução analítica exata. A equação de Schrödinger independente do tempo:

$$\hat{H} \psi = E \psi$$

em coordenadas esféricas $(r, \theta, \phi)$ se separa em duas partes independentes:

$$\psi_{n,l,m}(r,\, \theta,\, \phi) = R_{n,l}(r) \cdot Y_l^m(\theta, \phi)$$

### Números quânticos

| Símbolo | Nome | Restrição | Significado físico |
|---|---|---|---|
| $n$ | Principal | $n \geq 1$ | Nível de energia |
| $l$ | Angular | $0 \leq l < n$ | Momento angular orbital |
| $m$ | Magnético | $-l \leq m \leq l$ | Componente z do momento angular |

Exemplos de orbitais:
- **1s** → $n=1,\ l=0,\ m=0$ (esférico)
- **2p** → $n=2,\ l=1,\ m \in \{-1, 0, 1\}$ (lobos)
- **3d** → $n=3,\ l=2,\ m \in \{-2, \ldots, 2\}$ (formas complexas)

### Energia

$$E_n = \frac{-13.6\ \text{eV}}{n^2}$$

Apenas o número quântico principal determina a energia no átomo de hidrogênio.

### Densidade de probabilidade

A interpretação de Born diz que $|\psi|^2$ é a **densidade de probabilidade** de encontrar o elétron em um ponto do espaço. Os pontos renderizados são amostrados dessa distribuição — regiões com mais pontos são locais onde há mais probabilidade do elétron ser encontrado no colapso da função de onda.

---

## Matemática: as funções especiais

### Parte radial — Polinômios de Laguerre Associados

A função radial $R_{n,l}(r)$ em unidades de Bohr ($a_0 = 1$):

$$R_{n,l}(r) = N \cdot e^{-\rho/2} \cdot \rho^{\,l} \cdot L_{n-l-1}^{2l+1}(\rho)$$

onde $\rho = 2r/n$ e $N$ é a normalização:

$$N = \sqrt{\left(\frac{2}{n}\right)^3 \cdot \frac{(n-l-1)!}{2n\,(n+l)!}}$$

Os **polinômios de Laguerre associados** $L_n^\alpha(x)$ são calculados pela recorrência de três termos:

$$L_0^\alpha(x) = 1 \qquad L_1^\alpha(x) = 1 + \alpha - x$$

$$L_{k+1}^\alpha(x) = \frac{(2k + 1 + \alpha - x)\cdot L_k^\alpha - (k + \alpha)\cdot L_{k-1}^\alpha}{k + 1}$$

Isso é numericamente estável e evita calcular fatoriais grandes.

### Parte angular — Harmônicos Esféricos Reais

Os **harmônicos esféricos** $Y_l^m(\theta, \phi)$ são as autofunções do operador de momento angular:

$$Y_l^m(\theta, \phi) = N_{lm} \cdot P_l^{|m|}(\cos\theta) \cdot \begin{cases} \cos(|m|\phi) & \text{se } m > 0 \\ 1 & \text{se } m = 0 \\ \sin(|m|\phi) & \text{se } m < 0 \end{cases}$$

com normalização:

$$N_{lm} = \sqrt{\frac{2l+1}{4\pi} \cdot \frac{(l-|m|)!}{(l+|m|)!}} \cdot \begin{cases} \sqrt{2} & \text{se } m \neq 0 \\ 1 & \text{se } m = 0 \end{cases}$$

Os **polinômios de Legendre associados** $P_l^m(x)$ são calculados em duas etapas:

1. **Diagonal:** $P_m^m$ via produto acumulado $\displaystyle\prod_{k=0}^{m-1} -(2k+1)\sqrt{1-x^2}$
2. **Vertical:** sobe de $P_m^m$ até $P_l^m$ pela recorrência:

$$P_l^m(x) = \frac{(2l-1)\cdot x \cdot P_{l-1}^m - (l+m-1)\cdot P_{l-2}^m}{l - m}$$

---

## Método numérico: Rejection Sampling

Não é possível sortear $(r, \theta, \phi)$ diretamente da distribuição $|\psi|^2$ — ela não tem inversa fechada. A solução é o **rejection sampling**:

1. Sorteia $(r, \theta, \phi)$ uniformemente no domínio
2. Calcula $f = |\psi|^2(r,\theta,\phi) \cdot r^2 \cdot \sin\theta$ — inclui o jacobiano
3. Sorteia $u \sim \mathrm{Uniform}(0,\, M)$
4. Se $u < f$ → aceita o ponto; se $u \geq f$ → descarta e repete

O fator $r^2 \sin\theta$ é o **jacobiano** da transformação esférica → cartesiana. Sem ele, o volume de cada casca esférica não seria levado em conta, e a nuvem ficaria incorretamente concentrada na origem.

**Estimativa de $M$** (o teto da distribuição): amostram-se ~5000 pontos aleatórios, guarda-se o maior $f$ encontrado e aplica-se uma margem de 10%. $M$ precisa ser $\geq \max(f)$ para garantir que nenhum ponto válido seja rejeitado.

**Domínio de busca:** $r \in [0,\ 3n^2]$ em unidades de Bohr, o que cobre ~99% da probabilidade de qualquer orbital $n$.

---

## OpenGL: como a cena é renderizada

### Pipeline resumido

```
CPU (C++)          →     GPU (GLSL)
posições vec3      →     vertex shader  →  gl_Position (clip space)
                   →     fragment shader →  cor do pixel
```

### VAO e VBO

- **VBO (Vertex Buffer Object):** bloco de memória na GPU que armazena as posições $(x, y, z)$ de cada partícula.
- **VAO (Vertex Array Object):** descreve *como* interpretar o VBO (quantos floats por vértice, quais atributos existem). Sem o VAO, o OpenGL não sabe ler o VBO.

Cada frame, as posições são enviadas para o VBO com `glBufferData` e desenhadas com `glDrawArrays(GL_POINTS, ...)`.

### Shaders GLSL

**Vertex shader (`particle.vert`):** recebe a posição 3D e a multiplica pela matriz MVP para obter a posição na tela.

```glsl
gl_Position = MVP * vec4(position, 1.0);
```

**Fragment shader (`particle.frag`):** define a cor de cada fragmento (pixel). No Sprint 3 usará um colormap baseado na probabilidade; por ora é branco.

### Matriz MVP

Transformação padrão 3D em 3 etapas:

| Matriz | O que faz |
|---|---|
| **M** (Model) | Posiciona o objeto no mundo |
| **V** (View) | Move a câmera (transforma mundo → espaço da câmera) |
| **P** (Projection) | Aplica perspectiva (objetos distantes ficam menores) |

### Câmera Orbital

A câmera orbita em torno da origem. O usuário arrasta o mouse para rotacionar e usa o scroll para zoom. Internamente, a câmera mantém um ângulo polar e azimutal convertidos para posição esférica a cada frame:

$$\text{eye} = \bigl(r\sin\theta\cos\phi,\; r\cos\theta,\; r\sin\theta\sin\phi\bigr)$$

### GL_POINTS

Cada partícula é um ponto primitivo do OpenGL. `glPointSize(n)` controla o tamanho em pixels. É a primitiva mais simples e eficiente para nuvens de partículas.

---

## Estrutura do projeto

```
atom-simulation/
├── CMakeLists.txt
├── shaders/
│   ├── particle.vert          # Vertex shader: posição 3D → clip space
│   └── particle.frag          # Fragment shader: colormap por probabilidade
├── tests/
│   ├── math/SpecialFunctionsTest.cpp
│   └── physics/OrbitalTest.cpp
└── src/
    ├── main.cpp
    ├── app/
    │   └── Application.{h,cpp}        # Loop principal, GLFW, event dispatch
    ├── core/
    │   ├── math/
    │   │   ├── SpecialFunctions.{h,cpp}  # Laguerre, Legendre, Y_l^m
    │   │   └── SphericalCoords.{h,cpp}   # (r,θ,φ) ↔ (x,y,z)
    │   ├── physics/
    │   │   ├── QuantumNumbers.h           # Struct {n,l,m} + validação
    │   │   ├── HydrogenOrbital.{h,cpp}   # Calcula ψ e |ψ|²
    │   │   └── ISampler.h                # Interface Strategy para samplers
    │   └── sampling/
    │       ├── RejectionSampler.{h,cpp}  # Rejection sampling de |ψ|²
    │       └── CDFSampler.{h,cpp}        # CDF inversa (Sprint 4)
    ├── rendering/
    │   ├── Shader.{h,cpp}               # Compila/linka GLSL, uniforms
    │   ├── ParticleBuffer.{h,cpp}       # VAO/VBO para N partículas
    │   ├── Camera.{h,cpp}               # Câmera orbital (drag + scroll)
    │   └── OrbitalRenderer.{h,cpp}      # Facade OpenGL + ISimulationObserver
    ├── simulation/
    │   ├── ISimulationObserver.h        # Interface Observer
    │   └── OrbitalSimulation.{h,cpp}   # Subject: chama sampler, notifica observers
    ├── ui/
    │   ├── ICommand.h                   # Interface Command
    │   └── OrbitalCommands.{h,cpp}     # Comandos de teclado
    └── factory/
        ├── IOrbitalFactory.h
        └── OrbitalFactory.{h,cpp}      # Cria HydrogenOrbital + ISampler
```

### Design Patterns utilizados

| Pattern | Onde | Por quê |
|---|---|---|
| **Strategy** | `ISampler` | Troca RejectionSampler ↔ CDFSampler sem alterar `OrbitalSimulation` |
| **Observer** | `ISimulationObserver` | `OrbitalSimulation` notifica `OrbitalRenderer` ao gerar novas partículas |
| **Factory Method** | `OrbitalFactory` | Encapsula criação de orbital + sampler por (n,l,m) |
| **Command** | `ICommand` | Encapsula teclas do teclado; permite futura fila de undo |
| **Facade** | `OrbitalRenderer` | Ponto único de contato com VAO/VBO/shader/câmera |

---

## Controles

| Tecla / Input | Ação |
|---|---|
| `W` / `S` | $n\ +1\ /\ -1$ |
| `E` / `D` | $l\ +1\ /\ -1$ |
| `R` / `F` | $m\ +1\ /\ -1$ |
| `T` / `G` | partículas +10k / -10k |
| `Espaço` | pausar/retomar reamostragem |
| Mouse drag | orbitar câmera |
| Scroll | zoom |
| `ESC` | fechar |

> Controles de teclado disponíveis a partir do Sprint 3.

---

## Dependências

Todas baixadas automaticamente pelo CMake (FetchContent) — não é necessário instalar nada manualmente.

| Biblioteca | Versão | Papel |
|---|---|---|
| [GLFW](https://www.glfw.org/) | 3.4 | Janela + contexto OpenGL + input |
| [GLAD](https://github.com/Dav1dde/glad) | v2.0.8 (GL 3.3 Core) | Loader de funções OpenGL |
| [GLM](https://github.com/g-truc/glm) | 1.0.1 | Matemática: vetores, matrizes, perspectiva |
| [GoogleTest](https://github.com/google/googletest) | latest | Testes unitários do core físico |
