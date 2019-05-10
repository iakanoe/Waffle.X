#include "main.h"

#define GIROVEL 700
#define ATAQUEVEL 1000
#define ATRASVEL -1000
#define T_ATRAS 200
#define T_ESPERA 4970
//#define T_ACCEL_GIRO 400

void init(){
    //Todo digital
    ADCON1bits.PCFG = 0b1111;
    
    //Apagar USB (asi se pueden usar pines RC4 RC5)
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1;
    
    //Configurar direccion de pines
    TRIS_BTN_1 = INPUT; 
    TRIS_BTN_2 = INPUT;
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
    
    //Config modulos ccp para PWM
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
    setMotores(1000, 0);
    LAT_LED_R = 0;
    LAT_LED_A = 0;
    LAT_LED_V = 0;
    while(1);
}

void loop(){
    static unsigned long m = 0;
    static int estado = MENU;
    static int direccion = DER;
    switch(estado){
        case MENU:
            if(btn(BTNL)) {
                estado = LIMPIAR;
                break;
            }
            
            if(btn(1)){
                LAT_LED_R = 1;
                LAT_LED_A = 1;
                LAT_LED_V = 1;
                while(btn(1)){
                    m = millis();
                    direccion = DER;
                    estado = ESPERA;
                }
                break;
            }
            
            if(btn(2)){
                LAT_LED_R = 1;
                LAT_LED_A = 1;
                LAT_LED_V = 1;
                while(btn(2)){
                    m = millis();
                    direccion = IZQ;
                    estado = ESPERA;
                }
                break;
            }
            
            setMotores(0, 0);
            LAT_LED_R = ir(1) || ir(2) || cny(1);
            LAT_LED_A = ir(3);
            LAT_LED_V = ir(4) || ir(5) || cny(2);
            break;
            
        case LIMPIAR:
            if(!btn(BTNL)){
                estado = MENU;
                break;
            }
            
            LAT_LED_R = 1;
            LAT_LED_A = 0;
            LAT_LED_V = 1;
            setMotores(1000, 1000);
            break;
            
        case ESPERA:
            if(millis() > (m + T_ESPERA)){
                estado = 1;
                LAT_LED_R = 0;
                LAT_LED_A = 0;
                LAT_LED_V = 1;
                break;
            }
            
            setMotores(0, 0);
            LAT_LED_R = 1;
            LAT_LED_A = millis() > (m + (T_ESPERA / 2));
            LAT_LED_V = 0;
            break;
            
        case ANALISIS:
            if(cny(1) || cny(2)){
                estado = ATRAS;
                m = millis();
                break;
            }
            
            if(ir(3)){
                if(ir(2) == ir(4)){
                    estado = ATAQUE;
                    break;
                }
                
                if(ir(2)){
                    estado = IZQAV;
                    break;
                }
                
                if(ir(4)){
                    estado = DERAV;
                    break;
                }
            }
            
            if(ir(2)){
                estado = IZQA;
                break;
            }
            
            if(ir(4)){
                estado = DERA;
                break;
            }
            
            if(ir(1)){
                estado = IZQ;
                break;
            }
            
            if(ir(5)){
                estado = DER;
                break;
            }
            
            estado = direccion;
            break;
            
        case ATAQUE:
            setMotores(ATAQUEVEL, ATAQUEVEL);
            estado = ANALISIS;
            break;
            
        case DERAV:
            setMotores(ATAQUEVEL, ATAQUEVEL / 2);
            direccion = DER;
            estado = ANALISIS;
            break;
            
        case IZQAV:
            setMotores(ATAQUEVEL / 2, ATAQUEVEL);
            direccion = IZQ;
            estado = ANALISIS;
            break;
            
        case ATRAS:
            if(millis() > (m + T_ATRAS)){
                estado = ANALISIS;
                break;
            }
            
            setMotores(ATRASVEL, ATRASVEL);
            break;
            
        case IZQA:
            setMotores(0, ATAQUEVEL);
            direccion = IZQ;
            estado = ANALISIS;
            break;
            
        case DERA:
            setMotores(ATAQUEVEL, 0);
            direccion = DER;
            estado = ANALISIS;
            break;
            
        case IZQ:
            setMotores(-GIROVEL, GIROVEL);
            direccion = IZQ;
            estado = ANALISIS;
            break;
            
        case DER:
            setMotores(GIROVEL, -GIROVEL);
            direccion = DER;
            estado = ANALISIS;
            break;
            
        case TEST_CNY:
            LAT_LED_R = !PORT_CNY_1;
            LAT_LED_V = !PORT_CNY_2;
            LAT_LED_A = 1;
            if(btn(1) || btn(2)){
                while(btn(1) || btn(2));
                estado = 0;
            }
            break;
    }
}

void setMotores(int dutyI, int dutyD){
    dutyI = limitar(dutyI, -1000, 1000);
    dutyD = limitar(dutyD, -1000, 1000);
    
    LAT_DIR_I = dutyI < 0;
    LAT_DIR_D = dutyD < 0;
    
    dutyI = (dutyI < 0 ? 1000+dutyI : dutyI);
    dutyD = (dutyD < 0 ? 1000+dutyD : dutyD);
    
    dutyI *= 1.023;
    dutyD *= 1.023;
    
    CCPR1L = dutyI / 4;
    CCPR2L = dutyD / 4;
    CCP1CONbits.DC1B = dutyI % 4;
    CCP2CONbits.DC2B = dutyD % 4;
}

unsigned long millisCounter = 0;
unsigned long millis(){ return millisCounter;}
void interrupt ISR(void){
    if(!TMR2IF) return;
    TMR2IF = 0;
    millisCounter++;
}

void main(){
    init();
    while(1) loop();
}