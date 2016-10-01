class Wiimote
{
	int fd;

	public:

	// Constructor
	Wiimote();

	// Destructor
	~Wiimote();

	void Listen();

	virtual void AccelerationEvent(int code, int acceleration);
};
