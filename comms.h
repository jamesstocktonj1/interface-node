// communication definition file


// flow values
#define TRANSMIT        1
#define RECEIVE         0

#define TX_DELAY        1

// communication parameters
#define BUFFER_SIZE     12
#define SEND_START_CHAR 'S'
#define REPLY_START_CHAR 'R'
#define REPLY_ACK 'A'
#define REPLY_NACK 'N'
#define END_CHAR 'E'

// communication status
#define STATE           0           // state [0:3]
#define FLOW1           4           // flow 1 [4]
#define FLOW2           5           // flow 2 [5]