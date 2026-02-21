# Matemática

Para a alegria de muitos, a computação gráfica é uma área na qual estamos constantemente aplicando conceitos matemáticos. Portanto, achamos que seria bom criar este capítulo para refrescar a sua memória em algumas áreas que serão importantes daqui para frente. Se você já se sente confortável e confiante com os conteúdos listados abaixo, pode pular este módulo sem problemas. Já se você não sabe absolutamente nada sobre nenhum dos tópicos, isto pode sim ser um gargalo na sua jornada - mas não desanime, iremos linkar bons recursos para você aprender bem tudo que será ensinado aqui.

1. Trigonometria 
2. Matrizes
3. Vetores
4. Interpolação linear e coordenadas baricêntricas

Nesta trilha não usaremos nenhum conceito avançado sobre estes tópicos, este capítulo é realmente um bônus para refrescar a memória de alguns.

## Trigonometria

A trigonometria é o estudo dos triângulos. Como já vimos lá no começo da trilha, a primitiva principal com a qual trabalhamos na computação gráfica **é** o triângulo, então vale a pena relembrar algumas coisas sobre eles.

### Ângulos

No geral, quando trabalharmos com ângulos na computação gráfica, estaremos representando eles em **_radianos_**. `360°` equivalem a `2π radianos`, então para converter um valor em graus para radianos, basta multiplicar este valor por `π/180`.

A soma dos ângulos internos de um triângulo é sempre igual a `180°`, ou seja, `π` radianos.

### Funções trigonométricas

![Triângulo retângulo](../imagens/04_triangret.png)

Em um triângulo retângulo (triângulo com um ângulo de `90°` ou `π/2 rad`), o lado oposto à este ângulo é chamado de **hipotenusa** (`h`), selecionando um dos outros ângulos como sendo `θ`, temos que o lado oposto a este ângulo se chama **cateto oposto** (`o`), e o lado que sobrou se chama **cateto adjacente** (`a`). Sendo assim, Pitágoras nos dá a seguinte relação entre o tamanho dos três lados:

```
         _\|/_
         (o o)
 +----oOO-{_}-OOo--------+
 |                       |
 | Teorema de pitágoras: |
 |     a² + o² = h²`     |
 |                       |
 +-----------------------+
