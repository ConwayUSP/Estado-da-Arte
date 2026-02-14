# Transformações e Espaços

Ainda na pegada do capítulo anterior, vamos apresentar para vocês aplicações matemáticas que são essenciais para o desenvolvimento de um projeto não apenas no OpenGL, mas na computação gráfica como um todo. 

Aqui, mostraremos matrizes utilizadas para as seguintes transformações:

1. Redimensionamento;
2. Translação;
3. Rotação.

Além, é claro, das suas composições e do uso de GLM para a criação delas. Também, apresentaremos os seguintes espaços e como mover entre eles na prática:

1. Local Space;
2. World Space;
3. View Space;
4. Clip Space.

Por fim, demonstraremos projeções ortográficas e de perspectiva e teste de profundidade. Sem mais delongas. Vamos lá:


## Redimensionamento

Imagine que você, ao final desta trilha, está modelando uma simulação onde existe uma árvore e um prédio de 30 andares.

Se a árvore não estiver anabolizada, é natural supor que o prédio terá um tamanho consideravelmente maior, não? 

Ou, melhor, vamos mudar de exemplo: imagine que você está fazendo uma simulação onde uma formiga terá o tamanho de um elefante (por algum motivo). Como seremos capazes de fazer isso no nosso computador?

Visualizando a nossa formiga enquanto objeto computacional com suas devidas componentes _(x, y, z, 1)_, representando suas dimensões na simulação, apresentaremos nossa primeira matriz de transformação e faremos um exemplo de sua aplicação!

Vamos lá. Observe a matriz _4x1_ abaixo:

![Componentes do nosso objeto representado por um vetor](../imagens/05_vetor.png)

Ela é o vetor que representa o nosso objeto. Aqui, não tem mistério:

1. A componente X representa a largura;
2. A componente Y representa a altura;
3. A componente Z representa a profundidade.

Ok! Mas e aquele número 1 na quarta linha da nossa matriz? Se estamos tratando aqui de um modelo 3D, por que o nosso vetor teria quatro componentes?

Ora, aquele valor serve unicamente para facilitar o processo de transformação. Para redimencionar um objeto, alteraremos o valor das componentes através de multiplicações em cada uma. 
Ao invés de multiplicarmos nosso vetor de maneira sucessiva por escalares, vamos "compactar" toda a nossa transformação em uma única nova matriz e realizar apenas uma multiplicação de matrizes. Isso facilita bastante o processo, além de reduzir a complexidade computacional. (Por quê?)

Inclusive, segue abaixo a nossa querida matriz de redimensionamento:

![Matriz utilizada para redimensionar objetos](../imagens/05_matrizred.png)

Ela é uma matriz _4x4_ e possui uma formatação um tanto quanto familiar, não? Perceba que ela é, basicamente, uma matriz identidade com os valores nas três primeiras linhas alterados de maneira arbitrária. No caso, _(S1, S2, S3)_ são nossas variáveis de redimensionamento.

Ao realizar a multiplicação dessa matriz pela outra que representa nosso objeto, teremos o seguinte:

![Aplicação de transformação para redimensionamento](../imagens/05_redimensionamento.png)
>Caso você tenha ficado na dúvida do porquê de chegarmos neste resultado, verifique o capítulo anterior na parte de multiplicação de matrizes.

Mavilha! Repare que essa alteração provocaria uma distorção das dimensões do nosso objeto a partir do que estivesse definido para _(S1, S2, S3)_. Assim, é possível fazer uma formiga ficar com as mesmas proporções que um elefante!


## Translação

No que diz respeito à translação, estamos falando literalmente de mover (transladar) o nosso objeto pela nossa simulação ao alterar sua posição. Para isso, fazemos uma adição entre dois vetores, retornando um novo vetor com uma posição diferente baseada em um vetor de translação.

Se representarmos o nosso vetor de translação como sendo _(Tx, Ty, Tz)_, podemos definir a nossa matriz de translação da seguinte maneira:
![Matriz de translação](../imagens/05_matriztransl.png)

E, assim como fizemos na aplicação de redimensionamento, iremos multiplicar as nossas matrizes para obter um novo resultado, isto é, o nosso vetor com posição atualizada!

![Aplicação de transformação para translação](../imagens/05_translocamento.png)

> _Um breve trecho adaptado do livro Learn OpenGL sobre Coordenadas homogêneas:_ A componente w (a quarta linha, no nosso caso 4x1 acima) de um vetor também é conhecida como coordenada homogênea. Para obter o vetor 3D a partir de um vetor homogêneo, dividimos as coordenadas x, y e z pela sua coordenada w. Normalmente não percebemos isso, pois a componente w é 1,0 na maioria das vezes. O uso de coordenadas homogêneas tem várias vantagens: permite
realizar translações matriciais em vetores 3D (sem uma componente w não podemos transladar
vetores) e usar o valor de w para criar perspectiva 3D. Além disso,
sempre que a coordenada homogênea for igual a 0, o vetor é especificamente conhecido como um
vetor de direção, pois um vetor com coordenada w igual a 0 não pode ser transladado.

Logo, com a matriz de translação no nosso arsenal, podemos mover objetos em qualquer um dos três eixos _(X, Y, Z)_, o que é essencial para nós.

## Rotação

![Rotação](../imagens/05_angulo.png)

Quando falamos de rotação, é possível perceber que aqui as coisas ficam um pouco mais complexas do que as duas aplicações que acabamos de apresentar. Isso, pois, elas são mais intuitivas. Tentaremos manter a simplicidade aqui o máximo possível. 

