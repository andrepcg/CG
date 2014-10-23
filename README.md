Computer Graphics Project 2014
==

This is a project for Computer Graphics course in Computer Science @ DEI.pt

Goal
--
Create a simple game that teaches us how to work with OpenGL

Game Description
--
You control a player in a big open field. You must capture the missions to improve your player skills (attack, speed, jump, etc...). These missions spawn in a random position in the field. You must go there and stay within the radius for X amount of seconds. After being in the are for the required time, you improve your player skills and/or win some special powers. 

The difficulty of the game is created by having hundreds of enemies roaming around the field, colliding with each other and obstacles (created by the player or just trees). The player must avoid being around the enemies otherwise he will suffer damage.

Techniques
--
 - Quad tree for storing entities positions so they can collide with each other (they don't overlap)
 - X*Z Grid to store the collision map. 0 is empty, 1 is blocked.
 - Each entity (enemy) has is own AI (independent of each other). They target the player within a specific radius

 
===========================

Um Android aos tiros
========================================

###Computação Gráfica 2013/2014
###Departamento de Engenharia Informática, Universidade de Coimbra
####{apcosta,rsgomes}@student.dei.uc.pt
-------

##1. Introdução:
Neste projeto optámos por desenvolver um jogo FPS, onde a personagem é um robot Android que dispara tiros contra os seus adversários. Decorre num abiente tropical constituido por palmeiras, areia, um céu soalheiro e vegetação.
Para tal, recorremos às funcionalidades procedimentais do OpenGl que nos ajudou na modelação dos objetos, na criação de luzes, importação de modelos, cenário em 3D. Ocorreram no entanto algumas limitações devido ao uso do pipeline fixo, como por exemplo dos objetos, das texturas e da aproximação à realidade da parte da iluminação onde implementámos uma malha de polígonos para conseguirmos um efeito mais próximo do real.

Em termos práticos, abordamos os seguintes conceitos:
* Modelização e transformações geométricas: Rotações, translações, escalas, desenho de polígonos
* Projeções e transformações de visualização: Perspetiva de jogo em first-person e third-person
* Iluminação: Luz ambiente e um foco por cima de cada baú
* Cores/Texturas: Textura nos pol´ıgonos constituintes do ambiente e nos modelos importados
* Modelos outsource
* Outros: Agentes reativos e deteção de colissões

##2. Modelização e transformações geométricas
A cena, que no fundo é uma skybox, é composta pelos os inimigos do robot, as caixas, o chão, e os modelos importados que são as palmeiras e os baús. Como é um jogo, tem de ser dinâmico, havendo interacção pela parte do utilizador e do ambiente, logo implica a implementação de translações e rotações para os movimentos do robot e seus inimigos, assim como dos tiros que são disparados.

##3. Projeções e transformações de visualização
No nosso modelo de visualização, é possível acompanhar dinamicamente a posição do robot e a direção da visão segue os movimentos do rato. Para tal foi necessário recolher as coordenadas do cursor de forma a convertê-las em ângulos e usá-las para defi a rotação da câmara. O resultado final aproxima-se, quer a nível visual como computacional (flûencia, exigência computacional) com os sistemas implementados em jogos (FPS), simuladores, etc. Em termos de vista do robot, pode-se jogar em primeira ou terceira pessoa, de forma a visualizar o corpo todo do robot. Usámos um viewport para a janela de visualização geral do jogo.

##4. Iluminação
Implementou-se uma luz ambiente de cor laranja a simular a cor do pôr do sol e focos a apontar para os baús, de forma a que o jogador os possa localizar facilmente. Constrúımos também uma malha de polígonos para tornar a luz do foco mais real porque num pipeline fixo um foco ilumina os vértices todos dum polígono, não permitindo iluminação por píxel, como ocorre numa pipeline programável, através dos fragment shaders. A malha de polígonos vai dividir o nosso polígono em diversos polígonos e assim já é possível obter um gradiente de iluminação. Claro que quanto maior for o número de divisões feitas num polígno, maior será a definição mas em contra partida teremos mais trabalho computacional, o que tornará o processo mais lento. Cada objeto tem a sua pro´pria componente difusa e especular, por exemplo o nosso robot tem uma componente especular bem acentuada, o que se reflete num brilho mais intenso que os outros objetos.

##5. Cores e texturas
Na skybox foi aplicada a textura do céu, onde não se nota a diferença nos vários lados do cubo pois usámos o Clamp e como a textura é igual não há variação de imagem. Aplicámos também textura nos inimigos, nas caixas e outra transparente na relva, pois esta é composta por um rectângulo e não seria bom vê-lo com relva e o restande espa¸co preenchido a branco, portanto, desta forma fazemos com que esse espaço branco fique transparente.

##6. Modelos _outsource_
Os baús e as palmeiras foram modelos importados da internet, ou seja, importámos ficheiros de objetos que têm normais, vértices e face. Passámos para um array e depois fazemos o draw desse array, faz-se o bind da textura e desenha-se o modelo.

##7. Funcionamento do jogo
O objetivo principal do jogo consiste na execução de várias miss~oes onde os inimigos dificultam a vida ao nosso robot. As missões são compostas por baús e para serem sucedidas, o robot terá de permanecer dentro de uma dada circunfer^encia à volta do baú durante o tempo indicado para a missão. Após sucedida, o robot ganha um bónus aleatório da seguinte lista:
* Escudo de proteção
* Vida
* Salto
* Dano
* Velocidade
* Uma caixa para usar como obstáculo
* Ataque especial

O robot dispõe de tiros para matar os seus inimigos, de caixas que pode colocar no chão de modo a ser um obstáculo para o proteger dos inimigos e tem também obstáculos do mapa que são as palmeiras, as quais não deixam o robot passar por elas.

###7.1. Como jogar
São usadas as teclas W-A-S-D para movimentar o robot, o rato para controlar a câmara e apontar/disparar e o espaço para saltar.

###7.2. Detecção de colisões

As várias entidades que circulam pelo mundo efectuam detecção de colisões a cada frame. De forma a que se possam ter centenas de entidades a circular, foi preciso criar um sistema que permita saber, no menor número de etapas, se há uma colisão, quer seja com uma outra entidade ou grelha, ou não.

O sistema de colisões está dividido em duas partes:
* Detecção de colisões com entidades móveis
* Detecção de colisões com objectos estáticos
 
####7.2.1. Quadtree
Uma quadtree é um sistema de particionamento de espaço. Este sistema permite saber quais as únidades mais perto de determinado ponto, com o objectivo de efectuar a verificação de colisão com o menor número de entidades. 

Vamos definir um exemplo de forma a mostrar a vantagem de uma quadtree. Supondo que existem 100 entidades a circular pelo mundo e queremos que cada entidade repila todas as outras de forma a que não se sobreponham.

#####Sem Quadtree
	Para cada entidade, detectar colisão com as restantes 99.
    100 * 100 = 10000 detecções de colisão / frame (!!!)

![Alt text](http://i.imgur.com/6Ug8owJ.jpg)

Uma quadtree subdivide o espaço em 4 quadrantes recursivamente, isto é, cada quandrante possui outros 4 quadrantes, dependendo do número de entidades inseridas nesse quadrante. Existe um limite definido para o número máximo de entidades por quadrante e assim que é atingido esse limite o quadrante subdivide. Cada quandrante tem uma bounding box que o define, permitindo facilmente detectar se uma outra bounding box está a colidir (chamadas colisões AABB).

![](http://i.imgur.com/TcKoEJ5.png)

Na imagem anterior é mostrada uma quadtree e no topo encontra-se um rectângulo. O objectivo é “perguntar” à quadtree que entidades se encontram dentro do rectângulo amarelo. A quadtree facilmente sabe quais os quadrantes que sobrepõem o rectângulo, também denomidado de query range, através de colisões AABB. 

Cada entidade móvel possui a sua bounding box utilizada para detectar que entidades se encontram dentro da mesma. Como detecções AABB são bastante rápidas, cada uma das centenas de entidades pode saber com rapidez que outras entidades se encontram ao seu redor. Caso seja encontrada alguma, executa o procedimento de as repelir.

![](http://i.imgur.com/MQKVQJB.png)

Na imagem anterior podemos visualizar uma quadtree que podemos uma quadtree com 6 entidades. No quadrante a verde encontram-se 3 entidades. Foi definido que o número máximo de entidades por quadrante são 3, sendo assim o quadrante subdivide, como mostrado. Cada rectângulo vermelho em volta de cada entidade representa a zona onde efectua colisão. Se uma entidade estiver no rectângulo da outra, ela será repelida.

![](http://i.imgur.com/VEed18f.png)

####7.2.2. Grelha

Pretende-se efectuar colisões com objectos no mundo que se encontram estáticos (ex: palmeira, caixa...). Podia usar-se igualmente a quadtree para o armazenamento destes objectos mas optou-se por uma grelha com N número de células. Esta grelha armazena um inteiro em cada célula para saber se está ou não bloqueada (0 ou 1). Todas as células têm o mesma comprimento e largura, definido no código.

![](http://i.imgur.com/TsMGGrx.png)

A imagem anterior representa uma grelha do mundo. Células pintadas encontram-se bloqueadas e não é possível atravessá-las. O objectivo desta grelha é impedir entidades de atravessarem determinados objectos, criando um ambiente mais realista com interacções.

###7.3. Agentes reactivos

O jogo é caracterizado por centenas de inimigos a vaguear pelo mundo. Cada uma destas entidades possui a sua inteligência artificial, ou seja, são processadas individualmente apesar das reacções ao mundo serem as mesmas.

Um agente pode encontrar-se nos seguintes estados:
* Vagueando
* Chegou ao destino
* À espera após chegar ao destino
* Perseguir jogador

Inicialmente, cada entidade escolhe uma posição aleatória no mundo e dirige-se em linha recta para o destino, colidindo com objectos e repelindo outros agentes. Após chegar ao destino aguarda um determinado número de segundos escolhido de forma aleatória. Após esperar este tempo, escolhe nova localização e segue viagem.

Cada agente possui também um raio de visão à espera de encontrar o jogador. Se o jogador entrar neste raio de visão, o agente persegue o jogador e ataca-o.

A melhorar:
* Diferentes personalidades e características por agente
* Agentes agrupam-se formando uma horda

