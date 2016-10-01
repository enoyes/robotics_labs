class Wiimote
{
	int fd;

	public:

	// Constructor
	Wiimote();

	// Destructor
	~Wiimote();

	void Listen();
	
	void AccelerationEvent(int code, int acceleration);
};
