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

#define TRIS_SWITCH TRISCbits.RC7
#define PORT_SWITCH PORTCbits.RC7

enum {
    OUTPUT,
    INPUT
};

// Arreglo problema del interrupt en XC8 >=2.0
#if __XC8_VERSION >= 2000
#   define interrupt __interrupt()
#endif

// Hago que el switch use el método más rápido de comparación
// (que es lo más útil para máquinas de estados)
#pragma switch time

// Para no tener conversiones implicitas de signed a unsigned
#define u (unsigned)

#define btn(n) u!PORT_BTN_ ## n
#define ir(n) u!PORT_IR_ ## n
#define cny(n) u!PORT_CNY_ ## n
#define sw() u!PORT_SWITCH
#define limitar(x, min, max) (x > max ? max : (x < min ? min : x))

inline void init();
inline void loop();
inline void setMotores(int dutyI, int dutyD);
inline unsigned long millis();

inline void estrategiaClasica();
inline void estrategiaCiego();
inline void estrategiaAdelante();
inline void estrategiaAtras();

enum { // Estados menu
    MENU,
    LIMPIAR,
    ESPERA,
    E_CLASICA,
    E_CIEGO,
    E_ADELANTE,
    E_ATRAS
};

enum { // Estados estrategias
    ANALISIS,
    ATAQUE,
    DER,
    DERA,
    DERAV,
    IZQ,
    IZQA,
    IZQAV,
    ATRAS,
    POSICIONAR,
    AVANZAR,
    FRENAR
};