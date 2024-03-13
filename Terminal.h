#ifndef __TERMINAL_H__
#define __TERMINAL_H__


class Terminal {
	private:
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
		

		
	public:
        Terminal();
        ~Terminal();
		void println();
		void print(char ch);
		void scroll();	
		void setCursorPosition(uint8_t row, uint8_t col);
		void clearScren();
		//escreve uma linha de estado no rodapé da tela
		void printStatus();
};

#endif
