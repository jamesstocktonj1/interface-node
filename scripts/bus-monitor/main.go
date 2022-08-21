package main

import (
	"flag"
	"fmt"
	"go.bug.st/serial"
	"os"
)

var (
	port        *string
	baud        *int
	defaultPort *bool
)

func main() {

	flagSet := flag.NewFlagSet("Bus Monitor", flag.ContinueOnError)

	baud = flagSet.Int("baud", 9600, "select Baud Rate")
	defaultPort = flagSet.Bool("d", false, "select Default Serial Port")

	flagSet.Parse(os.Args[1:])

	port = new(string)
	*port = flagSet.Args()[0]

	serialPort, err := serial.Open(*port, &serial.Mode{BaudRate: *baud})

	if err == nil {
		fmt.Printf("Open Port: %s\n", *port)
	} else {
		fmt.Printf("Error Opening Port: %s\n", *port)
	}

	serialPort.Close()
}
