
class RoboticWiimote
{

  // File descriptor for Wiimote
  int w_fd;

  // File descriptor for RoboticArm
  int r_fd;

  // Mapped address for RoboticArm
  char *ptr;

  //int wiimote_x;
  //int wiimote_y;
  //int wiimote_z;

  // Write a value into the given memory offset in the memory-mapped I/O.
 	void RegisterWrite(unsigned offset, unsigned value);

  public:

    // Constructor
    RoboticWiimote();

    // Destructor
    ~RoboticWiimote();

    // Listens for Wiimote input
    void Listen();

    // Called when the Wiimote moves
    void AccelerationEvent(int code, int acceleration);

    // Move a servo to a target position with the given speed. ARgument
  	// 'angle' is a value between 0 and 180. Argument 'speed' in an integer
  	// greater than 0, given in angles per second.
  	//
  	// Argument 'id' can take the following values:
  	// 0 - Base
  	// 1 - Bicep
  	// 2 - Elbow
  	// 3 - Wrist
  	// 4 - Gripper
  	//    void MoveServo(int id, int angle, int speed);
    void MoveServo(int id, int angle, int speed);


};
