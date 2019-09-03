#include "main.h"
#include "constants.h"

char direccion = DER;
static char estadoMenu = MENU;

inline void loop(){
    static unsigned long m = 0;
    static char estrategia = E_CLASICA;
    
    switch(estadoMenu){
        case MENU:            
            if(btn(1)){
                LAT_LED_R = 1;
                LAT_LED_A = 1;
                LAT_LED_V = 1;
                m = millis();
                direccion = DER;
                estadoMenu = ESPERA;
                while(btn(1))
                    while(btn(2))
                        estadoMenu = LIMPIAR;
                return;
            }
            
            if(btn(2)){
                LAT_LED_R = 1;
                LAT_LED_A = 1;
                LAT_LED_V = 1;
                m = millis();
                direccion = IZQ;
                estadoMenu = ESPERA;
                while(btn(2))
                    while(btn(1))
                        estadoMenu = LIMPIAR;
                return;
            }
            
            setMotores(0, 0);
            LAT_LED_R = u(ir(1) || ir(2) || cny(1));
            LAT_LED_A = u(ir(3));
            LAT_LED_V = u(ir(4) || ir(5) || cny(2));
            return;
            
        case LIMPIAR:
            if(btn(1) || btn(2)){
                estadoMenu = MENU;
                while(btn(1) || btn(2));
                return;
            }
            
            LAT_LED_R = 1;
            LAT_LED_A = 0;
            LAT_LED_V = 1;
            setMotores(1000, 1000);
            return;
            
        case ESPERA:
            if(millis() > (m + T_ESPERA)){
#ifdef CIEGO
                estadoMenu = E_CIEGO;
#else
                estadoMenu = estrategia;
#endif
                LAT_LED_R = 0;
                LAT_LED_A = 0;
                LAT_LED_V = 1;
                return;
            }
            
            if(sw()){
                if(direccion == DER) estrategia = E_ADELANTE;
                else estrategia = E_ATRAS;
            }
            
            setMotores(0, 0);
            LAT_LED_R = 1;
            LAT_LED_A = u(millis() > (m + (T_ESPERA / 2)));
            LAT_LED_V = 0;
            return;
            
        case E_CLASICA:
            estrategiaClasica();
            return;
            
        case E_CIEGO:
            estrategiaCiego();
            return;
            
        case E_ADELANTE:
            estrategiaAdelante();
            return;
            
        case E_ATRAS:
            estrategiaAtras();
            return;
    }
}

inline void estrategiaClasica(){
    static unsigned long m = 0;
    static char estado = ANALISIS;
    
    switch(estado){
        case ANALISIS:
            if(cny(1) || cny(2)){
                estado = ATRAS;
                m = millis();
                return;
            }
                        
            if(ir(3)){
                if(ir(2)){
                    if(ir(4)){
                        estado = ATAQUE;
                        return;
                    }
                    
                    estado = IZQAV;
                    return;
                }
                
                if(ir(4)){
                    if(ir(2)){
                        estado = ATAQUE;
                        return;
                    }
                    
                    estado = DERAV;
                    return;
                }
                
                estado = ATAQUE;
                return;
            }
            
            if(ir(2)){
                estado = IZQA;
                return;
            }
            
            if(ir(4)){
                estado = DERA;
                return;
            }
            
            if(ir(1)){
                estado = IZQ;
                return;
            }
            
            if(ir(5)){
                estado = DER;
                return;
            }
            
            estado = direccion;
            return;
            
        case ATAQUE:
            setMotores(V_ATAQUE, V_ATAQUE);
            estado = ANALISIS;
            return;
            
        case DERAV:
            setMotores(V_ATAQUE, V_ATAQUE / 2);
            direccion = DER;
            estado = ANALISIS;
            return;
            
        case IZQAV:
            setMotores(V_ATAQUE / 2, V_ATAQUE);
            direccion = IZQ;
            estado = ANALISIS;
            return;
            
        case ATRAS:
            if(millis() > (m + T_ATRAS)){
                estado = ANALISIS;                    
                return;
            }
            
            setMotores(V_ATRAS, V_ATRAS);
            return;
            
        case IZQA:
            setMotores(0, V_ATAQUE);
            direccion = IZQ;
            estado = ANALISIS;
            return;
            
        case DERA:
            setMotores(V_ATAQUE, 0);
            direccion = DER;
            estado = ANALISIS;
            return;
            
        case IZQ:
            setMotores(-V_GIRO, V_GIRO);
            direccion = IZQ;
            estado = ANALISIS;
            return;
            
        case DER:
            setMotores(V_GIRO, -V_GIRO);
            direccion = DER;
            estado = ANALISIS;
            return;
    }
}

inline void estrategiaCiego(){
    static unsigned long m = 0;
    static char estado = ANALISIS;
    
    switch(estado){
        case ANALISIS:
            if(cny(1) || cny(2)){
                estado = ATRAS;
                m = millis();
                return;
            }
            
            estado = ATAQUE;
            return;
            
        case ATAQUE:
            setMotores(V_ATAQUE, V_ATAQUE);
            estado = ANALISIS;
            return;
            
        case ATRAS:
            if(millis() > (m + T_ATRAS)){
                estado = direccion;
                m = millis();                    
                return;
            }
            
            setMotores(V_ATRAS, V_ATRAS);
            return;

        case DER:
            if(millis() > (m + T_GIRO_CIEGO)){
                estado = ANALISIS;
                return;
            }
            
            setMotores(V_GIRO, -V_GIRO);
            return;
            
        case IZQ:
            if(millis() > (m + T_GIRO_CIEGO)){
                estado = ANALISIS;
                return;
            }
            
            setMotores(-V_GIRO, V_GIRO);
            return;
    }
}

inline void estrategiaAdelante(){
    static unsigned long m = 0;
    static char estado = POSICIONAR;
    
    if(ir(1) || ir(2) || ir(3) || ir(4) || ir(5) || cny(1) || cny(2)){
        estadoMenu = E_CLASICA;
        return;
    }
    
    if(millis() < m)
        return;
    
    switch(estado){
        case POSICIONAR:
            setMotores(V_POSICIONAR, 0);
            m = millis() + T_POSICIONAR_ADELANTE;
            estado = AVANZAR;
            return;
            
        case AVANZAR:
            setMotores(V_AVANZAR, V_AVANZAR);
            m = millis() + T_AVANZAR;
            estado = FRENAR;
            return;
            
        case FRENAR:
            setMotores(0, 0);
            m = millis() + T_FRENAR;
            estado = AVANZAR;
            return;
    }
}

inline void estrategiaAtras(){
    static unsigned long m = 0;
    static char estado = POSICIONAR;
    
    if(ir(1) || ir(2) || ir(3) || ir(4) || ir(5) || cny(1) || cny(2)){
        estadoMenu = E_CLASICA;
        return;
    }
    
    if(millis() < m)
        return;
    
    switch(estado){
        case POSICIONAR:
            setMotores(0, -V_POSICIONAR);
            m = millis() + T_POSICIONAR_ATRAS;
            estado = AVANZAR;
            return;
            
        case AVANZAR:
            setMotores(V_AVANZAR, V_AVANZAR);
            m = millis() + T_AVANZAR;
            estado = FRENAR;
            return;
            
        case FRENAR:
            setMotores(0, 0);
            m = millis() + T_FRENAR;
            estado = AVANZAR;
            return;
    }
}

void main(){
    init();
    while(1) loop();
}