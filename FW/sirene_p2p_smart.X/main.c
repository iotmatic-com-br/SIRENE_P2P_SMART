////////////////////////////////////////////////////////////////////////////////
//// SIRENE SMART - de 9001 a 9099 !!
////
////    JP1 ABERTO  - RECEBE E DEPOIS REPLICA
////        FECHADO - "NAO REPLICA"
////    JP2 ABERTO  - Pisca Verde a cada  1s 
////        FECHADO - Pisca Verde a cada  5s 
////    JP3 ABERTO  -
////        FECHADO -
////    JP4 ABERTO  -
////        FECHADO -
////
////    Desligara AUTOMATICAMENTE após 20 minutos acionado              28/06/22
////  ---------- Correcao da replicacao infinita !!                     28/07/22
////
////    revisao                                                         09/12/24
////                PRODUCAO 2025 - INICIO                              20/08/25
//###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  
//
////    _____________________Vgit                                       07/04/26
//
#define     __ENDERECO_DESTE_MODULO__       9060 // ULTIMO GRAVADO 30/10/25
//
//###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  
////////////////////////////////////////////////////////////////////////////////
#define     K_MaxDelay              30      //delay max para replicacao
#define     SinalizaTransmissao             // led vermelho indica radio lora enviando
#define     indicaACverm        //acende vermelho se acionado

////////////////////////////////////////////////////////////////////////////////
#pragma warning disable 520
#include "mcc_generated_files/mcc.h"

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@                      SMART P2P
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
////////////////////////////////////////////////////////////////////////////////
//// Macros SMART
const uint8_t _RESET[]                = "ATZ\r";
const uint8_t _CFOFF[ ]               = "AT+TOFF\r";
const uint8_t _TXC22PING[]            = "AT+TXLRA=906700:0:PING\r";
//_____MACROS CANAIS
const uint8_t _TXSTART22[]            = "AT+TXLRA=906700:0:";
const uint8_t _CFC22[]                = "AT+TCONF=906700:22:125:12:4/8:1:1\r";
const uint8_t _RXC22[]                = "AT+RXLRA=906700:1\r";

/////////////////////////////////////////////////////////////////
//// vars
        //   SMART
enum
{
    fSM_OK           = 1,       //
    fSM_ATERROR      = 2,       // AT respondeu erro
    fSM_TOUT         = 3,       //  timeout 
    fSM_CFGERR       = 4,       // config retornou erro
    fSM_CHERR        = 5,       // canal selecionado errado
    fSM_D            = 6,       // 
    fSM_NORADIO      = 7        //
} SM_STATUSflags;
//modelo default
uint8_t     TIPO;
uint8_t     AcAliveBatOK,AcAliveBatLOW;
uint8_t     AcEVTBatOK,AcEVTBatLOW;

            //LORA
volatile  bool      fBusy,fAT,fATerr,fParm,fDetec,fOK,fTxOk,fAtOk;
volatile  bool      fTimeOut,fProgID,fErroID,fLigado,fRTX,fReplicar;
volatile  uint8_t   fRetLora,indice;
uint16_t  OFFSET,ENDERECO,TempoDeReplicacao;
volatile  uint8_t   DadosLora[64];
volatile  uint8_t   flags[6],ID[4];
volatile  uint8_t   pacoteTRX[14];
volatile  uint8_t   UltimoDadoRecebido,fComandoSirene;;
volatile  uint16_t  TotalSerial;
            //Sensor
volatile    uint8_t     indiceSensor,fRetSensor;
volatile    uint8_t     DadosSensor[8];

            //Outros
__persistent uint16_t    segsAlive;

uint8_t     fRepete,fEspera,HH,LL,loop,canal,dips;
uint16_t    tempoEspera,tMs,cicloLED,tempoRTX,contaRTX;
uint8_t     BatH,BatL;
uint8_t     temp8,rett;
uint16_t    temp16;
uint32_t    passagem;

uint8_t   M_EnderecoRecebido,C_EnderecoRecebido,D_EnderecoRecebido,U_EnderecoRecebido;
uint16_t    enderecoRecebido;
union Data {
   uint16_t end16;
   uint8_t  endhl[2];
} endereco,offset;

