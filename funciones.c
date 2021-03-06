#include "main.h"

unsigned long millisCounter = 0;

inline void init(){
    //Todo digital
    ADCON1bits.PCFG = 0b1111;
    
    //Apagar USB (asi se pueden usar pines RC4 RC5)
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1;
    
    //Configurar direccion de pines
    TRIS_BTN_1 = INPUT; 
    TRIS_BTN_2 = INPUT;
    TRIS_SWITCH = INPUT;
    TRIS_LED_R = OUTPUT;
    TRIS_LED_A = OUTPUT;
    TRIS_LED_V = OUTPUT;
    TRIS_IR_1 = INPUT;
    TRIS_IR_2 = INPUT;
    TRIS_IR_3 = INPUT;
//    TRIS_IR_4 = INPUT;
//    TRIS_IR_5 = INPUT;
    TRIS_CNY_1 = INPUT;
    TRIS_CNY_2 = INPUT;
    TRIS_PWM_D = OUTPUT;
    TRIS_PWM_I = OUTPUT;
    TRIS_DIR_D = OUTPUT;
    TRIS_DIR_I = OUTPUT;

    //Config Timer2 para PWM y millis
    T2CON = 0;
    T2CONbits.TOUTPS=11;
	T2CONbits.T2CKPS=1;
	PR2=249;
	T2CONbits.TMR2ON=1;
    
    //Config modulos CCP para PWM
    CCPR1L=0;
	CCP1CONbits.DC1B=0;
	CCP1CONbits.CCP1M=12;
	CCPR2L=0;   
	CCP2CONbits.DC2B=0;
	CCP2CONbits.CCP2M=12;
    
    //Config interrupciones
    INTCON3=0;
	PIE1=0;
	PIE2=0;
	RCONbits.IPEN=0;
	PIE1bits.TMR2IE=1;
	INTCONbits.PEIE_GIEL=1;
	INTCONbits.GIE_GIEH=1;
    RCONbits.IPEN = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //Aplica estado inicial
    setMotores(0, 0);
    LAT_LED_R = 0;
    LAT_LED_A = 0;
    LAT_LED_V = 0;
}

inline void setMotores(int speedI, int speedD){
    speedD = -speedD;
    
    speedI = limitar(speedI, -1000, 1000);
    speedD = limitar(speedD, -1000, 1000);
    
    LAT_DIR_I = u(speedI < 0);
    LAT_DIR_D = u(speedD < 0);
    
    unsigned int dutyI = u(speedI < 0 ? 1000+speedI : speedI);
    unsigned int dutyD = u(speedD < 0 ? 1000+speedD : speedD);
    
    dutyI *= 1.023;
    dutyD *= 1.023;
    
    CCPR1L = dutyI / 4;
    CCPR2L = dutyD / 4;
    CCP1CONbits.DC1B = dutyI % 4;
    CCP2CONbits.DC2B = dutyD % 4;
}

inline unsigned long millis(){
    return millisCounter;
}

void interrupt ISR(void){
    if(!TMR2IF) return;
    TMR2IF = 0;
    millisCounter++;
}
