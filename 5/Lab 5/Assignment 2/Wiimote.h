class Wiimote
{
	int fd;

	public:

	// Constructor
	Wiimote();

	// Destructor
	~Wiimote();

	void Listen();
	
	void ButtonEvent(int code, int value);
};
