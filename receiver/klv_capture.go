package main

import (
	"fmt"
	"log"
	"net"
	"time"

	betterklv "github.com/hadar225/AWS_hadar/betterklv"
)

const (
	multicastAddr = "235.235.235.235:1234"
	pidKLV        = 0xFD
	tsPacketSize  = 188
	timeoutSec    = 10
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

	fmt.Printf("🔍 Listening for MPEG-TS packets on %s via interface %s\n", multicastAddr, ifaceName)

	buffer := make([]byte, tsPacketSize*7)

	var klvBuffer []byte
	var collecting bool
	var timer *time.Timer

	resetTimer := func() {
		if timer != nil {
			timer.Stop()
		}
		timer = time.AfterFunc(timeoutSec*time.Second, func() {
			if collecting && len(klvBuffer) > 0 {
				fmt.Printf("⏰ Timeout reached — Finalizing KLV PES (%d bytes)\n", len(klvBuffer))
				parseKLV(klvBuffer)
				klvBuffer = nil
				collecting = false
			}
		})
	}

	for {
		n, src, err := conn.ReadFromUDP(buffer)
		if err != nil {
			log.Printf("ReadFromUDP failed: %v", err)
			continue
		}

		fmt.Printf("📩 UDP packet received: %d bytes from %v\n", n, src)

		for i := 0; i+tsPacketSize <= n; i += tsPacketSize {
			packet := buffer[i : i+tsPacketSize]
			if len(packet) != tsPacketSize || packet[0] != 0x47 {
				continue
			}

			pid := int(packet[1]&0x1F)<<8 | int(packet[2])
			if pid != pidKLV {
				continue
			}

			pusi := (packet[1] & 0x40) != 0
			fmt.Printf("🎯 Got KLV PID packet: 0x%X (PUSI: %v)\n", pid, pusi)

			// חשב את ההיסט של ה־payload
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

			payload := packet[payloadOffset:]
			fmt.Printf("📤 TS packet payload (%d bytes): %v\n", len(payload), payload)

			if pusi {
				if collecting && len(klvBuffer) > 0 {
					fmt.Printf("📦 Finalized previous KLV PES (%d bytes)\n", len(klvBuffer))
					parseKLV(klvBuffer)
				}
				klvBuffer = make([]byte, 0, 512)
				collecting = true
			}

			if collecting {
				klvBuffer = append(klvBuffer, payload...)
				resetTimer()

				// אם הפקטה הזו קטנה מ־188 – ייתכן שזו הפקטה האחרונה
				if len(payload) < (tsPacketSize - payloadOffset) {
					fmt.Printf("📦 Finalized KLV PES (last TS packet detected, %d bytes)\n", len(klvBuffer))
					parseKLV(klvBuffer)
					klvBuffer = nil
					collecting = false
					if timer != nil {
						timer.Stop()
					}
				}
			}
		}
	}
}

func parseKLV(pes []byte) {
	if len(pes) < 9 {
		fmt.Println("❌ PES too short")
		return
	}

	pesHeaderLength := int(pes[8])
	klvStart := 9 + pesHeaderLength
	if klvStart >= len(pes) {
		fmt.Println("❌ No KLV payload in PES")
		return
	}

	klvData := pes[klvStart:]
	fmt.Printf("📦 Extracted KLV data (%d bytes)\n", len(klvData))
	fmt.Println("klvPacket:", klvData)

	res, err := betterklv.HandleKLVBuffer(klvData[5:])
	if err != nil {
		fmt.Println("❌ Error parsing KLV:", err)
	} else {
		fmt.Println("✅ Decoded KLV buffer:", res)
	}
}