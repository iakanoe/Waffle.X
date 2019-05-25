#define MAIN_H
#include <xc.h>

#pragma config PLLDIV = 5, CPUDIV = OSC1_PLL2, USBDIV = 2, FOSC = HSPLL_HS, FCMEN = OFF, IESO = OFF, PWRT = OFF
#pragma config BOR = OFF, BORV = 0, VREGEN = OFF, WDT = OFF, WDTPS = 32768, CCP2MX = ON
#pragma config PBADEN = OFF, LPT1OSC = OFF, MCLRE = OFF, STVREN = OFF, LVP = OFF, XINST = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTRB = OFF

#define TRIS_BTN_1  TRISAbits.RA1
#define TRIS_BTN_2  TRISAbits.RA5
#define PORT_BTN_1  PORTAbits.RA1
#define PORT_BTN_2  PORTAbits.RA5

#define TRIS_LED_R  TRISAbits.RA2
#define TRIS_LED_A  TRISAbits.RA3
#define TRIS_LED_V  TRISAbits.RA4
#define LAT_LED_R   LATAbits.LA2
#define LAT_LED_A   LATAbits.LA3
#define LAT_LED_V   LATAbits.LA4

#define TRIS_IR_1   TRISBbits.RB5
#define TRIS_IR_2   TRISBbits.RB4
#define TRIS_IR_3   TRISBbits.RB1
//#define TRIS_IR_4   TRISCbits.RC5
//#define TRIS_IR_5   TRISCbits.RC4
#define PORT_IR_1   PORTBbits.RB5
#define PORT_IR_2   PORTBbits.RB4
#define PORT_IR_3   PORTBbits.RB1
#define PORT_IR_4   PORTCbits.RC5
#define PORT_IR_5   PORTCbits.RC4

#define TRIS_CNY_1  TRISBbits.RB3
#define TRIS_CNY_2  TRISCbits.RC6
#define PORT_CNY_1  PORTBbits.RB3
#define PORT_CNY_2  PORTCbits.RC6

#define TRIS_DIR_D  TRISCbits.RC0
#define TRIS_DIR_I  TRISAbits.RA0
#define LAT_DIR_D   LATCbits.LC0
#define LAT_DIR_I   LATAbits.LA0

#define TRIS_PWM_D  TRISCbits.RC1
#define TRIS_PWM_I  TRISCbits.RC2
#define LAT_PWM_D   LATCbits.LC1
#define LAT_PWM_I   LATCbits.LC2

enum {
    OUTPUT,
    INPUT
};

// Arreglo problema del interrupt en XC8 >=2.0
#if __XC8_VERSION >= 2000
#   define interrupt __interrupt()
#endif

// Aviso que en este robot no hay BTNL (borrar estas lineas cuando se arregle)
#warning En Waffle no hay BTNL
#define PORT_BTN_BTNL 0

// Hago que el switch use el método más rápido de comparación
// (que es lo más útil para máquinas de estados)
#pragma switch time

//#define btn(n) (n == 1 ? !PORT_BTN_1 : (n == 2 ? !PORT_BTN_2 : 0))
//#define ir(n) (n == 1 ? !PORT_IR_1 : (n == 2 ? !PORT_IR_2 : (n == 3 ? !PORT_IR_3 : (n == 4 ? !PORT_IR_4 : (n == 5 ? !PORT_IR_5 : 0)))))
//#define cny(n) (n == 1 ? !PORT_CNY_1 : (n == 2 ? !PORT_CNY_2 : 0))

// Para no tener conversiones implicitas de signed a unsigned
#define u (unsigned)

#define btn(n) u!PORT_BTN_ ## n
#define ir(n) u!PORT_IR_ ## n
#define cny(n) u!PORT_CNY_ ## n
#define limitar(x, min, max) (x > max ? max : (x < min ? min : x))

void setMotores(int dutyI, int dutyD);
inline unsigned long millis();

enum {
    MENU,
    LIMPIAR,
    ESPERA,
    ANALISIS,
    ATAQUE, 
    DER, 
    DERA,
    DERAV,
    IZQ, 
    IZQA,
    IZQAV,
    ATRAS,
    TEST_CNY,
    DERCIEGO
};