volatile uint8_t     endereco_M,endereco_C,endereco_D,endereco_U;

#define     bp              __nop()
#define     K_Toff          (20*60000)      // 20 min
#define     K_SegsFlash     5000
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ModuloOn()
{
    rstlora;
    __delay_ms(50);
    bp;    bp;    bp;    bp;
    runlora;
    bp;    bp;    bp;    bp;
    __delay_ms(1000);
}
void ModuloOff()
{
    rstlora;                    __delay_ms(10);
    RST_SetDigitalOutput();     __delay_ms(10);
//    TXSTA = 0;    RCSTA = 0;
//    RB5_SetDigitalInput();    RB5_SetLow();
//    RB2_SetDigitalInput();    RB2_SetLow();
}
////////////////////////////////////////////////////////////////////////////////
void ErrCode(uint8_t code)          // 1 a 13 = busy
{                                   //     20 = tx param
uint8_t qual = code;
    __nop();
    ModuloOff();
    for (;;)
    {
        VMon; __delay_ms(250); VMoff; __delay_ms(250); 
        VMon; __delay_ms(250); VMoff; __delay_ms(250); 
        VDon; __delay_ms(250); VDoff; __delay_ms(250); 
        VDon; __delay_ms(250); VDoff; __delay_ms(250); 
        __delay_ms(1000);
    }
}
uint8_t Espera_OK()
{
    temp16=0;   
    while (!fOK)
    {
        __delay_ms(1);
        if (++temp16>2000)          // timeout 2s sem resposta
            return fSM_TOUT;
        if (fATerr)
            return fSM_ATERROR;     // retornou erro do comando AT
    }
    return fSM_OK;      // OK
}
uint8_t Espera_TxOk()
{
    temp16=0;   
    while (indice<36)
    {
        __delay_ms(1);
        if (++temp16>5000)          // timeout 2s sem resposta
            return fSM_TOUT;
    }
        return fSM_OK;      // OK
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// envio das Strings de comando
////////////////////////////////////////////////////////////////////////////////
void SM_RESET()
{
    EUSART_Reset();
    for (loop=0; _RESET[loop]!=0; loop++) EUSART_Write (_RESET[loop]);
    __delay_ms(100);        
}
void SM_CONFIGRADIO(uint8_t ch)
{
    EUSART_Reset();
    for (loop=0; _CFC22[loop]!=0; loop++) EUSART_Write (_CFC22[loop]);    
    __delay_ms(100);  
}
void SM_MODORX(uint8_t ch)
{
    EUSART_Reset();
    for (loop=0; _RXC22[loop]!=0; loop++) EUSART_Write (_RXC22[loop]);
    __delay_ms(100);  
    EUSART_Reset();
}
void SM_CFOFF()
{
    EUSART_Reset();
    for (loop=0; _CFOFF[loop]!=0; loop++) EUSART_Write (_CFOFF[loop]);
    __delay_ms(100);  
}
void SM_PING()
{
    EUSART_Reset();
    for (loop=0; _TXC22PING[loop]!=0; loop++) EUSART_Write (_TXC22PING[loop]);
    __delay_ms(2000);  
}
//##############################################################################
uint8_t SM_START()
{
    EUSART_Reset();
    ModuloOn();
    __delay_ms(3000);    
    __nop();
    temp8=0;
    while (temp8<3)         // 3 tentativas
    {
        SM_RESET();
        temp16=0;
        while (++temp16<3000)             // 3s max
        {
            __delay_ms(1);
            if (fAtOk)
                return fSM_OK;
        }
        temp8++;
    }
    return fSM_NORADIO;
}
////////////////////////////////////////////////////////////////////////////////
void TX_Lora()
{
    __nop();
#ifdef     SinalizaTransmissao
    VDon;
#endif    
    /// DEinit do modulo - ja esta alimentado !!!
    ModuloOn();
    /// CONFIGURA CANAL 22 - VAI TRANSMITIR
    SM_CFOFF();
    __nop();
    SM_CONFIGRADIO(22); 
    __nop();
        
    //// envia pacote        //// SIRENE SEMPRE 22 !!
    EUSART_Reset();
    for (loop=0; _TXSTART22[loop]!=0; loop++) EUSART_Write (_TXSTART22[loop]);
//    //dados...[4]--[17]
    EUSART_Write(pacoteTRX[ 0]);
    EUSART_Write(pacoteTRX[ 1]);
    EUSART_Write(pacoteTRX[ 2]);
    EUSART_Write(pacoteTRX[ 3]);
    EUSART_Write(pacoteTRX[ 4]);
    EUSART_Write(pacoteTRX[ 5]);
    EUSART_Write(pacoteTRX[ 6]);
    EUSART_Write(pacoteTRX[ 7]);
    EUSART_Write(pacoteTRX[ 8]);
    EUSART_Write(pacoteTRX[ 9]);
    EUSART_Write(pacoteTRX[10]);
    EUSART_Write(pacoteTRX[11]);
    EUSART_Write(pacoteTRX[12]);
    EUSART_Write(pacoteTRX[13]);
    
    EUSART_Write (0x0D);
    Espera_TxOk();
    __nop();

#ifdef     SinalizaTransmissao
    VDoff;
#endif    
}
////////////////////////////////////////////////////////////////////////////
void TesteEnvio()
{
    while (Sensor0)
    { VMon; __delay_ms(50); VMoff; __delay_ms(50); }

    VMon;
    /// CONFIGURA CANAL 22 - VAI TRANSMITIR
    SM_CFOFF();
    __nop();
    SM_CONFIGRADIO(22); 
    __nop();

//            SM_PING();
    ENDERECO = __ENDERECO_DESTE_MODULO__;
    endereco_M = (uint8_t)(ENDERECO/1000);  // 9876 (pego o 9)
    temp16 = ENDERECO%1000;                 //resto 876
    endereco_C = (uint8_t)(temp16/100);     // 8
    temp16 = temp16%100;                    //resto 76
    endereco_D = (uint8_t)(temp16/10);      // 7
    endereco_U = (uint8_t)(temp16%10);

    endereco_M += 48;
    endereco_C += 48;
    endereco_D += 48;
    endereco_U += 48;
    __nop();

    EUSART_Reset();
    for (loop=0; _TXSTART22[loop]!=0; loop++) EUSART_Write (_TXSTART22[loop]);
    EUSART_Write('S');
    EUSART_Write('i');
    EUSART_Write('r');
    EUSART_Write('e');
    EUSART_Write('n');
    EUSART_Write('e');
    EUSART_Write(':');
    EUSART_Write(' ');
    EUSART_Write(endereco_M);
    EUSART_Write(endereco_C);
    EUSART_Write(endereco_D);
    EUSART_Write(endereco_U);
    EUSART_Write (0x0D);
    __delay_ms(2000);

    /// CONFIGURA CANAL 22 PARA RECEBER
    SM_CFOFF();
    SM_CONFIGRADIO(22); 
    SM_MODORX(22);
    SM_CFOFF();
    SM_CONFIGRADIO(22); 
    SM_MODORX(22);
    __nop();
    VMoff;
}
////////////////////////////////////////////////////////////////////////////
void main(void)
{
    __nop();
    __nop();
    //---------------------------
    //    SYSTEM_Initialize();
    PIN_MANAGER_Initialize();
    RST_SetDigitalOutput();
    rstlora;
    OSCILLATOR_Initialize();
    WDT_Initialize();
    WDTCONbits.SWDTEN=0;
    SIRENE_SetLow();
    SIRENE_SetDigitalMode();
    SIRENE_SetDigitalOutput();
    SireneOFF;
    ModuloOff();

    VDon; VMon;
    
    //---------------------------
    indice=0;
    fRetSensor=250;     // evita enviar ALEM DO ALIVE, apos o reset
    segsAlive=0;         // fora alive no boot
    
    EUSART_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    /// INICIALIZA O MODULO !!
    __nop();
    rett = SM_START();
    if (rett != fSM_OK)
        ErrCode(rett);
    __nop();

    /// CONFIGURA CANAL 22 PARA RECEBER
    VDoff; VMoff;
    __delay_ms(50);    
    SM_CFOFF();
    SM_CONFIGRADIO(22); 
    SM_MODORX(22);
    SM_CFOFF();
    SM_CONFIGRADIO(22); 
    SM_MODORX(22);
    __nop();

    VDon; VMon; __delay_ms(50); VDoff; VMoff; __delay_ms(50);    
    VDon; VMon; __delay_ms(50); VDoff; VMoff; __delay_ms(50);    
    VDon; VMon; __delay_ms(50); VDoff; VMoff; __delay_ms(150);    
    
    fLigado=0;
    indice=0;
    tempoRTX=0;
    passagem=0;
    contaRTX=0;
    cicloLED=50000;
    __nop();
    
    while (1)
    {
        __nop();
        if (Sensor0)
        {
            TesteEnvio();
        }
        
        //////////////////////// ciclo de 1ms
        __delay_ms(1);

        /////////////////////// flash led em modo normal
        if (fRTX==0)  cicloLED++;
        if (cicloLED==200)                      
        {
            VDoff;
        }
        else
        {
            if (cicloLED>=K_SegsFlash)     
            {
                VDon;
                cicloLED=0;
            }                                
        }

        /////////////////////// se acionada desliga sozinha apos dT
        if (fLigado)
        {
#ifdef indicaACverm
            VMon;
#endif            
            passagem++;
            if (passagem>K_Toff) 
            {
                passagem=K_Toff;
                __nop();
                RESET();
            }
        }
#ifdef indicaACverm
        else
            VMoff;
#endif            

        /////////////////////// delay para retransmissao
        if (fRTX==1)
        {
            if (tempoRTX < 65000)
                tempoRTX++;
            if (tempoRTX >= TempoDeReplicacao)
            {
                fRTX=0;
                VDoff;
                TX_Lora();
                __nop();
                /// CONFIGURA PARA RECEBER - canal 22
                SM_CFOFF();
                __nop();
                SM_CFOFF();
                SM_CONFIGRADIO(22); 
                SM_MODORX(22);
                SM_CFOFF();
                SM_CONFIGRADIO(22); 
                SM_MODORX(22);
                __nop();
                indice=0;
                tempoRTX=0;
                cicloLED=13000;
            }
        }

        //////////////////////// check radio lora
        if (fRetLora==99)
        {
            __nop();
            fRetLora=0;
        }
        if (fRetLora==1)
        {
            fRetLora=0;
            fComandoSirene=false;
            indice=0;

            ENDERECO = __ENDERECO_DESTE_MODULO__;

            __nop();
            ///DECOMPOE
            endereco_M = (uint8_t)(ENDERECO/1000);  // 9876 (pego o 9)
            temp16 = ENDERECO%1000;                 //resto 876
            endereco_C = (uint8_t)(temp16/100);     // 8
            temp16 = temp16%100;                    //resto 76
            endereco_D = (uint8_t)(temp16/10);      // 7
            endereco_U = (uint8_t)(temp16%10);      // 6

            OFFSET = ENDERECO-9000;
            __nop();
            switch (endereco_U)
            {
                case 1:
                case 6:
                    TempoDeReplicacao = 3;
                    break;
                case 2:
                case 7:
                    TempoDeReplicacao = 5;
                    break;
                case 3:
                case 8:
                    TempoDeReplicacao = 7;
                    break;
                case 4:
                case 9:
                    TempoDeReplicacao = 9;
                    break;
                case 5:
                case 0:
                    TempoDeReplicacao = 11;
                    break;
                default:
                    TempoDeReplicacao = 4;
                    break;
            }
            TempoDeReplicacao *= 1000;      
            
            ENDERECO -= OFFSET;             // 9876-876 = 9000
            endereco.end16 = ENDERECO;      // Ex.: 9000 - global // 9876 - esta peça
            offset.end16 = OFFSET;          // =876
            __nop();

            M_EnderecoRecebido = DadosLora[4]-48;
            C_EnderecoRecebido = DadosLora[5]-48;
            D_EnderecoRecebido = DadosLora[6]-48;
            U_EnderecoRecebido = DadosLora[7]-48;
            __nop();
            enderecoRecebido = M_EnderecoRecebido*1000;
            enderecoRecebido += C_EnderecoRecebido*100;
            enderecoRecebido += D_EnderecoRecebido*10;
            enderecoRecebido += U_EnderecoRecebido;
            __nop();
                    
            pacoteTRX[ 0] = DadosLora[ 2];
            pacoteTRX[ 1] = DadosLora[ 3];
            pacoteTRX[ 2] = DadosLora[ 4];
            pacoteTRX[ 3] = DadosLora[ 5];
            pacoteTRX[ 4] = DadosLora[ 6];
            pacoteTRX[ 5] = DadosLora[ 7];
            pacoteTRX[ 6] = DadosLora[ 8];
            pacoteTRX[ 7] = DadosLora[ 9];
            pacoteTRX[ 8] = DadosLora[10];
            pacoteTRX[ 9] = DadosLora[11];
            pacoteTRX[10] = DadosLora[12];
            pacoteTRX[11] = DadosLora[13];
            pacoteTRX[12] = DadosLora[14];
            pacoteTRX[13] = 'F';  //DadosLora[15]; // do GW recebo 'B'
                                                   // replicado recebo 'F'
            if (DadosLora[15]=='F')    //rx replicado, indica
            {
#ifdef indicaACverm
                VDon; __delay_ms(50);  VDoff; __delay_ms(50);
                VDon; __delay_ms(50);  VDoff; 
#endif                
            }
            __nop();

            // checa endereco GLOBAL
            if ( enderecoRecebido == endereco.end16 )
                fComandoSirene=true;
            
            // checa endereco individual
            endereco.end16 += offset.end16;         // aqui endereco.end16 foi somado ao offset.end16
            if ( enderecoRecebido == endereco.end16 )
                fComandoSirene=true;
            
            if (fComandoSirene)     // é comigo !!
            {
                __nop();
                fComandoSirene=false;
                passagem=0;
                // comando ligar ?
                if ( (DadosLora[ 8] == '1')&&(DadosLora[ 9] == '1')&&
                     (DadosLora[10] == '2')&&(DadosLora[11] == '2')&&
                     (DadosLora[12] == '3')&&(DadosLora[13] == '3') )
                {
                    if (fLigado==0) //NAO ESTAVA LIGADO !!
                    {
                        __nop();            // SIM !
                        fLigado = 1;
                        fRTX=1;                 //...replico
                        VDon;
                        cicloLED=0;
                        tempoRTX=0;
                        SireneON;
                    }
                    else
                        contaRTX=1;                        
                }
                /// desligar ?
                if ( (DadosLora[ 8] == '4')&&(DadosLora[ 9] == '4')&&
                     (DadosLora[10] == '5')&&(DadosLora[11] == '5')&&
                     (DadosLora[12] == '6')&&(DadosLora[13] == '6') )
                {
                    if (fLigado==1) //JA ESTAVA LIGADO !!
                    {
                        __nop();
                        fLigado=0;
                        fRTX=1;                 //...replico
                        VDon;
                        cicloLED=0;
                        tempoRTX=0;
                        SireneOFF;
                    }
                    else
                        contaRTX=2;                        
                }
            }
            /// CONFIGURA PARA RECEBER - canal 22
            SM_CFOFF();
            SM_CONFIGRADIO(22); 
            SM_MODORX(22);
            SM_CFOFF();
            SM_CONFIGRADIO(22); 
            SM_MODORX(22);
            __nop();
        }
    }
}
////////////////////////////////////////////////////////////////////////////
// EEPROM
//=========================================================
// Endereco 0, 1, 2 E 3 = ID em ascci - default 9876
__EEPROM_DATA(  '9','8','7','6',  2,  1, 54, 55 );  //   0--7 
// Random
__EEPROM_DATA(   52, 51, 51, 50, 71, 71, 70, 70 );  //   8 
// Random
__EEPROM_DATA(   14,212, 57,111, 21,  8, 11, 27 );  //  16
__EEPROM_DATA(   11,222,  4,  0,  0, 85, 36,232 );  //  24 
__EEPROM_DATA(   89,189,  9,  8, 43,243, 42, 31 );  //  32 
__EEPROM_DATA(   21,  0,  1,  2,  3,  4,  5,  6 );  //  40 
__EEPROM_DATA(   56, 55, 55, 54, 68, 68, 68, 67 );  //  48 
__EEPROM_DATA(   91, 90, 90, 89, 38, 37, 37,  0 );  //  56--63

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/**      End of File        */