/**
    eusart.c
*/

/**
  Section: Included Files
*/
#include "eusart.h"
#include "stdint.h"

/**
  Section: Macro Declarations
*/

#define EUSART_TX_BUFFER_SIZE 32
#define EUSART_RX_BUFFER_SIZE 64

/**
  Section: Global Variables
*/
volatile uint8_t eusartTxHead = 0;
volatile uint8_t eusartTxTail = 0;
volatile uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
volatile uint8_t eusartTxBufferRemaining;

volatile uint8_t eusartRxHead = 0;
volatile uint8_t eusartRxTail = 0;
volatile uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
volatile eusart_status_t eusartRxStatusBuffer[EUSART_RX_BUFFER_SIZE];
volatile uint8_t eusartRxCount;
volatile eusart_status_t eusartRxLastError;

/////////////////////////////////////////////////////////////////
//// vars
extern  volatile  bool      fBusy,fAT,fATerr,fParm,fDetec,fOK,fTxOk,fAtOk;
extern  volatile  bool      fTimeOut,fProgID,fErroID,fLigado,fRTX;
extern  volatile  uint8_t   fRetLora,indice;
extern  volatile  uint8_t   DadosLora[64];
extern  volatile  uint8_t   flags[6],ID[4];
extern  volatile  uint8_t   IDsensor[4];
extern  volatile  uint8_t   UltimoDadoRecebido;
extern  volatile  uint16_t  TotalSerial;

/**
  Section: EUSART APIs
*/
void (*EUSART_TxDefaultInterruptHandler)(void);
void (*EUSART_RxDefaultInterruptHandler)(void);

void (*EUSART_FramingErrorHandler)(void);
void (*EUSART_OverrunErrorHandler)(void);
void (*EUSART_ErrorHandler)(void);

void EUSART_DefaultFramingErrorHandler(void);
void EUSART_DefaultOverrunErrorHandler(void);
void EUSART_DefaultErrorHandler(void);

//....
void EUSART_Reset(void)
{ 
    for (indice=0;indice<64;indice++)
    {
        DadosLora[indice]=0x20;
        eusartRxBuffer[indice]=0x20;
    }
    eusartTxBuffer[0]=0x20;     eusartTxBuffer[1]=0x20;     eusartTxBuffer[2]=0x20;     
    eusartTxBuffer[3]=0x20;     eusartTxBuffer[4]=0x20;     eusartTxBuffer[5]=0x20;     
    eusartTxBuffer[6]=0x20;     eusartTxBuffer[7]=0x20;     
    indice=0;
    TotalSerial=0;
    fOK=0;  fATerr=0; fAtOk=0;
    fBusy=0;fParm=0;fTxOk=0;
}

void EUSART_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RCIE = 0;
    EUSART_SetRxInterruptHandler(EUSART_Receive_ISR);
    PIE1bits.TXIE = 0;
    EUSART_SetTxInterruptHandler(EUSART_Transmit_ISR);
    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUDCON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TXSTA = 0x24;

    // SPBRGL 160; 
    SPBRGL = 0xA0;

    // SPBRGH 1; 
    SPBRGH = 0x01;


    EUSART_SetFramingErrorHandler(EUSART_DefaultFramingErrorHandler);
    EUSART_SetOverrunErrorHandler(EUSART_DefaultOverrunErrorHandler);
    EUSART_SetErrorHandler(EUSART_DefaultErrorHandler);

    eusartRxLastError.status = 0;

    // initializing the driver state
    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;

    // enable receive interrupt
    PIE1bits.RCIE = 1;
}

bool EUSART_is_tx_ready(void)
{
    return (eusartTxBufferRemaining ? true : false);
}

bool EUSART_is_rx_ready(void)
{
    return (eusartRxCount ? true : false);
}

bool EUSART_is_tx_done(void)
{
    return TXSTAbits.TRMT;
}

eusart_status_t EUSART_get_last_status(void){
    return eusartRxLastError;
}

uint8_t EUSART_Read(void)
{
    uint8_t readValue  = 0;
    
    while(0 == eusartRxCount)
    {
        CLRWDT();
    }

    eusartRxLastError = eusartRxStatusBuffer[eusartRxTail];

    readValue = eusartRxBuffer[eusartRxTail++];
    if(sizeof(eusartRxBuffer) <= eusartRxTail)
    {
        eusartRxTail = 0;
    }
    PIE1bits.RCIE = 0;
    eusartRxCount--;
    PIE1bits.RCIE = 1;

    return readValue;
}