```

As funções trigonométricas, por sua vez, são funções que relacionam o ângulo `θ` com a razão de dois lados de um triângulo retângulo:

- Seno: `sen(θ) = o/h`
- Cossecante: `cossec(θ) = h/o`
- Cosseno: `cos(θ) = a/h`
- Secante: `sec(θ) = h/a`
- Tangente: `tan(θ) = o/a`
- Cotangente: `cot(θ) = a/o`

Estas funções são periódicas, e possuem um período de `2π`. Além disso, as funções seno, cossecante, cotangente e tangente são ímpares, enquanto as funções cosseno e secante são pares, o que significa que:

- `tan(-θ) = - tan(θ)`
- `cot(-θ) = - cot(θ)`
- `sen(-θ) = - sen(θ)`
- `cossec(-θ) = - cossec(θ)`
- `cos(-θ) = cos(θ)`
- `sec(-θ) = sec(θ)`

### Identidades pitagóricas

Outra coisa que Pitágoras deixou como legado são as **identidades pitagóricas**:

1. `sen²(θ) + cos²(θ) = 1`
2. `sec²(θ) - tan²(θ) = 1`
3. `cossec²(θ) - cot²(θ) = 1`

### Leis dos senos, cossenos e cálculo da área

Agora, se tivermos um triângulo qualque com ângulos internos `a`, `b` e `c`, e lados `A`, `B` e `C`, sendo `A` o lado oposto ao ângulo `a` e assim em diante, temos as seguintes leis:

- Lei dos senos: `sen(A)/a = sen(B)/b = sen(C)/c`
- Lei dos cossenos: `c² = a² + b² - 2ab × cos(C)`

Por fim, temos também essa fórmula coringa capaz de calcular a área de um triângulo com base apenas no comprimento de seus lados:

```
Area = 1/4 × √((a + b + c)(-a + b + c)(a - b + c)(a + b - c))
```

E é só isso de trigonometria por aqui, se quiser um recurso para aprender melhor eu recomendo o grande e moral [Khan Academy](https://www.khanacademy.org/math/trigonometry).

## Matrizes

Informalmente, uma matriz é um conjunto de números organizado em linhas e colunas. Uma matriz **N** por **M** possui N linhas e M colunas. Por exemplo, uma matriz 3x2 tem essa cara:

![Primeiro exemplo de matriz](../imagens/04_matriz1.png)

### Operações com matrizes

Somar duas matrizes envolve somar cada um de seus componentes:

![Exemplo de soma](../imagens/04_soma.png)

Já a conta que tem que ser feita para multiplicar duas matrizes é um pouco mais feia. Primeiro que nem sempre é possível multiplicar duas matrizes, precisamos que o número de colunas na primeira seja igual ao número de linhas na segunda. Segundo que para calcular o elemento na i-ésima linha e j-ésima coluna da matriz resultante, iremos multiplicar todos os elementos da i-ésima linha da primeira matriz por todos os elementos da j-ésima coluna da segunda matriz, obtendo no final uma matriz com o número de linhas da primeira e o número de colunas da segunda. Isso tudo significa também que a multiplicação de matrizes **não é comutativa** - ou seja, `A × B` não necessáriamente é igual a `B × A` (e raramente será).

![Exemplo de multiplicação](../imagens/04_multiplica.png)

Multiplicar uma matriz com um escalar, por outro lado, é bem simples, basta multiplicar cada componente isoladamente por aquele escalar:

![Exemplo de multiplicação por escalar](../imagens/04_multescalar.png)

### Matrizes especiais

Algumas matrizes possuem características que podem ser interessantes em alguns casos, e portanto recebem nomes específicos.

**Matriz quadrada**: é uma matriz com o mesmo número de linhas e colunas, como uma 3x3 por exemplo.

![Exemplo de matriz 3x3](../imagens/04_matriz2.png)

**Matriz transposta**: a transposta de uma matriz A é a matriz que você obtém ao trocar as linhas de A pelas colunas (e vice-versa).

![Exemplo de transposição](../imagens/04_transposta.png)

**Matriz diagonal**: é uma matriz cuja diagonal principal (elementos cujo número da linha [`i`] é igual ao número da coluna [`j`]; `i = j`) possui apenas valores diferentes de zero, enquanto o resto da matriz é preenchida com zeros.

![Exemplo de Matriz diagonal](../imagens/04_matrizdiagonal.png)

- **Matriz identidade**: uma matriz identidade de grau N é uma matriz diagonal NxN cujos valores não nulos são todos 1.

![Exemplo de Matriz identidade](../imagens/04_identidade.png)

- **Matriz inversa**: a inversa de uma matriz A é a matriz que quando múltiplicada com A resulta na matriz identidade. Nem toda matriz possui uma inversa.

![Exemplo de Matriz inversa](../imagens/04_inversa.png)

Se quiser aprender mais sobre matrizes, eu recomendo as primeiras 70 e poucas páginas do livro [Álgebra Linear com Aplicações](https://archive.org/details/algebra-linear-com-aplica-10-edi-anton-rorres/mode/2up).

## Vetores

O conceito do que é um vetor é bastante abrangente e abstrato, então iremos tomar aqui uma visão mais intuitiva e geométrica do que é um vetor.

Para nós, um vetor de dimensão N é um conjunto de números que indica uma de duas coisas (na verdade indica as duas coisas simultâneamente, mas dependendo da situação uma interpretação será mais útil do que a outra):

- Uma posição em um espaço N-dimensional
- Um comprimento e uma direção

Um vetor tridimensional pode ser escrito assim: `<x, y, z>`.

Pensando em vetores como sendo a posição de um ponto, seus componentes indicam o "peso" de cada um dos eixos (vetores da base de um espaço) para a posição que ele representa. Um ponto representado pelo vetor `<2, 1, 0>`, por exemplo, está mais deslocado no eixo X do que no Y, e mais no Y do que no Z. Já o vetor `<0, 0, 0>` senta exatamente na origem do espaço tridimensional.

Agora, pensando em um vetor como um comprimento e uma direção, seus componentes indicam o quanto cada eixo influencia sua direção, e o quão longo é o vetor. Por exemplo, o vetor `<1, 0, 1>` aponta para uma direção que está entre as direções positivas dos eixos X e Y. Já o vetor `<2, 0, 2>` aponta na mesma direção, mas possui um comprimento maior.

### Operações com vetores

Somar dois vetores é igual somar duas matrizes, é só ir componente por componente:

```
<x, y, z> + <a, b, c> = <(x+a), (y+b), (z+c)>
```

![Soma de vetores](../imagens/04_somavet.png)

Multiplicar um vetor por um escalar também é igual...

``` 
2 × <x, y, z> = <2x, 2y, 2z>
```

![Multiplicação por escalar](../imagens/04_escalarvet.png)

Temos também uma operação especial chamada **norma** (ou módulo), que se consiste em pegar o comprimento do vetor. A norma de um vetor **u** é escrita desta forma: **|u|**. Para descobrir essa norma nós basicamente fazemos uma versão genérica do teorema de pitágoras:

``` 
|<x, y, z>| = √(x² + y² + z²)
```

![Norma vetor](../imagens/04_norma.png)

Agora, existem dois tipos de produto entre vetores:

**Produto escalar**: soma-se os produtos dos componentes equivalentes nos dois vetores. O resultado final é um escalar. Para representar o produto escalar entre vetores **u** e **v**, usamos o símbolo **⋅**, ou seja: `u ⋅ v`.

``` 
<x, y, z> ⋅ <a, b, c> = xa + yb + zc
```

Existe outra forma de se calcular o produto escalar de dois vetores, para isso utilizamos o cosseno do ângulo entre eles e seus módulos:

```
u ⋅ v = |u||y| × cos(θ)
```

- **Produto vetorial**: resolve-se o determinante de uma matriz cuja primeira linha são os vetores da base do espaço  (`i`, `j` e `k`), a segunda linha é o primeiro vetor e a terceira linha é o segundo vetor. O resultado é um vetor ortogonal aos dois que estão sendo multiplicados (veremos a definição de vetor ortogonal daqui a pouco). Como não te ensinamos a calcular o determinante de uma matriz, você também pode usar a fórmula abaixo, que é a mesma coisa. Para representar o produto vetorial, usamos o símbolo `×` mesmo, então `u × v`.

```
                        │i  j  k│
