#define ESCOLHA_OFF      0 //Usado para controlar LEDs
#define ESCOLHA_NENHUMA  0 //Usado para verificar botÃƒÂµes
#define ESCOLHA_VERMELHO (1 << 0)
#define ESCOLHA_VERDE    (1 << 1)
#define ESCOLHA_AZUL     (1 << 2)
#define ESCOLHA_AMARELO  (1 << 3)

#define LED_VERMELHO 13
#define LED_VERDE    3
#define LED_AZUL     10
#define LED_AMARELO  6

// Pinos dos botÃƒÂµes
#define BOTAO_VERMELHO 12
#define BOTAO_VERDE    2
#define BOTAO_AZUL     9
#define BOTAO_AMARELO  5

// DefiniÃƒÂ§ÃƒÂµes de pinos do som
#define SOM  7

// Definir parÃƒÂ¢metros do jogo
#define RODADAS_PARA_VENCER 8   //NÃƒÂºmero de rodadas para ganhar 10. 

 
#define LIMITE_DE_TEMPO     3000 //Quantidade de tempo para pressionar um botÃƒÂ£o antes que o jogo expire. 000ms = 3seg

// VariÃƒÂ¡veis Ã¢â‚¬â€¹Ã¢â‚¬â€¹de estado do jogo
byte sequenciaJogada[32];       //ContÃƒÂ©m a combinaÃƒÂ§ÃƒÂ£o de botÃƒÂµes ÃƒÂ  medida que avanÃƒÂ§amos
byte nivelJogo = 0;             //Conta o nÃƒÂºmero de rodadas com sucesso que o jogador fez

void setup()
{
    //Configurar perifericos de entradas
    pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
    pinMode(BOTAO_VERDE,    INPUT_PULLUP);
    pinMode(BOTAO_AZUL,     INPUT_PULLUP);
    pinMode(BOTAO_AMARELO,  INPUT_PULLUP);

    //Configurar perifericos de saÃƒÂ­das
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(LED_VERDE,    OUTPUT);
    pinMode(LED_AZUL,     OUTPUT);
    pinMode(LED_AMARELO,  OUTPUT);
    pinMode(SOM,          OUTPUT);

    JogadorAcertou(); // Toca uma mÃƒÂºsica antes de comeÃƒÂ§ar
}

void loop()
{
    MonstarLuz(); // Pisca luzes enquanto aguarda o usuÃƒÂ¡rio apertar um botÃƒÂ£o
    // Indique o inÃƒÂ­cio do jogo
    definirLEDs(ESCOLHA_VERMELHO | ESCOLHA_VERDE | ESCOLHA_AZUL | ESCOLHA_AMARELO); // Ativar todos os LEDs
    delay(1000);
    definirLEDs(ESCOLHA_OFF); // Desligue os LEDs
    delay(250);

    // Jogar jogo de memÃƒÂ³ria e lidar com resultado
    if (JogoDaMemoria() == true)
        JogadorAcertou(); // Jogador venceu, joga os tons de vencedor
    else
        JogadorErrou();   // Jogador perdido, jogar tons de perdedor
}
// Retorna 0 se o jogador errou ou 1 se o jogador acertou
boolean JogoDaMemoria(void)
{
    randomSeed(millis()); // Aqui ÃƒÂ© a escolha aletÃƒÂ³ria

    nivelJogo = 0; // Reinicie o jogo para o inÃƒÂ­cio

    while (nivelJogo < RODADAS_PARA_VENCER)
    {
        Add_Movimentos(); // Adicione um botÃƒÂ£o aos movimentos

        ReproduzirMovimentos(); // Reproduza os movimentos jÃƒÂ¡ feitos

        // Aqui ÃƒÂ© exigido que o jogador repita a sequÃƒÂªncia.
        for (byte movimentoAtual = 0; movimentoAtual < nivelJogo; movimentoAtual++)
        {
            byte escolha = wait_for_button(); // Veja qual botÃƒÂ£o o usuÃƒÂ¡rio pressionou

            if (escolha == 0) return false; // Se a espera expirar, o jogador perde

            if (escolha != sequenciaJogada[movimentoAtual]) return false; // Se a escolha estiver incorreta, o jogador perde
        }

        delay(1000); // Se o jogador acertou, espera por 1 segundo antes de continuar
    }

    return true; // O jogador passou por todas as rodadas e zerou 
}

