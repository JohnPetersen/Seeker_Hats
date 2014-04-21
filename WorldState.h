
#define MSG_LEN 11
#define MSG_TAG 0xff
#define MSG_END 0x00

class WorldState
{
  pubilc:
    byte alarm;
    long lat;
    long lon;
    
    WorldState() { lat = lon = 0; alarm = 0; };
    
    // fill the buffer with a state message
    void getMessage(byte buff[]);
    // update this state with the contents of the buffer
    boolean update(byte buff[]);
  private:
    long getLong(byte buff[]);
    void setLong(byte buff[], long l);
}

