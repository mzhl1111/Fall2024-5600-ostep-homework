package main

import (
	"fmt"
	"net/http"
)

var (
	maxConcurrentRequests = 100
	semaphore             = make(chan struct{}, maxConcurrentRequests)
)

func handler(w http.ResponseWriter, r *http.Request) {
	semaphore <- struct{}{} 
	defer func() { <-semaphore }() 

	fmt.Fprintln(w, "Hello, World!")
	w.WriteHeader(http.StatusOK) 
	w.Write([]byte("OK"))    
}

func main() {
	http.HandleFunc("/", handler)
	http.ListenAndServe(":8080", nil)
}