// Reproduz o conteÃƒÂºdo atual dos movimentos do jogo
void ReproduzirMovimentos(void)
{
    for (byte movimentoAtual = 0; movimentoAtual < nivelJogo; movimentoAtual++)
    {
        toner(sequenciaJogada[movimentoAtual], 150);

        // Espere algum tempo entre a reproduÃƒÂ§ÃƒÂ£o do botÃƒÂ£o
        // Reduza para tornar o jogo mais difÃƒÂ­cil
        delay(150); // 150 funciona bem. 75 fica rÃƒÂ¡pido.
    }
}

// Adiciona um novo botÃƒÂ£o aleatÃƒÂ³rio ÃƒÂ  sequÃƒÂªncia do jogo
void Add_Movimentos(void)
{
    byte novoBotao = random(0, 4); //min (incluido), max (excluido)

    // AvariÃƒÂ¡vel novoBotao deve ser preencia com os nÃƒÂºmeros de 0 a 3
    if (novoBotao      == 0) novoBotao = ESCOLHA_VERMELHO;
    else if (novoBotao == 1) novoBotao = ESCOLHA_VERDE;
    else if (novoBotao == 2) novoBotao = ESCOLHA_AZUL;
    else if (novoBotao == 3) novoBotao = ESCOLHA_AMARELO;

    sequenciaJogada[nivelJogo++] = novoBotao; // Adicione este novo botÃƒÂ£o ao array do jogo
}

// Acende o respectivo LED
// Passe em um byte que ÃƒÂ© composto de ESCOLHA_VERMELHO, ESCOLHA_AMARELO, etc
void definirLEDs(byte leds)
{
    if ((leds & ESCOLHA_VERMELHO) != 0)
        digitalWrite(LED_VERMELHO, HIGH);
    else
        digitalWrite(LED_VERMELHO, LOW);

    if ((leds & ESCOLHA_VERDE) != 0)
        digitalWrite(LED_VERDE, HIGH);
    else
        digitalWrite(LED_VERDE, LOW);

    if ((leds & ESCOLHA_AZUL) != 0)
        digitalWrite(LED_AZUL, HIGH);
    else
        digitalWrite(LED_AZUL, LOW);

    if ((leds & ESCOLHA_AMARELO) != 0)
        digitalWrite(LED_AMARELO, HIGH);
    else
        digitalWrite(LED_AMARELO, LOW);
}

// Espere atÃƒÂ© que um botÃƒÂ£o seja pressionado.
// Retorna uma das cores do LED (LED_VERMELHO, etc.) se for bem-sucedido, 0 se o tempo expirou
byte wait_for_button(void)
{
    long startTime = millis(); // Aqui comeÃƒÂ§a a contar o tempo

    while ((millis() - startTime) < LIMITE_DE_TEMPO) // Permanece nesse loop atÃƒÂ© que o tempo limite seja atingido
    {
        byte button = botaoPrecionado();

        if (button != ESCOLHA_NENHUMA)
        {
            toner(button, 150); // Toque o botÃƒÂ£o que o usuÃƒÂ¡rio acabou de pressionar
            while (botaoPrecionado() != ESCOLHA_NENHUMA);  // Vai permanecer neste laÃƒÂ§o atÃƒÂ© que o jogador solte o botÃƒÂ£o
            delay(10); // Este delay evita duplos cliques no botÃƒÂ£o
            return button;
        }

    }
    return ESCOLHA_NENHUMA; // Se chegarmos aqui, o tempo esgotou e o jogador perdeu
}

// Retorna um bit '1' na posiÃƒÂ§ÃƒÂ£o correspondente a ESCOLHA_VERMELHO, ESCOLHA_VERDE, etc.
byte botaoPrecionado(void)
{
    if (digitalRead(BOTAO_VERMELHO) == 0) return(ESCOLHA_VERMELHO);
    else if (digitalRead(BOTAO_VERDE) == 0) return(ESCOLHA_VERDE);
    else if (digitalRead(BOTAO_AZUL) == 0) return(ESCOLHA_AZUL);
    else if (digitalRead(BOTAO_AMARELO) == 0) return(ESCOLHA_AMARELO);

    return(ESCOLHA_NENHUMA); // Se nenhum botÃƒÂ£o for pressionado, nÃƒÂ£o retorna nenhum
}

