#include <Wire.h>
#include <SPI.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// i2c address
#define ADDRESS 0x6A
#define MAX_I2C_BUFFER 2 // --
#define REG_COUNT 256 // simulated memory


// pins
const int AMP_EN_1 = 4; // FLASH_WP
const int AMP_EN_2 = 5; // AMP_ALERT
const int AMP_EN_3 = 6; // AMP_EN

const int AMP_MODE_1 = 7; // AMP_CTRL
const int AMP_MODE_2 = 8; // AMP_Config
const int AMP_MODE_3 = 9; // FLASH_RST
// SS 10 - AMP CS1/CS2/CS3/CS4
// MOSI 11 - AMP_SO
// SCK 13 - AMP_SCLK
const int pins[6] = {AMP_EN_1, AMP_EN_2, AMP_EN_3, AMP_MODE_1, AMP_MODE_2, AMP_MODE_3};


// Serial
volatile uint8_t opcode;
volatile uint8_t response[2] = {0x01, 0x01};
volatile uint8_t response_index = 0;
volatile uint8_t response_length = 0;
volatile bool waiting_for_opcode = true; 
volatile bool spi_opcode_recieved = false;

// i2c
volatile byte lastByte;
volatile byte i2c_buffer[MAX_I2C_BUFFER];
volatile byte length = 0;

volatile byte registers[REG_COUNT];
volatile byte reg_pointer = 0;
volatile bool pointer_set = false;
volatile bool write_mode = false;

// program vars
char buffer[100];


void setup() {
  // gpio input
  for (int i = 0; i < 6; i++){
    pinMode(pins[i], INPUT);
  }
  // serial
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  SPCR |= _BV(SPE) | _BV(SPIE); // SPI enable
  SPDR = 0x00;
  sei();

  //i2c
  Wire.begin(ADDRESS);
  Wire.setClock(100000); 
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);
  Serial.println("\n\n");
  Serial.println("I2C ready");

  registers[0x9f] = 0;
}


void loop() {
  // print out opcode
  if (spi_opcode_recieved){
    //Serial.print("Opcode: 0x");
    //Serial.println(opcode, HEX);
    spi_opcode_recieved = false;
  }
}

// ISR triggered when SPI byte received
ISR (SPI_STC_vect){
  uint8_t received = SPDR; // read byte
  
  if (waiting_for_opcode){
    opcode = received;
    spi_opcode_recieved = true;

    // process opcode
    switch (opcode) {
      case 0x05:
        response[0] = 0b00010010;
        response[1] = 0b00000000;
        response_length = 2;
        break;
      case 0x60: // erase
        response_length = 0;
        break;
        
      default:
        response[0] = 0xEA;
        response[1] = 0xEA;
        response_length = 0;
        break;
    }

    response_index = 0;

    if (response_length > 0){
      SPDR = response[response_index++];
    } else {
      SPDR = 0x00;
      waiting_for_opcode = true;
    }
    
    waiting_for_opcode = (response_length <= 1);
  } else {
    if (response_index < response_length){
      SPDR = response[response_index++];
    } else {
      // reset to wait for next opcode
      waiting_for_opcode = true;
      SPDR = 0x00;
    }
  }
}


// Handle i2c send
void recieveEvent(int numBytes){
  // length = 0;
  // while (Wire.available() && length < MAX_I2C_BUFFER){
  //   i2c_buffer[length++] = Wire.read();
  // }

  // snprintf(buffer, sizeof(buffer), "Recieved 0x%02x at address 0x%02x", i2c_buffer[1], i2c_buffer[0]);
  // Serial.println(buffer);
  // lastByte = i2c_buffer[1];
  // Serial.println(lastByte, HEX);


  if (numBytes == 0) return;
  reg_pointer = Wire.read(); // first byte is register address
  pointer_set = true;
  write_mode = (Wire.available() > 0); //if there's more data its a write

  int i = 0;
  while (Wire.available() && reg_pointer + i < REG_COUNT){
    byte data = Wire.read();
    registers[reg_pointer + i] = data;

    //snprintf(buffer, sizeof(buffer), "Recieved 0x%02x at address 0x%02x", data, reg_pointer);
    //Serial.println(buffer);

    i++;
  }
}


// Handle i2c receive
void requestEvent(){
  if (!pointer_set){
    Wire.write(0x00);
    return;
  }

  Wire.write(registers[reg_pointer]);
  //snprintf(buffer, sizeof(buffer), "Read from reg 0x%02x: 0x%02x", reg_pointer, registers[reg_pointer]);
  //Serial.println(buffer);
}