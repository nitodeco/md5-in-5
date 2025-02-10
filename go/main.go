package main

import (
	"fmt"
	"md5in5/md5in5"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <string>")
		os.Exit(1)
	}

	input := os.Args[1]
	hash := md5in5.MD5(input)
	fmt.Println(hash)
}
