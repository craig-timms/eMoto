#ifndef MCP23016_H
#define MCP23016_H

class MCP23016
{
  public:
    void setup();
    void readAll();
    int readArr();

  private:
    void configurePort(uint8_t port, uint8_t custom);
    void writeBlockData(uint8_t cmd, uint8_t data);
    void checkButton(uint8_t GP);
    uint8_t readPin(uint8_t pin, uint8_t gp);
    uint8_t valueFromPin(uint8_t pin, uint8_t statusGP);

};

#endif MCP23016_H
