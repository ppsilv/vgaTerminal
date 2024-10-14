#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#define AZUL      0
#define BRANCO    1
#define CIANO     2
#define AMARELO   3
#define MAGENTA   4
#define VERMELHO  5
#define VERDE     6


class Terminal {
	private:
    const unsigned int alarme = 5250;
		//parametros da tela
		uint8_t		screenTotalChar; 	// contador de caracter na tela
		uint8_t		totalRow;		// total de linhas da tela
		uint8_t		totalCol;		// total de colunas da tela
		
		//variaveis do estado da tela	
		uint8_t		row;		//linha atual
		uint8_t		col;		//coluna atual
		uint16_t	totalChar;	//total de caractere atualmente na tela
		uint8_t		bgcolor;	//cor de fundo da tela
		uint8_t		fgcolor;	//cod do texto
    unsigned long future;
		bool cursorStatus;
	
	public:
        Terminal();
        ~Terminal();
		void println();
		void print(const char ch);
		void scroll();	
    
    void showCursor();
    void showCursorCursorStatus();
		void setCursorPosition(uint8_t row, uint8_t col);
    void run();
    void execCmdVt100();
		//escreve uma linha de estado no rodapé da tela
		void printStatus();

    bool getCursorStatus(){
      return cursorStatus;
    }
    void setCursorStatus(bool status){
      cursorStatus = status;
    }

};

#endif