<x, y, z> × <a, b, c> = │x  y  z│
                        │a  b  c│

= <(yc - zb), (za - xc), (xb - ya)>
```

![Produto vetorial](../imagens/04_prodvet.png)

### Conceitos

Ao falarmos de vetores frequentemente surgem alguns termos que no nosso cotidiano possuem um sentido mas na álgebra linear possuem outro. Ao mesmo tempo, existem alguns outros conceitos que são mais exclusivos da matemática mesmo, então bora ver alguns deles:

- **Normal**: um vetor `u` é dito normal quando `|u| = 1`. Geralmente vetores normais são escritos com um chapéuzinho circunflexo, tipo assim: `û`.
- **Espaço vetorial**: um espaço vetorial é um conjunto de vetores que segue algumas propriedades. Para nós essas propriedades não vão importar muito, mas algo que é relevante sim para nós é saber que espaços vetoriais possuem _dimensão_ e _base_.
- **Base**: uma base de um espaço vetorial `V` é um conjunto mínimo de vetores capaz de gerar `V`. A base canônica do espaço tridimensional é `{<1, 0, 0>, <0, 1, 0>, <0, 0, 1>}`.
- **Dimensão**: a dimensão de um espaço vetorial `V` é o número de vetores em sua base.
- **Ortogonalidade**: dois vetores são ditos ortogonais se o produto escalar entre eles é igual a **zero**. Geometricamente isso nos diz que eles estão a um ângulo de `90°` um do outro, o que pode ser facilmente verificado se nós observarmos a fórmula `u ⋅ v = |u||y| × cos(θ)` e lembrarmos que o cosseno de `θ` vale zero quando `θ = π/2`.
- **Base ortonormal**: é uma base formada apenas por vetores normais e ortogonais entre si.

> Para normalizar um vetor, basta dividir ele pelo seu módulo; assim, sua direção será mantida, mas seu comprimento se tornará 1.

Por fim, se você quiser aprender mais formalmente sobre vetores, espaços vetoriais e algebra linear no geral, esses recursos abaixo são altamente recomendados:

- A playlist "[Excência da Algebra Linear](https://www.youtube.com/watch?v=fNk_zzaMoSs&list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)", do 3Blue1Brown.
- O livro já citado anteriormente, [Álgebra Linear com Aplicações](https://archive.org/details/algebra-linear-com-aplica-10-edi-anton-rorres/mode/2up), do Howard Anton e do Chris Rorres.
- O livro [Álgebra Linear e Aplicações](https://sites.icmc.usp.br/frasson/aled/material/Algebra%20Linear%20e%20Aplicacoes%206ed%20-%20Callioli,%20Domingues,%20Costa.pdf), do Callioli.
- O livro [Álgebra Linear](https://archive.org/details/algebralinearboldrini_201908/mode/2up), do Boldrini.

## Interpolação linear e coordenadas baricêntricas

![Interpolação Linear](../imagens/04_interpolacao.png)

Uma interpolação linear entre os pontos/vetores `A` e `B` é basicamente uma função com um parâmetro `t` que passa em A quando `t = 0` e passa em B quando `t = 1`, sendo os coeficientes de A e B polinômios lineares de `t`. Uma interpolação linear (também conhecida como **_lerp_**) possui essa cara: `f(t) = (1 - t)A + tB`.

No caso de pontos bidimensionais, também podemos pensar na fórmula de uma interpolação linear entre `A = <x0, y0>` e `B = <x1, y1>` como sendo:

```
y = y0 + (x - x0) × (y1 - y0)/(x1 - x0)
```

Nesse caso, você pode pensar em em `(x - x0)` como assumindo o papel de `t` e em `(y1 - y0)/(x1 - x0)` como sendo a declividade entre os pontos A e B. No geral, pense que quando `t` vai crescendo, A vai perdendo sua influência no valor resultante, enquanto B vai proporcionalmente ganhando influência. Relativamente simples, né?

Coordenadas baricêntricas são um pouco mais confusas.

Nós vimos em capítulos anteriores que atributos como cores são geralmente passados para o vertex shader como sendo atributos dos vértices de triângulos. Quando nossos triângulos passam pelo rasterizador, eles são transformados em um monte de fragmentos, e vimos que os atributos de cada fragmento são interpolações lineares dos atributos dos vértices do triângulo ao qual ele pertence. Contudo, fazer essa interpolação entre 3 valores não é tão simples quanto fazer a interpolação entre apenas dois :(. Por sorte, 200 anos atrás, um maluco chamado Möbius (sim, o da [fita de Möbius](https://pt.wikipedia.org/wiki/Fita_de_M%C3%B6bius)) inventou as coordenadas baricêntricas.

![Coordenadas baricêntricas](../imagens/04_baricentro.png)

Elas funcionam assim, pense que os vértices do seu triângulo têm os nomes muito criativos `a`, `b` e `c`. Nós definimos `a` como sendo a origem do nosso sistema de coordenadas, e os vetores que vão de `a` até `c` e `a` até `b` como sendo os vetores da base (ou seja, a base é `{c - a, b - a}`). Assim, qualquer ponto `P` que esteja no mesmo plano que o triângulo criativo poderá ser escrito na seguinte forma:

```
P = a + β(b - a) + γ(c - a)
```

Contudo, essa fórmula é meio feia de se trabalhar, então nós podemos rearranjar os termos para chegar no seguinte:

``` 
P =  (1 - β - γ)a + βb + γc
```

E então introduzir um novo coeficiente `α = 1 - β - γ` para deixar o negócio ainda mais limpo

``` 
P = αa + βb + γc
```

Logo, para calcular uma posição com coordenadas baricêntricas, podemos usar essa função:

```
P(α, β, γ) = αa + βb + γc
```

Com a restrição de que `α + β + γ = 1`.

Um efeito imediatamente legal disso é que um ponto só está dentro do triângulo `abc` se suas coordenadas baricêntricas estiverem entre 0 e 1, ou seja, `0 < α, β, γ < 1`. Se uma das coordenadas for igual a zero, então o ponto está na aresta oposta ao vértice associado àquele coeficiente. Se duas coordenadas forem zero, então a última é 1, o que significa que o ponto está exatamente naquele vértice.

Mas como isso resolve o problema da interpolação exatamente? Bom, nós temos agora 3 coeficientes que, dentro do triângulo, variam entre 0 e 1. Ou seja, as coordenadas baricêntricas **são os pesos** que podemos usar para ponderar/interpolar os atributos dos vértices no interior do triângulo. Assim como no caso mais simples nós tinhamos o `t`, aqui temos `α`, `β` e `γ`. Isso é lindo e emocionante, não ironicamente.

É literalmente assim que sua GPU faz sozinha aquele efeito degradê maneiro que nós vimos dois capítulos atrás.

Honestamente não sei o que recomendar se você quiser saber mais sobre coordenadas baricêntricas, talvez o [artigo da wikipedia](https://en.wikipedia.org/wiki/Barycentric_coordinate_system) e uma boa conversa com algum chatbot. Se você souber alemão, também tem o livro [Barycentrische Calcul](https://archive.org/details/gesammeltewerkeh01mbuoft/page/n3/mode/2up), no qual Möbius propôs essa parada toda.

## Conclusão

A matemática é um dos principais gargalos para quem deseja aprender computação gráfica, assim como a complexidade das APIs gráficas. Contudo, com perseverança e estudo, uma hora todos nós internalizamos esses conceitos. O importante é praticar e refrescar a memória sempre.

Como sempre, obrigado por ler até aqui :)

```
         _\|/_
         (o o)
 +----oOO-{_}-OOo---------------------------+
 |                                          |
 | Se necessário, leia o capítulo novamente |
 |       ou tire dúvidas com alguém.        |
 |       Não precisa ter vergonha ;^]       |
 |                                          |
 +------------------------------------------+
```
