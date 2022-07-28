// communication definition file


// flow values
#define TRANSMIT        1
#define RECEIVE         0


// communication status
#define STATE           0           // state [0:3]
#define FLOW1           4           // flow 1 [4]
#define FLOW2           5           // flow 2 [5]

// preset states
#define RESET_STATE     0
#define COM_START       4           // green states
#define COM_END         8
#define COM_MID         12
#define COM_TIMEOUT     1           // red states
#define COM_UNDEFINED   2