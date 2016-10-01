class ZedBoard
{
  char *ptr;
  int fd;

  public:
    ZedBoard();
    ~ZedBoard();
    void RegisterWrite(int offset, int value);
    int RegisterRead(int offset);
    void setLed(int led, int value);
    int CheckError();

};
