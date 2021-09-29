/*
 * Gerardo Fuentes
 * Laboratorio 7
 * TIVA C TMC4C123 80Mhz
 */

/**********************************************************************/
/*----------------------------Librerias-------------------------------*/
/**********************************************************************/
#include <SPI.h>
#include <SD.h>

/**********************************************************************/
/*----------------------------ARCHIVOS FILE---------------------------*/
/**********************************************************************/
Sd2Card card;                //Variables de librería sd
SdVolume volume;
SdFile root;
File ARCH;                   //Variable para abrir archivos

int pb1 = PUSH1;             //Botón a utilizar
const int chipSelect = PA_3; //cs PIN

/**********************************************************************/
/*----------------------------SETUP-----------------------------------*/
/**********************************************************************/
void setup() {
  Serial.begin(9600);                    //Inicialización com. serial a 9600 baudrate
  SPI.setModule(0);                      //Inicialización com. SPI
  pinMode(PA_3, OUTPUT);                 //ChipSelect como output
  pinMode(pb1, INPUT_PULLUP);            //Botón para desplegar menú
 
  if (!card.init(SPI_HALF_SPEED, chipSelect)) { //Inicialización de tarjeta para menú
    Serial.println("initialization failed!");
    return;
  }
 if (!SD.begin(PA_3)) {                         //Inicialización de tarjeta para opciones
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
   
  volume.init(card);                            //Inicialización de información general de la tarjeta

}

/**********************************************************************/
/*------------------------------LOOP----------------------------------*/
/**********************************************************************/
void loop() {
  int onoff = digitalRead(pb1);             //Lectura del PB1 para generar condición

  if(onoff == LOW){                         //Condición para desplegar menú
  Serial.println("\nMenú de Archivos: ");   //Título para el menú
  Serial.println("");
  root.openRoot(volume);                    //Función para obtener los datos de los archivos de la SD
  root.ls(LS_R | LS_DATE | LS_SIZE);        // Crea una lista de los archivos de la SD
  }
    int OPC = (int)Serial.read();           //Envío de datos a monitor serial por el usuario

    
    if(OPC == 49){                               //ASCII "3" envía el contenido de 3.txt
        ARCH = SD.open("1.txt");                 //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
         
          while (ARCH.available()) {             //Mientras haya contenido
            Serial.write(ARCH.read());           //Leerlo y escribirlo en monitor serial
          }

          ARCH.close();                          //Cerrar el archivo
        } else {                                 //De lo contrario 

          Serial.println("error opening 1.txt"); //Enviar mensaje de error
        }  
    }

    if(OPC == 50){                               //ASCII "2" envía el contenido de 2.txt
        ARCH = SD.open("2.txt");                 //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
         
          while (ARCH.available()) {             //Mientras haya contenido
            Serial.write(ARCH.read());           //Leerlo y escribirlo en monitor serial
          }

          ARCH.close();                          //Cerrar el archivo
        } else {                                 //De lo contrario 

          Serial.println("error opening 2.txt"); //Enviar mensaje de error
        }
    }

    if(OPC == 51){                               //ASCII "3" envía el contenido de 3.txt
        ARCH = SD.open("3.txt");                 //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
         
          while (ARCH.available()) {             //Mientras haya contenido
            Serial.write(ARCH.read());           //Leerlo y escribirlo en monitor serial
          }

          ARCH.close();                          //Cerrar el archivo
        } else {                                 //De lo contrario 

          Serial.println("error opening 3.txt"); //Enviar mensaje de error
        }  
    }
  }