/*
    Acenda um LED e toque um tom
    Vermelho: pulso de 440 Hz - 2,272 ms - 1,136 ms
    Verde: pulso de 880 Hz - 1.136 ms - 0,568 ms
    Azul: 587,33 Hz - 1,702 ms - pulso de 0,851 ms
    Amarelo: pulso de 784 Hz - 1,276 ms - 0,638 ms
*/
void toner(byte which, int largura_do_pulso)
{
    definirLEDs(which); //Turn on a given LED

    //Toque o som associado ao LED
    switch (which)
    {
    case ESCOLHA_VERMELHO:
        EmitirSom(largura_do_pulso, 1136);
        break;
    case ESCOLHA_VERDE:
        EmitirSom(largura_do_pulso, 568);
        break;
    case ESCOLHA_AZUL:
        EmitirSom(largura_do_pulso, 851);
        break;
    case ESCOLHA_AMARELO:
        EmitirSom(largura_do_pulso, 638);
        break;
    }
    definirLEDs(ESCOLHA_OFF); // Desligue todos os LEDs
}

// Alterna o som a cada delay_som, por uma duraÃƒÂ§ÃƒÂ£o de largura_do_pulso.
void EmitirSom(int largura_do_pulso, int delay_som)
{
    // Converter o tempo total de reproduÃƒÂ§ÃƒÂ£o de milissegundos em microssegundos
    long buzz_length_us = largura_do_pulso * (long)1000;

    // Loop atÃƒÂ© que o tempo restante seja menor que um ÃƒÂºnico delay_som
    while (buzz_length_us > (delay_som * 2))
    {
        buzz_length_us -= delay_som * 2; //Diminua o tempo restante

        // Alterne o som em vÃƒÂ¡rias velocidades
        digitalWrite(SOM, HIGH);
        delayMicroseconds(delay_som);
        digitalWrite(SOM, LOW);
        delayMicroseconds(delay_som);
    }
}

// Toque o som e acenda as luzes do vencedor
void JogadorAcertou(void)
{
    definirLEDs(ESCOLHA_VERDE | ESCOLHA_AZUL);
    SomVencedor();
    definirLEDs(ESCOLHA_VERMELHO | ESCOLHA_AMARELO);
    SomVencedor();
    definirLEDs(ESCOLHA_VERDE | ESCOLHA_AZUL);
    SomVencedor();
    definirLEDs(ESCOLHA_VERMELHO | ESCOLHA_AMARELO);
    SomVencedor();
}

// Toque o som vencedor
void SomVencedor(void)
{
    // Alterne a som em vÃƒÂ¡rias velocidades
    for (byte x = 250; x > 70; x--)
    {
        for (byte y = 0; y < 3; y++)
        {
            digitalWrite(SOM, HIGH);
            delayMicroseconds(x);

            digitalWrite(SOM, LOW);
            delayMicroseconds(x);
        }
    }
}

// Toque o som / luzes de derrota
void JogadorErrou(void)
{
    definirLEDs(ESCOLHA_VERMELHO | ESCOLHA_VERDE);
    EmitirSom(255, 1500);

    definirLEDs(ESCOLHA_AZUL | ESCOLHA_AMARELO);
    EmitirSom(255, 1500);

    definirLEDs(ESCOLHA_VERMELHO | ESCOLHA_VERDE);
    EmitirSom(255, 1500);

    definirLEDs(ESCOLHA_AZUL | ESCOLHA_AMARELO);
    EmitirSom(255, 1500);
}

// Mostra uma exibiÃƒÂ§ÃƒÂ£o de "modo de atraÃƒÂ§ÃƒÂ£o" enquanto espera que o usuÃƒÂ¡rio pressione o botÃƒÂ£o.
void MonstarLuz(void)
{
    while (1)
    {
        definirLEDs(ESCOLHA_VERMELHO);
        delay(100);
        if (botaoPrecionado() != ESCOLHA_NENHUMA) return;

        definirLEDs(ESCOLHA_AZUL);
        delay(100);
        if (botaoPrecionado() != ESCOLHA_NENHUMA) return;

        definirLEDs(ESCOLHA_VERDE);
        delay(100);
        if (botaoPrecionado() != ESCOLHA_NENHUMA) return;

        definirLEDs(ESCOLHA_AMARELO);
        delay(100);
        if (botaoPrecionado() != ESCOLHA_NENHUMA) return;
    }
}