void EUSART_Write(uint8_t txData)
{
    while(0 == eusartTxBufferRemaining)
    {
        CLRWDT();
    }

    if(0 == PIE1bits.TXIE)
    {
        TXREG = txData;
    }
    else
    {
        PIE1bits.TXIE = 0;
        eusartTxBuffer[eusartTxHead++] = txData;
        if(sizeof(eusartTxBuffer) <= eusartTxHead)
        {
            eusartTxHead = 0;
        }
        eusartTxBufferRemaining--;
    }
    PIE1bits.TXIE = 1;
}


void EUSART_Transmit_ISR(void)
{

    // add your EUSART interrupt custom code
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining)
    {
        TXREG = eusartTxBuffer[eusartTxTail++];
        if(sizeof(eusartTxBuffer) <= eusartTxTail)
        {
            eusartTxTail = 0;
        }
        eusartTxBufferRemaining++;
    }
    else
    {
        PIE1bits.TXIE = 0;
    }
}

void EUSART_Receive_ISR(void)
{
    
    eusartRxStatusBuffer[eusartRxHead].status = 0;

    if(RCSTAbits.FERR){
        eusartRxStatusBuffer[eusartRxHead].ferr = 1;
        EUSART_FramingErrorHandler();
    }

    if(RCSTAbits.OERR){
        eusartRxStatusBuffer[eusartRxHead].oerr = 1;
        EUSART_OverrunErrorHandler();
    }
    
    if(eusartRxStatusBuffer[eusartRxHead].status){
        EUSART_ErrorHandler();
    } else {
        EUSART_RxDataHandler();
    }
    
    // or set custom function using EUSART_SetRxInterruptHandler()
}

void EUSART_DefaultFramingErrorHandler(void){}

void EUSART_DefaultOverrunErrorHandler(void){
    // EUSART error - restart

    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;

}

void EUSART_DefaultErrorHandler(void){
    EUSART_RxDataHandler();
}

void EUSART_SetFramingErrorHandler(void (* interruptHandler)(void)){
    EUSART_FramingErrorHandler = interruptHandler;
}

void EUSART_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    EUSART_OverrunErrorHandler = interruptHandler;
}

void EUSART_SetErrorHandler(void (* interruptHandler)(void)){
    EUSART_ErrorHandler = interruptHandler;
}

void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_TxDefaultInterruptHandler = interruptHandler;
}

void EUSART_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_RxDefaultInterruptHandler = interruptHandler;
}

/////////////////////////////////////////////////////////////////
void EUSART_RxDataHandler(void)
{
    TotalSerial++;
   	UltimoDadoRecebido = RCREG;
    if (UltimoDadoRecebido == '>')
        indice=0;
    flags[0]=flags[1];
    flags[1]=flags[2];
    flags[2]=flags[3];
    flags[3]=flags[4];
    flags[4]=flags[5];
    flags[5]=UltimoDadoRecebido;

    if (indice<64)
    {
        DadosLora[indice]= UltimoDadoRecebido;
        indice++;
    }

    if ((flags[1]=='t')&&(flags[2]=='x')&&(flags[4]=='o')&&(flags[5]=='k'))
        fTxOk=1;
    else if ((flags[2]==0x0D)&&(flags[3]==0x0A)&&(flags[4]=='O')&&(flags[5]=='K'))
        fOK=1;
    if ((flags[2]=='O')&&(flags[3]=='T')&&(flags[4]=='A')&&(flags[5]=='A'))
        fAtOk=1;
    else if ((flags[1]=='A')&&(flags[2]=='T')&&(flags[3]=='_')&&(flags[4]=='E')&&(flags[5]=='R'))
        fATerr=1;
           
//    if (indice>15)
//        fRetLora=99;
    if (indice==17)         // apos o > no indice '0'
    {                       // > EA9050112233EB<><>   17 BYTES
        __nop();            // \=0           \_14 
        if ( (DadosLora[ 2] == 'E')&&
             (DadosLora[ 3] == 'A')&&
             (DadosLora[14] == 'E')
              )
        {
            fRetLora=1;
        }
	}	

    if ( (DadosLora[2]=='I')&&(DadosLora[7]=='D') )
    {
        fProgID=1;    
        ID[0] = DadosLora[3];   ID[1] = DadosLora[4];
        ID[2] = DadosLora[5];   ID[3] = DadosLora[6];
    }
    __nop(); 
    __nop();
    __nop();
    
    eusartRxBuffer[eusartRxHead++] = UltimoDadoRecebido;
    if(sizeof(eusartRxBuffer) <= eusartRxHead)
    {
        eusartRxHead = 0;
    }
    eusartRxCount++;
}
/**
  End of File
*/
