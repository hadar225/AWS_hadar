package main

import (
	"fmt"
	"log"
	"net"
	betterklv "github.com/hadar225/AWS_hadar/betterklv"
)

const (
	multicastAddr = "235.235.235.235:1234"
	pidKLV        = 0xFD
	tsPacketSize  = 188
)

func main() {
	ifaceName := "eth0"

	iface, err := net.InterfaceByName(ifaceName)
	if err != nil {
		log.Fatalf("Interface %s not found: %v", ifaceName, err)
	}

	addr, err := net.ResolveUDPAddr("udp", multicastAddr)
	if err != nil {
		log.Fatalf("Failed to resolve address: %v", err)
	}

	conn, err := net.ListenMulticastUDP("udp", iface, addr)
	if err != nil {
		log.Fatalf("Failed to listen on multicast: %v", err)
	}
	defer conn.Close()

	fmt.Printf("\U0001F50D Listening for MPEG-TS packets on %s via interface %s\n", multicastAddr, ifaceName)

	buffer := make([]byte, tsPacketSize*7)

	for {
		n, src, err := conn.ReadFromUDP(buffer)
		if err != nil {
			log.Printf("ReadFromUDP failed: %v", err)
			continue
		}

		fmt.Printf("\U0001F4E9 UDP packet received: %d bytes from %v\n", n, src)

		for i := 0; i+tsPacketSize <= n; i += tsPacketSize {
			packet := buffer[i : i+tsPacketSize]
			if len(packet) < tsPacketSize || packet[0] != 0x47 {
				continue
			}

			pid := int(packet[1]&0x1F)<<8 | int(packet[2])

			if pid == pidKLV {
				fmt.Printf("\U0001F3AF Got KLV PID packet: 0x%X (%d)\n", pid, pid)

				adaptationField := packet[3] >> 4 & 0x3
				payloadOffset := 4
				if adaptationField == 2 || adaptationField == 3 {
					adaptationLength := int(packet[4])
					payloadOffset += 1 + adaptationLength
				}

				if payloadOffset >= len(packet) {
					fmt.Println("❌ Invalid TS packet payload offset")
					continue
				}

				pes := packet[payloadOffset:]
				if len(pes) < 9 || !(pes[0] == 0x00 && pes[1] == 0x00 && pes[2] == 0x01) {
					fmt.Println("❌ Not a PES packet")
					continue
				}

				pesHeaderLength := int(pes[8])
				klvStart := 9 + pesHeaderLength
				if klvStart >= len(pes) {
					fmt.Println("❌ No KLV payload in PES")
					continue
				}

				klvData := pes[klvStart:]
				fmt.Printf("📦 Extracted KLV data (%d bytes)\n", len(klvData))
				parseKLV(klvData)
			} else {
				fmt.Printf("ℹ️ PID: 0x%X (%d)\n", pid, pid)
			}
		}
	}
}

func parseKLV(klvData []byte) {
	fmt.Println("klvPacket:", klvData)
	res, err := betterklv.HandleKLVBuffer(klvData[5:])
		if err != nil {
		fmt.Println("Error parsing KLV:", err)
		return
	} else {
		fmt.Println("Decoded KLV buffer:", res)
	}
}