Podemos rotacionar no nosso objeto em torno de cada um dos eixos. Para cada um, temos uma matriz de rotação diferente que se baseia em um ângulo theta (Θ), que é baseado em graus ou radianos e é recebido pelas funções trigonométricas _seno_ e _cosseno_. Explicar detalhadamente o funcionamento delas acaba saindo um pouco do escopo da nossa trilha. Se você quiser se aprofundar, pesquise vídeos no youtube ou livros voltados para isso.

A seguir, temos cada uma das matrizes de rotação. Novamente, realizaremos multiplicação com cada uma:
### _Rotação em torno do eixo X_:
![Matriz de rotação em torno de x](../imagens/05_matrizrotacaox.png)
![Aplicação de rotação em torno de x](../imagens/05_rotacaox.png)

### _Rotação em torno do eixo Y_:
![Matriz de rotação em torno de y](../imagens/05_matrizrotacaoy.png)
![Aplicação de rotação em torno de y](../imagens/05_rotacaoy.png)

### _Rotação em torno do eixo Z_:
![Matriz de rotação em torno de z](../imagens/05_matrizrotacaoz.png)
![Aplicação de rotação em torno de z](../imagens/05_rotacaoz.png)

### _Matriz para rotação em torno de eixo arbitrário_:
Para rotacionar em torno de um eixo 3D arbitrário, nós podemos combinar todas as três que acabamos de apresentar ao primeiro rotacionar em torno do eixo X, depois Y e então Z, por exemplo. Entretanto, isso rapidamente gera um problema chamado [_Gimbal lock_](https://pt.wikipedia.org/wiki/Gimbal_lock). 

Não discutiremos os detalhes, mas uma solução melhor é rotacionar em torno de um eixo unitário arbitrário, por exemplo, (0,662, 0,2, 0,722) (observe que este é um vetor unitário), diretamente em vez de combinar as
matrizes de rotação. Tal matriz existe e é dada abaixo com _(Rx, Ry, Rz)_ como o eixo de rotação arbitrário (que coisinha linda...):

![Matriz genérica de rotação](../imagens/05_coisinhalinda.png)

Ainda assim, essa matriz não previne completamente o _Gimbal lock_. Para realmente evitar, precisamos representar as rotações usando [quatérnios](https://pt.wikipedia.org/wiki/Quaterni%C3%A3o), que não são apenas mais seguros, mas também mais amigáveis computacionalmente. No entanto, uma discussão sobre quatérnios está fora do escopo deste capítulo.

## Combinando Matrizes

O verdadeiro poder de usar matrizes para transformações é que podemos combinar múltiplas transformações em uma única matriz graças à multiplicação de matrizes. Observe a seguinte combinação entre uma matriz para translocação e outra para redimensionamento:

![Exemplo de combinação de matrizes](../imagens/05_combinacao.png)

Agora, em termos de complexidade computacional, imagine que você fosse aplicar aquela translocação e aquele redimensionamento em, talvez, milhares ou milhões de objetos em uma simulação de computação gráfica. O fato de "empacotarmos" essas duas transformações em uma única matriz reduz pela metade o nosso custo computacional, o que é extremamente valioso em simulações que exigem bastante desempenho.

Lembrando que a multiplicação entre matrizes não é comutativa! O que significa que a ordem importa, interfere no resultado final.

Ao multiplicar matrizes, a matriz mais à direita é multiplicada primeiro pelo vetor, portanto, você deve ler as multiplicações da direita para a esquerda. Recomenda-se realizar primeiro as operações de escala, depois as rotações e, por último, as translações ao combinar matrizes; caso contrário, elas podem afetar-se (negativamente).

Aplicando a nossa matriz acima no nosso vetor _(x, y, z, 1)_, temos:
![Aplicação de combinação de matrizes](../imagens/05_aplicacaocombinada.png)
Maravilha! O vetor é primeiro escalado por dois e depois transladado por (1,2,3).

Agora que falamos um pouco sobre as transformações, podemos ir para a parte do código e ver como que funciona na prática. O OpenGL não possui nenhuma forma de conhecimento ou funcionalidades de matrizes ou vetores, então precisamos definir nossas próprias classes e funções. Felizmente, existe uma biblioteca matemática fácil de usar e feita sob medida para OpenGL chamada GLM. Acompanhe!

## GLM

GLM significa OpenGL Mathematics e é uma biblioteca composta apenas por arquivos de cabeçalho, o que significa que basta incluir os arquivos de cabeçalho apropriados e pronto; não é necessário vincular nem compilar.

Para instalar no Linux (saia do Windows imediatamente), temos duas formas comuns de fazer:

### 1. Usando o Gerenciador de Pacotes (Recomendado)

A maioria das distribuições Linux possui o GLM nos repositórios oficiais. Essa é a melhor opção porque facilita as atualizações e a gestão de dependências.

_Ubuntu / Debian / Linux Mint:_
```
sudo apt update
sudo apt install libglm-dev
```
_Fedora:_
```
sudo dnf install glm-devel
```
_Arch Linux:_
```
sudo pacman -S glm
```

### 2. Instalação Manual (Via GitHub)

Se você precisa de uma versão específica ou quer a mais recente que ainda não chegou nos repositórios, pode baixar direto do código-fonte.

_Clone o repositório:_
```
git clone https://github.com/g-truc/glm.git
```
_Mova para o diretório de inclusão local (opcional, mas útil para acesso global):_
```
sudo cp -r glm/glm /usr/local/include/
```


### 2. Manualmente
